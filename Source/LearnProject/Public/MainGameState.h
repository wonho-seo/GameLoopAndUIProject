// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ThunderActor.h"
#include "MainGameState.generated.h"
/**
 * 
 */
UCLASS()
class LEARNPROJECT_API AMainGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMainGameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	UDataTable* WaveDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<AThunderActor> ThunderActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUPdateTimerHandle;
	FTimerHandle ThunderTimerHandle;
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	UFUNCTION(BlueprintCallable, Category = "Thunder")
	void OnThunderAttack();

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnRestartLevel();
	void StartLevel();
	void WaveClear();
	void OnLevelTimeUp();
	void OnCoinClamped();
	void EndWave();
	void EndLevel();
	void UpdateHUD();

	void StartWave();

	void OnWaveTimeUp();
private:
	TArray<AActor*> SpawnedActors;
	bool IsAppearSpike;
	bool IsAppearThunder;
	AThunderActor* SpawnedThunder;

};
