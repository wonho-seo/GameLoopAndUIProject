// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "WaveRow.h"

AMainGameState::AMainGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;
	IsAppearThunder = false;
	IsAppearSpike = false;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUPdateTimerHandle,
		this,
		&AMainGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 AMainGameState::GetScore() const
{
	return Score;
}

void AMainGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* SpartaGameInstance = Cast<UMainGameInstance>(GameInstance);

		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddtoScore(Amount);
		}
	}
}

void AMainGameState::OnRestartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance))
		{
			MainGameInstance->TotalScore = 0;
			WaveClear();
			if (CurrentLevelIndex >= MaxLevels)
			{
				CurrentLevelIndex = MaxLevels - 1;
			}
			CurrentLevelIndex--;
			EndLevel();
		}
	}
}

void AMainGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			MainPlayerController->ShowGameHUD();
		}
	}
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* SpartaGameInstance = Cast<UMainGameInstance>(GameInstance);

		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	StartWave();
	
}

void AMainGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	IsAppearThunder = false;
	IsAppearSpike = false;

	TArray<FWaveRow*> WaveRows;
	const FString ContextString(TEXT("WaveContexts"));
	WaveDataTable->GetAllRows(ContextString, WaveRows);
	SpawnedActors.Empty();

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (WaveRows.IsValidIndex(CurrentWaveIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex));
		int32 ItemToSpawn = WaveRows[CurrentWaveIndex]->SpawnCount;
		float WaveDuration = WaveRows[CurrentWaveIndex]->WaveDuration;
		UDataTable* ObstacleSpawnTable = WaveRows[CurrentWaveIndex]->ObstacleSpawnTables;

		if (FoundVolumes.Num() > 0)
		{
			if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
			{
				if (ObstacleSpawnTable)
				{
					TArray<FObstacleSapwnRow*> ObstacleSpawnRows;
					const FString ObstacleContextString(TEXT("ObstacleContexts"));
					ObstacleSpawnTable->GetAllRows(ObstacleContextString, ObstacleSpawnRows);

					for (FObstacleSapwnRow* ObstacleSpawnRow : ObstacleSpawnRows)
					{
						for (int32 SpawnCount = 0; SpawnCount < ObstacleSpawnRow->SpawnCount; SpawnCount++)
						{
							AActor* SpawnedObstacle = SpawnVolume->SpawnObstacle(ObstacleSpawnRow->ObstacleClass);
							if (SpawnedObstacle)
							{
								SpawnedActors.Add(SpawnedObstacle);
								IsAppearSpike = true;
							}
						}
					}
				}

				if (CurrentWaveIndex == 2)
				{
					GetWorld()->GetTimerManager().SetTimer(
						ThunderTimerHandle,
						this,
						&AMainGameState::OnThunderAttack,
						3.0f,
						true
					);
				}
				for (int32 i = 0; i < ItemToSpawn; i++)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItemInZone();
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
					SpawnedActors.Add(SpawnedActor);
				}

				if (SpawnedCoinCount == 0)
				{
					*SpawnedActors.begin() = SpawnVolume->SpawnItemInZone(ACoinItem::StaticClass());
					SpawnedCoinCount++;
				}
			}
		}

		GetWorldTimerManager().SetTimer(
			WaveTimerHandle,
			this,
			&AMainGameState::OnWaveTimeUp,
			WaveDuration,
			false
		);
	}
	else
	{
		EndLevel();
	}
}
void AMainGameState::OnWaveTimeUp()
{
	EndWave();
	StartWave();
}

void AMainGameState::WaveClear()
{

	if (GetWorldTimerManager().IsTimerActive(WaveTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	}

	for (AActor* Actor : SpawnedActors)
	{
		//if (GetWorld())
		//{
		//	GetWorld()->GetTimerManager().ClearAllTimersForObject(Actor);
		//}
		if (ABaseItem* Item = Cast<ABaseItem>(Actor))
		{
			Item->Destroy();
		}
		if (ABaseObstacle* Obstacle = Cast<ABaseObstacle>(Actor))
		{
			Obstacle->Destroy();
		}
	}

	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(ThunderTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ThunderTimerHandle);
	}
}

void AMainGameState::EndWave()
{
	WaveClear();
	CurrentWaveIndex++;
}

void AMainGameState::OnLevelTimeUp()
{
	EndLevel();
}
void AMainGameState::OnCoinClamped()
{
	CollectedCoinCount++;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
		StartWave();
	}
}

void AMainGameState::EndLevel()
{
	CurrentWaveIndex = 0;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* SpartaGameInstance = Cast<UMainGameInstance>(GameInstance);

		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}

}

void AMainGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MainPlayerController->GetHUDWidget())
			{
				
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
						if (MainGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MainGameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d - %d"), CurrentLevelIndex + 1, CurrentWaveIndex + 1)));

				}
				if (UTextBlock* SpikeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("SpikeText"))))
				{
					if (IsAppearSpike)
					{
						SpikeText->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						SpikeText->SetVisibility(ESlateVisibility::Hidden);

					}

				}
				if (UTextBlock* ThunderText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ThunderText"))))
				{
					if(SpawnedThunder && SpawnedThunder->IsPendingKillPending())
					{
						ThunderText->SetVisibility(ESlateVisibility::Hidden);
						SpawnedThunder = nullptr;
					}
					else if (SpawnedThunder)
					{
						ThunderText->SetVisibility(ESlateVisibility::Visible);

					}
				}
			}
		}
	}
}

void AMainGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast< AMainPlayerController>(PlayerController))
		{
			MainPlayerController->SetPause(true);
			MainPlayerController->ShowGameOver();
		}
	}
}

void AMainGameState::OnThunderAttack()
{
	if (GetWorld() && ThunderActor && FMath::RandBool())
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (APawn* Player = PlayerController->GetPawn())
			{
				SpawnedThunder = GetWorld()->SpawnActor<AThunderActor>(
					ThunderActor,
					Player->GetActorLocation(),
					FRotator::ZeroRotator
				);
			}
		}
	}
}