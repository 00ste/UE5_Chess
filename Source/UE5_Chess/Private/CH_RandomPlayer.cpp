// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_RandomPlayer.h"
#include "CH_GameMode.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("AI turn!"));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
		TArray<FChessMove> AllMovesList = GameMode->CalculateAllFullyLegalMoves(PieceColor::PBLACK);
		uint32 RandomIndex = FMath::Rand() % AllMovesList.Num();
		GameMode->DoFinalMove(AllMovesList[RandomIndex]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Move done!"));
		GameMode->UpdateChessboard();
		GameMode->TurnNextPlayer();
	}, 2, false);
}

void ACH_RandomPlayer::OnWin()
{
}

void ACH_RandomPlayer::OnLose()
{
}

