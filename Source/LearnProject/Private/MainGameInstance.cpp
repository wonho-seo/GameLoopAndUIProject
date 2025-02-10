// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UMainGameInstance::AddtoScore(int32 Amount)
{
	TotalScore += Amount;
}
