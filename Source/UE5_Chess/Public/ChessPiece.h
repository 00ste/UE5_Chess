// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

UENUM()
enum PieceColor : uint32 {
	BLACK, WHITE, NONE
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
