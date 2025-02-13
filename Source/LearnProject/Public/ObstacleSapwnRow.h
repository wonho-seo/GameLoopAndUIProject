// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObstacle.h"
#include "Engine/DataTable.h"
#include "ObstacleSapwnRow.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FObstacleSapwnRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObstacleName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseObstacle> ObstacleClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;
};
