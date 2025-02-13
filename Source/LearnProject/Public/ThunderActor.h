// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThunderActor.generated.h"

class USphereComponent;
UCLASS()
class LEARNPROJECT_API AThunderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThunderActor();


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystem* ExplosionParticleClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USoundBase* ExplosionSoundClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float ExplosionDamageAmount;

	void Explosion();
	FTimerHandle ExplosionTimerHandler;
	FTimerHandle DistoryExplosionParticleTimerHandler;
};
