// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "MainGameState.h"
#include "Components/TextBlock.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerController::AMainPlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    VisualMenuWidgetInstance(nullptr)
{
}

UUserWidget* AMainPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void AMainPlayerController::ShowGameHUD()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    if (VisualMenuWidgetInstance)
    {
        VisualMenuWidgetInstance->RemoveFromParent();
        VisualMenuWidgetInstance = nullptr;
    }

    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
        }

        AMainGameState* MainGameState = GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
        if (MainGameState)
        {
            MainGameState->UpdateHUD();
        }
    }
}

void AMainPlayerController::ShowMainMenu()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    if (VisualMenuWidgetInstance)
    {
        VisualMenuWidgetInstance->RemoveFromParent();
        VisualMenuWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass)
    {
        VisualMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (VisualMenuWidgetInstance)
        {
            VisualMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());

            UFunction* PlayAnimFunc = VisualMenuWidgetInstance->FindFunction(FName("PlayTitleAnim"));
            if (PlayAnimFunc)
            {
                VisualMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
            }
        }
    }
}

void AMainPlayerController::ShowSelectLevel()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    if (VisualMenuWidgetInstance)
    {
        VisualMenuWidgetInstance->RemoveFromParent();
        VisualMenuWidgetInstance = nullptr;
    }

    if (SelectLevelMenuWidgetClass)
    {
        VisualMenuWidgetInstance = CreateWidget<UUserWidget>(this, SelectLevelMenuWidgetClass);

        if (VisualMenuWidgetInstance)
        {
            VisualMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }
    }
}
void AMainPlayerController::ShowGameOver()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    if (VisualMenuWidgetInstance)
    {
        VisualMenuWidgetInstance->RemoveFromParent();
        VisualMenuWidgetInstance = nullptr;
    }

    if (GameOverMenuWidgetClass)
    {
        VisualMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);

        if (VisualMenuWidgetInstance)
        {
            VisualMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());

            UFunction* PlayAnimFunc = VisualMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
            if (PlayAnimFunc)
            {
                VisualMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
            }
        }
    }
}
void AMainPlayerController::StartGame(int32 CurrentLevel)
{
    if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        MainGameInstance->CurrentLevelIndex = CurrentLevel;
        MainGameInstance->TotalScore = 0; 
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
    SetPause(false);
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MenuLevel"))
    {
        ShowMainMenu();
    }
}
