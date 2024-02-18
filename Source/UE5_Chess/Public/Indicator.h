// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"

UENUM()
enum MoveType : uint32 {
	MOVE, CAPTURE, PROMOTE, NONE
};

UCLASS()
class UE5_CHESS_API AIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicator();

	void Setup(MoveType Type, FVector2D StartPos, FVector2D EndPos);

	// Getters
	MoveType GetType() const;
	FVector2D GetStartPosition() const;
	FVector2D GetEndPosition() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	MoveType Type;
	FVector2D StartPosition;
	FVector2D EndPosition;

};
