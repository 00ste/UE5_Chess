// Fill out your copyright notice in the Description page of Project Settings.


#include "Indicator.h"

// Sets default values
AIndicator::AIndicator()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

void AIndicator::Setup(MoveType Type_, FVector2D StartPos, FVector2D EndPos)
{
	Type = Type_;
	StartPosition = StartPos;
	EndPosition = EndPos;
}

MoveType AIndicator::GetType() const
{
	return Type;
}

FVector2D AIndicator::GetStartPosition() const
{
	return StartPosition;
}

FVector2D AIndicator::GetEndPosition() const
{
	return EndPosition;
}

// Called when the game starts or when spawned
void AIndicator::BeginPlay()
{
	Super::BeginPlay();

}

