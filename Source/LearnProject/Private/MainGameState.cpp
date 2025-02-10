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

	TArray<FWaveRow*> WaveRows;
	const FString ContextString(TEXT("WaveContexts"));
	WaveDataTable->GetAllRows(ContextString, WaveRows);

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (WaveRows.IsValidIndex(CurrentWaveIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex));
		int32 ItemToSpawn = WaveRows[CurrentWaveIndex]->SpawnCount;
		float WaveDuration = WaveRows[CurrentWaveIndex]->WaveDuration;
		for (int32 i = 0; i < ItemToSpawn; i++)
		{
			if (FoundVolumes.Num() > 0)
			{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
				if (SpawnVolume)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
					SpawnedItems.Add(SpawnedActor);
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
	for (AActor* Actor : SpawnedItems)
	{
		if (ABaseItem* Item = Cast<ABaseItem>(Actor))
		{
			Item->Destroy();
		}
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
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

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
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));

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
