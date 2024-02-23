// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CH_GameInstance.h"
#include "CH_PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CH_HumanPlayer.generated.h"

UCLASS()
class UE5_CHESS_API ACH_HumanPlayer : public APawn, public ICH_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACH_HumanPlayer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	UFUNCTION()
	void OnClick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UCameraComponent* Camera;
	UCH_GameInstance* GameInstance;
	FVector2D SelectedPosition;
	const FVector2D DESELECTED = { -1, -1 };
	bool IsMyTurn = false;
};
