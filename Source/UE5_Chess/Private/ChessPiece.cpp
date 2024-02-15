// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

PieceType AChessPiece::GetType() const
{
	return Type;
}

PieceColor AChessPiece::GetColor() const
{
	return Color;
}

void AChessPiece::Setup(PieceType Type, PieceColor Color)
{
	this->Type = Type;
	this->Color = Color;
}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	
}


