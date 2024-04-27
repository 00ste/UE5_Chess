// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CH_PlayerInterface.h"
#include "CH_RandomPlayer.generated.h"

UCLASS()
class UE5_CHESS_API ACH_RandomPlayer : public APawn, public ICH_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACH_RandomPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;
	virtual void OnDraw() override;
};
