// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "MainGameState.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    OverheadHpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget1"));
    OverheadHpWidget->SetupAttachment(GetMesh());
    OverheadHpWidget->SetWidgetSpace(EWidgetSpace::Screen);

    OverheadDebuffWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget2"));
    OverheadDebuffWidget->SetupAttachment(GetMesh());
    OverheadDebuffWidget->SetWidgetSpace(EWidgetSpace::Screen);

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    TempSpeedRate = 1.0f;
    SlowCount = 0;
    SlowTimerDelegete = nullptr;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    MaxHealth = 100;
    Health = MaxHealth;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    UpdateOverheadHp();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMainCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMainCharacter::StopSprint
                );
            }
        }
    }

}

float AMainCharacter::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    UpdateOverheadHp();

    if (Health <= 0.0f)
    {
        OnDeath();
    }
    return ActualDamage;
}

void AMainCharacter::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    FVector2D MoveInput = value.Get<FVector2D>();
    if (IsReverseControl)
    {
        MoveInput = MoveInput * -1.0f;
    }

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AMainCharacter::StartJump(const FInputActionValue& value)
{  
    if (value.Get<bool>())
    {
        Jump();
    }
}

void AMainCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void AMainCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AMainCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed * TempSpeedRate;
    }
}

void AMainCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * TempSpeedRate;
    }
}

void AMainCharacter::OnDeath()
{
    if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>())
    {
        MainGameState->OnGameOver();
    }
}

void AMainCharacter::UpdateOverheadHp()
{
    if (!OverheadHpWidget) return;

    if (UUserWidget* OverheadWidgetInstance = OverheadHpWidget->GetUserWidgetObject())
    {
        if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHp"))))
        {
                HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
        }

    }
    
}

void AMainCharacter::UpdateOverheadDeBuff()
{

    if (!OverheadDebuffWidget) return;

    if (UUserWidget* OverheadWidgetInstance = OverheadDebuffWidget->GetUserWidgetObject())
    {
        if (UTextBlock* DeBuffText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadDeBuff"))))
        {
            FString DisPlayText = "";
            float Time = 0.0f;

            if (SlowCount > 0)
            {
                if (GetWorld())
                {
                    Time = GetWorld()->GetTimerManager().GetTimerRemaining(SlowTimerHandler);
                }

                DisPlayText += FString::Printf(TEXT("%.0fSec Slow overlap : &d!"), Time, SlowCount);

            }
            if (IsReverseControl)
            {
                if (GetWorld())
                {
                    Time = GetWorld()->GetTimerManager().GetTimerRemaining(ReverseTimerHandler);
                }

                DisPlayText += FString::Printf(TEXT("%.0fSec ReverseKey!\n"), Time);

            }

            DeBuffText->SetText(FText::FromString(DisPlayText));
        }
    }

}

float AMainCharacter::GetHealth() const
{
    return Health;
}

void AMainCharacter::AddHealth(float Amount)
{
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    UpdateOverheadHp();
}

void AMainCharacter::AddSlowSpeed(float Rate, float Time)
{
    
    SlowTimerDelegete.BindUFunction(this, FName("SlowSpeedRollBack"), Rate);
    SlowCount++;
    TempSpeedRate -= Rate;
    GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * TempSpeedRate;
    GetWorld()->GetTimerManager().SetTimer(
        SlowTimerHandler,
        SlowTimerDelegete,
        Time,
        false);
    UpdateOverheadDeBuff();
}

void AMainCharacter::SlowSpeedRollBack(float Rate)
{
    SlowCount = 0;
    TempSpeedRate = 1.0f;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    UpdateOverheadDeBuff();

}

void AMainCharacter::ReverseControl(float Time)
{
    IsReverseControl = true;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            ReverseTimerHandler,
            this,
            &AMainCharacter::ConverseControl,
            Time,
            false
        );
    }
    UpdateOverheadDeBuff();

}

void AMainCharacter::ConverseControl()
{
    IsReverseControl = false;
    UpdateOverheadDeBuff();

}

