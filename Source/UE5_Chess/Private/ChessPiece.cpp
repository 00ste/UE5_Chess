// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"
#include "CH_GameMode.h"

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

void AChessPiece::Setup(PieceType Type_, PieceColor Color_)
{
	Type = Type_;
	Color = Color_;
}

void AChessPiece::SelfDestroy()
{
	Destroy();
}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	// TODO: call this from gamemode when adding the ChessPieces, can't do it here
	// because the Chessboard pointer is not public.
	// ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
	// GameMode->Chessboard->OnResetEvent.AddDynamic(this, &AChessPiece::SelfDestroy());
}


