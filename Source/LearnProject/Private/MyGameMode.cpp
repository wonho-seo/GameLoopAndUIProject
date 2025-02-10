// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MainPlayerController.h"
#include "MainCharacter.h"
#include "MainGameState.h"
AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
	GameStateClass = AMainGameState::StaticClass();
}
