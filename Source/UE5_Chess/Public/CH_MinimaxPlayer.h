// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CH_PlayerInterface.h"
#include "CH_MinimaxPlayer.generated.h"

UCLASS()
class UE5_CHESS_API ACH_MinimaxPlayer : public APawn, public ICH_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACH_MinimaxPlayer();

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

private:
	uint32 MaxDepth = 3;

	uint32 EvaluateState() const;
	int32 AlphaBetaMiniMax(uint32 Depth, int32 Alpha, int32 Beta, bool bIsMax);
};
