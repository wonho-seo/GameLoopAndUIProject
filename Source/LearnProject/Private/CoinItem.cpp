// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "MainGameState.h"
ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AMainGameState* GameState = World->GetGameState<AMainGameState>())
			{
				GameState->AddScore(PointValue);

				GameState->OnCoinClamped();
			}
		}
		DestroyItem();
	}
}
