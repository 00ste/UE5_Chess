// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.h"
#include "Indicator.generated.h"

/*
UENUM()
enum MoveType : uint32 {
	MOVE, CAPTURE, PROMOTE, NONE
};
*/

USTRUCT()
struct FChessMove {
	GENERATED_BODY()

	// Constructor for moves that don't capture
	FChessMove(FVector2D StartPosition_, FVector2D EndPosition_);
	// Constructor for moves that capture
	FChessMove(FVector2D StartPosition_, FVector2D EndPosition_,
		FVector2D CapturePosition_);
	// Constructor for moves that promote
	FChessMove(FVector2D StartPosition_, FVector2D EndPosition_,
		bool bDoesCapture_, FVector2D CapturePosition_,
		PieceType PromotionTarget_);
	// Full constructor
	FChessMove(FVector2D StartPosition_, FVector2D EndPosition_,
		bool bDoesCapture_, FVector2D CapturePosition_,
		bool bDoesPromote_, PieceType PromotionTarget_);
	// Default constructor (never used)
	FChessMove() = default;

	FVector2D StartPosition;
	FVector2D EndPosition;
	bool bDoesCapture;
	FVector2D CapturePosition;
	bool bDoesPromote;
	PieceType PromotionTarget;

	// Generates the Standard Algebraic Notation (SAN) String
	// associated with the ChessMove
	// FString GenerateSAN();
};

UCLASS()
class UE5_CHESS_API AIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicator();

	/*
	void BindToMove(FChessMove Move);

	// Getters
	FChessMove GetMove() const;
	*/
	FChessMove Move;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
private:	
	FChessMove Move;
	*/

};
