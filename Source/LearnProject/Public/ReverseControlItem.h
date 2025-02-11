// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseControlItem.generated.h"

/**
 * 
 */
UCLASS()
class LEARNPROJECT_API AReverseControlItem : public ABaseItem
{
public:
	AReverseControlItem();

	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ClampMin = "0.0"))
	float Time;

	virtual void ActivateItem(AActor* Activator) override;
};
