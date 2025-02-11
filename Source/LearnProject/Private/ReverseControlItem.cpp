// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseControlItem.h"
#include "MainCharacter.h"

AReverseControlItem::AReverseControlItem():
	Time(0.0f)
{
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Activator))
		{
			MainCharacter->ReverseControl(Time);
		}

		DestroyItem();
	}
}
