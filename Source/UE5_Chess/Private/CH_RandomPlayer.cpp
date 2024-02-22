// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_RandomPlayer.h"

// Sets default values
ACH_RandomPlayer::ACH_RandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACH_RandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACH_RandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACH_RandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACH_RandomPlayer::OnTurn()
{
}

void ACH_RandomPlayer::OnWin()
{
}

void ACH_RandomPlayer::OnLose()
{
}

