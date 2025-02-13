// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class LEARNPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	virtual void BeginPlay() override;
	void SortZones(int32 StartIndex);
	// Sets default values for this actor's properties
	ASpawnVolume();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 ZoneRowCount;


	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();
	
	FItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomRoundPointInVolume() const;
	AActor* SpawnObstacle(TSubclassOf<AActor> ObstacleClass);
	void ResizeZone(int32 Size);
	FVector GetRandomZoneInVolume(int32 ZoneIndex);
	FVector GetRandomPointInZone(int32 ZoneIndex);
	AActor* SpawnRandomItemInZone();
	AActor* SpawnItemInZone(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;

	TArray<int32> Zones;
	FVector ZoneRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 ZoneIndex;
};
