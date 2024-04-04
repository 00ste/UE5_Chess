// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"

UENUM()
enum MoveType : uint32 {
	MOVE, CAPTURE, PROMOTE, NONE
};

// TODO: Create ChessMove class and CaptureMove, MoveMove, PromoteMove
// as subclasses. ChessMove abstract class only declares a GenerateSAN
// method
USTRUCT()
struct FChessMove {
	GENERATED_BODY()

	FChessMove();
	FChessMove(FVector2D StartPosition, FVector2D EndPosition,
		FVector2D CapturePosition, MoveType Type);

	FVector2D StartPosition;
	FVector2D EndPosition;
	FVector2D CapturePosition;
	MoveType Type;

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

	void BindToMove(FChessMove Move);

	// Getters
	FChessMove GetMove() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	FChessMove Move;

};
