// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

UENUM()
enum PieceColor : uint32 {
	WHITE = 0, BLACK = 1, NONE = 2
};

UENUM()
enum PieceType : uint32 {
	BISHOP, KING, KNIGHT, PAWN, QUEEN, ROOK, NONE
};

UCLASS()
class UE5_CHESS_API AChessPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();

	// Getters
	PieceType GetType() const;
	PieceColor GetColor() const;

	// Initialisation function (passing parameters in
	// constructor or BeginPlay isn't allowed)
	void Setup(PieceType Type, PieceColor Color);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	PieceType Type;
	PieceColor Color;
};
