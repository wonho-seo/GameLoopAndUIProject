// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObstacle.h"
#include "SpikeObstacle.generated.h"

/**
 * 
 */
UCLASS()
class LEARNPROJECT_API ASpikeObstacle : public ABaseObstacle
{
	GENERATED_BODY()
	
public:
	ASpikeObstacle();

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Properties")
	float UpTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Properties")
	float MaxMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Properties")
	float DownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Properties")
	float HitDamageAmount;

	void HitObstacle(AActor* actor) override;

	void UpSpike();

	void DownSpike();

private:
	FTimerHandle UpTimerHandler;
	FTimerHandle DownTimerHandler;
	
};
