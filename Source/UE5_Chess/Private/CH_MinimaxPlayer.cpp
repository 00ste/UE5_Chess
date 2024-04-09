// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_MinimaxPlayer.h"
#include "CH_GameMode.h"

// Sets default values
ACH_MinimaxPlayer::ACH_MinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACH_MinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACH_MinimaxPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACH_MinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACH_MinimaxPlayer::OnTurn()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());

		FChessMove BestMove;
		int32 BestScore = -69420;

		for (FChessMove MoveCandidate : GameMode->CalculateAllFullyLegalMoves(OwnedColor))
		{
			GameMode->DoMove(MoveCandidate);
			int32 NewScore = AlphaBetaMiniMax(MaxDepth - 1, BestScore, 69420, false);

			if (NewScore > BestScore)
			{
				BestScore = NewScore;
				BestMove = MoveCandidate;
			}
			GameMode->UndoLastMove();
		}

		GameMode->DoFinalMove(BestMove);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Move done!"));
		GameMode->UpdateChessboard();
		GameMode->TurnNextPlayer();
	}, 0.1, false);
}

void ACH_MinimaxPlayer::OnWin()
{
}

void ACH_MinimaxPlayer::OnLose()
{
}

uint32 ACH_MinimaxPlayer::EvaluateState() const
{
	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
	PieceColor OtherColor = OwnedColor == PieceColor::PWHITE ? PieceColor::PBLACK : PieceColor::PWHITE;
	uint32 OwnedScore = 0;
	uint32 OtherScore = 0;

	// WHITE
	for (uint32 i = 0; i < 8; i++)
	{
		for (uint32 j = 0; j < 8; j++)
		{
			AChessPiece* Piece = GameMode->GetChessPieceAt(FVector2D(i, j));
			if (Piece != nullptr && Piece->GetColor() == OwnedColor)
			{
				switch (Piece->GetType())
				{
				case PieceType::BISHOP:
				case PieceType::ROOK:
					OwnedScore += 2;
					break;
				case PieceType::KNIGHT:
					OwnedScore += 3;
					break;
				case PieceType::PAWN:
					OwnedScore += 1;
					break;
				case PieceType::QUEEN:
					OwnedScore += 5;
					break;
				/*
				case PieceType::KING:
					OwnedScore += GameMode->CalculateFullyLegalMoves(FVector2D(i, j)).Num();
					*/
				}
			}
			if (Piece != nullptr && Piece->GetColor() == OtherColor)
			{
				switch (Piece->GetType())
				{
				case PieceType::BISHOP:
				case PieceType::ROOK:
					OtherScore += 2;
					break;
				case PieceType::KNIGHT:
					OtherScore += 3;
					break;
				case PieceType::PAWN:
					OtherScore += 1;
					break;
				case PieceType::QUEEN:
					OtherScore += 5;
					break;
				/*
				case PieceType::KING:
					OtherScore += GameMode->CalculateFullyLegalMoves(FVector2D(i, j)).Num();
					*/
				}
			}
		}
	}
	return OwnedScore - OtherScore;
}

int32 ACH_MinimaxPlayer::AlphaBetaMiniMax(uint32 Depth, int32 Alpha, int32 Beta, bool bIsMax)
{
	PieceColor OtherColor = OwnedColor == PieceColor::PWHITE ? PieceColor::PBLACK : PieceColor::PWHITE;
	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());

	if (Depth <= 0) return EvaluateState();
	if (GameMode->CheckCheckmate(OwnedColor)) return -1000;
	if (GameMode->CheckCheckmate(OtherColor)) return 1000;
	if (bIsMax)
	{
		for (FChessMove MoveCandidate : GameMode->CalculateAllFullyLegalMoves(OwnedColor))
		{
			GameMode->DoMove(MoveCandidate);
			Alpha = FMath::Max(Alpha, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, false));
			GameMode->UndoLastMove();
			if (Beta <= Alpha) return Alpha;
		}
		return Alpha;
	}
	else
	{
		for (FChessMove MoveCandidate : GameMode->CalculateAllFullyLegalMoves(OtherColor))
		{
			GameMode->DoMove(MoveCandidate);
			Beta = FMath::Min(Beta, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, true));
			GameMode->UndoLastMove();
			if (Beta <= Alpha) return Beta;
		}
		return Beta;
	}
}

