// Fill out your copyright notice in the Description page of Project Settings.


#include "ThunderActor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
AThunderActor::AThunderActor() :
	ExplosionDelay(0.0f),
	ExplosionDamageAmount(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneRootComponent);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(SceneRootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneRootComponent);
}

// Called when the game starts or when spawned
void AThunderActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			ExplosionTimerHandler,
			this,
			&AThunderActor::Explosion,
			ExplosionDelay,
			false
		);
	}
}

void AThunderActor::Explosion()
{
	if (!GetWorld()) return;

	TArray<AActor*> Actors;
	CollisionComponent->GetOverlappingActors(Actors);

	UParticleSystemComponent* ExplosionParticle = nullptr;
	if (ExplosionParticleClass)
	{
		ExplosionParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticleClass,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}
	if (ExplosionSoundClass)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			ExplosionSoundClass,
			GetActorLocation()
		);
	}
	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamageAmount,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}

	Destroy();

	if (ExplosionParticle)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DistoryExplosionParticleTimerHandler,
			[ExplosionParticle]()
			{
				ExplosionParticle->DestroyComponent();
			},
			2.0f,
			false
		);
	}
}
