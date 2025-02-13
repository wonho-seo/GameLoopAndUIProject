// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeObstacle.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

ASpikeObstacle::ASpikeObstacle():
	UpTime(0.0f),
	DownTime(0.0f),
	HitDamageAmount(0.0f),
	MaxMove(0.0f)
{


}

void ASpikeObstacle::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(GetActorLocation());
	if (UpTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			UpTimerHandler,
			this,
			&ASpikeObstacle::UpSpike,
			UpTime + DownTime,
			true
		);
	}
	if (DownTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DownTimerHandler,
			this,
			&ASpikeObstacle::DownSpike,
			UpTime + DownTime,
			true,
			UpTime
		);
	}
}


void ASpikeObstacle::HitObstacle(AActor* Activator)
{
	Super::HitObstacle(Activator);

	if (Activator && Activator->ActorHasTag("Player") && GetWorld())
	{
		UGameplayStatics::ApplyDamage(
			Activator,
			HitDamageAmount,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}
}

void ASpikeObstacle::UpSpike()
{
	SetActorLocation(GetActorLocation() + FVector{ 0, 0, MaxMove });
}
void ASpikeObstacle::DownSpike()
{
	SetActorLocation(GetActorLocation() + FVector{ 0, 0, -MaxMove });
}

