// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class LEARNPROJECT_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slowing", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SlowingSpeedRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slowing", meta = (ClampMin = "0.0"))
	float SlowingTime;

	virtual void ActivateItem(AActor* Activator) override;
};
