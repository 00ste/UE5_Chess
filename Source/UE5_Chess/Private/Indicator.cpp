// Fill out your copyright notice in the Description page of Project Settings.


#include "Indicator.h"

// Sets default values
AIndicator::AIndicator()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

void AIndicator::BindToMove(FChessMove Move_)
{
	Move = Move_;
}

FChessMove AIndicator::GetMove() const
{
	return Move;
}

// Called when the game starts or when spawned
void AIndicator::BeginPlay()
{
	Super::BeginPlay();

}

FChessMove::FChessMove()
{
	this->StartPosition = FVector2D(-6, -9);
	this->EndPosition = FVector2D(-6, -9);
	this->CapturePosition = FVector2D(-6, -9);
	this->Type = MoveType::NONE;
}

FChessMove::FChessMove(FVector2D StartPosition, FVector2D EndPosition, FVector2D CapturePosition, MoveType Type)
{
	this->StartPosition = StartPosition;
	this->EndPosition = EndPosition;
	this->CapturePosition = CapturePosition;
	this->Type = Type;
}

FString FChessMove::GenerateSAN()
{
	return FString("C2");
}
