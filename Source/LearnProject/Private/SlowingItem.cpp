// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "MainCharacter.h"
ASlowingItem::ASlowingItem():
	SlowingSpeedRate(0.0f),
	SlowingTime(0.0f)
{
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Activator))
		{
			MainCharacter->AddSlowSpeed(SlowingSpeedRate, SlowingTime);
		}

		DestroyItem();
	}
}