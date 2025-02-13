// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObstacle.h"
#include "Components/BoxComponent.h"

// Sets default values
ABaseObstacle::ABaseObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRootComponent);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionComponent->SetupAttachment(SceneRootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(SceneRootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseObstacle::OnObstacleBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseObstacle::OnObstacleEndOverlap);

}

void ABaseObstacle::OnObstacleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		HitObstacle(OtherActor);
	}
}

void ABaseObstacle::OnObstacleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseObstacle::HitObstacle(AActor* Activator)
{
}

void ABaseObstacle::DestroyObstacle()
{
	Destroy();
}

