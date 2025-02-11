// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;
UCLASS()
class LEARNPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	void AddSlowSpeed(float Rate, float Time);
	UFUNCTION()
	void SlowSpeedRollBack(float Rate);

	void ReverseControl(float Time);
	void ConverseControl();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadHpWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadDebuffWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float TempSpeedRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SlowCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsReverseControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void OnDeath();
	void UpdateOverheadHp();
	void UpdateOverheadDeBuff();
	FTimerHandle SlowTimerHandler;
	FTimerDelegate SlowTimerDelegete;

	FTimerHandle ReverseTimerHandler;
};
