// Fill out your copyright notice in the Description page of Project Settings.


#include "Indicator.h"
#include "Chessboard.h"

// Sets default values
AIndicator::AIndicator()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

/*
void AIndicator::BindToMove(FChessMove Move_)
{
	Move = Move_;
}

FChessMove AIndicator::GetMove() const
{
	return Move;
}
*/

// Called when the game starts or when spawned
void AIndicator::BeginPlay()
{
	Super::BeginPlay();

}

FChessMove::FChessMove(FVector2D StartPosition_, FVector2D EndPosition_,
	bool bDoesCapture_, FVector2D CapturePosition_, bool bDoesPromote_,
	PieceType PromotionTarget_)
	: StartPosition(StartPosition_), EndPosition(EndPosition_), bDoesCapture(bDoesCapture_),
	CapturePosition(CapturePosition_), bDoesPromote(bDoesPromote_),
	PromotionTarget(PromotionTarget_)
{

}

FChessMove::FChessMove(FVector2D StartPosition_, FVector2D EndPosition_)
{
	FChessMove(StartPosition_, EndPosition_, false, FVector2D(-6, -9),
		false, PieceType::PTNONE);
}

FChessMove::FChessMove(FVector2D StartPosition_, FVector2D EndPosition_,
	FVector2D CapturePosition_)
{
	FChessMove(StartPosition_, EndPosition_, true, CapturePosition_,
		false, PieceType::PTNONE);
}

FChessMove::FChessMove(FVector2D StartPosition_, FVector2D EndPosition_,
	bool bDoesCapture_, FVector2D CapturePosition_,
	PieceType PromotionTarget_)
{
	FChessMove(StartPosition_, EndPosition_, bDoesCapture_, CapturePosition_,
		true, PromotionTarget_);
}

/*
FString FChessMove::GenerateSAN()
{
	// Moving piece
	// Start position (only if strictly necessary)
	// Capture
	// End position
	// = Promotion
	return FString("C2");
}
*/
