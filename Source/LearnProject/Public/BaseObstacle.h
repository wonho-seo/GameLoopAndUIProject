// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleInterface.h"

#include "BaseObstacle.generated.h"


class UBoxComponent;
UCLASS()
class LEARNPROJECT_API ABaseObstacle : public AActor, public IObstacleInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	USceneComponent* SceneRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	UBoxComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	UStaticMeshComponent* StaticMeshComponent;

	// Sets default values for this actor's properties
	ABaseObstacle();

	// IObstacleInterface을(를) 통해 상속됨
	void OnObstacleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnObstacleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void HitObstacle(AActor* Activator) override;
	virtual void DestroyObstacle();

};
