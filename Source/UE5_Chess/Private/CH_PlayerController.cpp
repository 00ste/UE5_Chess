// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_PlayerController.h"
#include "CH_HumanPlayer.h"

ACH_PlayerController::ACH_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ACH_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = Cast<ACH_HumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}
}

void ACH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CH_Context, 0);
	}
}

void ACH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ACH_PlayerController::ClickOnGrid);
	}
}
