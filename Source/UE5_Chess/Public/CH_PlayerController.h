// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CH_HumanPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API ACH_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACH_PlayerController();

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* CH_Context;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ClickAction;

	void ClickOnGrid();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
