// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
	ZoneRowCount = 0;
	ZoneIndex = 0;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	ResizeZone(ZoneRowCount * ZoneRowCount);
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();

	ZoneRange = FVector(
		BoxExtent.X / ZoneRowCount,
		BoxExtent.Y / ZoneRowCount,
		0);

}
void ASpawnVolume::SortZones(int32 StartIndex)
{
	for (; StartIndex < Zones.Num(); StartIndex++)
	{
		if (StartIndex == Zones[StartIndex])
		{
			int SwapIndex = FMath::RandRange(0, Zones.Num() - 1);
			Zones.Swap(StartIndex, SwapIndex);
		}
	}
}

void ASpawnVolume::ResizeZone(int32 Size)
{
	if (Size > Zones.Num())
	{
		int StartIndex = Zones.Num();
		Zones.SetNum(Size);
		for (int Index = StartIndex; Index < Zones.Num(); Index++)
		{
			Zones[Index] = Index;
		}
		SortZones(StartIndex);
	}

}

FVector ASpawnVolume::GetRandomZoneInVolume(int32 ZonesIndex)
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation() - FVector(BoxExtent.X, BoxExtent.Y, 0) + ZoneRange;

	return BoxOrigin + FVector(ZoneRange.X * 2 * (ZonesIndex % ZoneRowCount), ZoneRange.Y * 2 * (ZonesIndex / ZoneRowCount), 0);
}

FVector ASpawnVolume::GetRandomPointInZone(int32 ZonesIndex)
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();

	return GetRandomZoneInVolume(ZonesIndex) + FVector(
		FMath::FRandRange(-ZoneRange.X, ZoneRange.X),
		FMath::FRandRange(-ZoneRange.Y, ZoneRange.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnRandomItemInZone()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItemInZone(ActualClass);
		}
	}
	return nullptr;
}
AActor* ASpawnVolume::SpawnItemInZone(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	if (ZoneIndex >= Zones.Num())
	{
		ZoneIndex = 0;
	}
	FVector Temp = GetRandomPointInZone(ZoneIndex);
	UE_LOG(LogTemp, Warning, TEXT("Index : %d, ZoneIndex : %d, Position : %s"), ZoneIndex, Zones[ZoneIndex], *Temp.ToString());
	return GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInZone(Zones[ZoneIndex++]),
		FRotator::ZeroRotator
	);
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->Spawnchance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->Spawnchance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	return GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
}

FVector ASpawnVolume::GetRandomRoundPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		-BoxExtent.Z
	);

}
AActor* ASpawnVolume::SpawnObstacle(TSubclassOf<AActor> ObstacleClass)
{
	if (!ObstacleClass) return nullptr;

	return GetWorld()->SpawnActor<AActor>(
		ObstacleClass,
		GetRandomRoundPointInVolume(),
		FRotator::ZeroRotator
	);
}

