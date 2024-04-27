// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_GameMode.h"
#include "ChessPiece.h"
#include "CH_PlayerController.h"
#include "CH_HumanPlayer.h"
#include "CH_RandomPlayer.h"
#include "CH_MinimaxPlayer.h"
#include "CH_WidgetManager.h"
#include "EngineUtils.h"


ACH_GameMode::ACH_GameMode()
{
	PlayerControllerClass = ACH_PlayerController::StaticClass();
	DefaultPawnClass = ACH_HumanPlayer::StaticClass();

	MovesSinceLastCapture = 0;
}

void ACH_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	if (ChessboardClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Chessboard class"));
		return;
	}

	Chessboard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	if (Chessboard == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Chessboard object"));
		return;
	}
	Chessboard->CreateChessboard();
	PrepareChessboard();

	// The Chessboard will be aligned to the left, so there will be
	// an offset along the x-axis
	double XOffset = 290.0;

	// Init players and add to list
	FVector CameraPosition = {
		4 * Chessboard->GetTileSize() + XOffset,
		4 * Chessboard->GetTileSize(),
		700.0f
	};
	ACH_HumanPlayer* HumanPlayer = Cast<ACH_HumanPlayer>(*TActorIterator<ACH_HumanPlayer>(GetWorld()));
	
	HumanPlayer->SetActorLocationAndRotation(
		CameraPosition,
		FRotator(-90.0, -90.0, 0.0)
	);

	// Human player = 0
	Players.Add(HumanPlayer);

	// Random Player
	// ACH_RandomPlayer* AI = GetWorld()->SpawnActor<ACH_RandomPlayer>(FVector(), FRotator());

	// TODO: select player from main menu
	// Minimax Player
	ACH_MinimaxPlayer* AI = GetWorld()->SpawnActor<ACH_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI);

	// Assign Colors to players
	Players[0]->OwnedColor = PieceColor::PWHITE;
	Players[1]->OwnedColor = PieceColor::PBLACK;
	
	// Create widget manager and bind delegates
	WidgetManager = GetWorld()->SpawnActor<ACH_WidgetManager>(WidgetManagerClass);
	HumanPlayer->SetWidgetManager(WidgetManager);

	WidgetManager->GetMovesHistory()->OnHistoryClickedDelegate.AddDynamic(this, &ACH_GameMode::OnHistoryClicked);
	Players[CurrentPlayer]->OnTurn();
}

double ACH_GameMode::GetTileSize() const
{
	return Chessboard->GetTileSize();
}

void ACH_GameMode::PrepareChessboard()
{
	// Delete all existing ChessPieces
	Chessboard->DeleteAllChessPieces();

	// Put ChessPieces on the Chessboard
	// Pawns
	for (uint32 i = 0; i < 8; i++) {
		Chessboard->AddNewChessPiece(PieceType::PAWN, PieceColor::PBLACK, FVector2D(i, 1));
		Chessboard->AddNewChessPiece(PieceType::PAWN, PieceColor::PWHITE, FVector2D(i, 6));
	}

	// Rooks
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(0, 0));
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(7, 0));
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(0, 7));
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(7, 7));

	// Bishops
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(1, 0));
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(6, 0));
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(1, 7));
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(6, 7));

	// Knights
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(2, 0));
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(5, 0));
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(2, 7));
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(5, 7));

	// Queens
	Chessboard->AddNewChessPiece(PieceType::QUEEN, PieceColor::PWHITE, FVector2D(3, 7));
	Chessboard->AddNewChessPiece(PieceType::QUEEN, PieceColor::PBLACK, FVector2D(3, 0));

	// Kings
	Chessboard->AddNewChessPiece(PieceType::KING, PieceColor::PWHITE, FVector2D(4, 7));
	Chessboard->AddNewChessPiece(PieceType::KING, PieceColor::PBLACK, FVector2D(4, 0));
}

void ACH_GameMode::TurnNextPlayer()
{
	// Find the next player
	CurrentPlayer = (CurrentPlayer + 1) % 2;

	// If the player is Checkmated end the game
	PieceColor NextPlayerColor = CurrentPlayer == 0 ? PWHITE : PBLACK;
	if (CheckCheckmate(NextPlayerColor))
	{
		Players[CurrentPlayer]->OnLose();
		Players[1 - CurrentPlayer]->OnWin();
	}
	// Otherwise let their turn begin
	else
	{
		Players[CurrentPlayer]->OnTurn();
	}
}

void ACH_GameMode::DoMove(FChessMove Move)
{
	Chessboard->MoveChessPiece(
		Move.StartPosition,
		Move.EndPosition,
		// CAPTURE is the only move type that is expected to overwrite a ChessPiece
		Move.bDoesCapture
	);

	if (Move.bDoesPromote)
	{
		AChessPiece* ChessPiece = GetChessPieceAt(Move.EndPosition);
		ChessPiece->Setup(Move.PromotionTarget, ChessPiece->GetColor());
	}
	MovesHistory.Push(Move);

	if (Move.bDoesCapture) MovesSinceLastCapture = 0;
	else MovesSinceLastCapture++;
}

void ACH_GameMode::DoFinalMove(FChessMove Move)
{
	WidgetManager->GetMovesHistory()->AddNewMove(
		FText::FromString(GenerateSANForMove(Move)),
		CurrentPlayer == 0 ? PWHITE : PBLACK
	);

	DoMove(Move);
}

FString ACH_GameMode::GenerateSANForMove(FChessMove Move)
{
	FString PieceLetter;
	AChessPiece* MovedChessPiece = GetChessPieceAt(Move.StartPosition);
	if (MovedChessPiece == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MovedChessPiece was NULL!"));
		return "error";
	}
	switch (MovedChessPiece->GetType())
	{
	case PieceType::BISHOP:
		PieceLetter = "B";
		break;
	case PieceType::KING:
		PieceLetter = "K";
		break;
	case PieceType::QUEEN:
		PieceLetter = "Q";
		break;
	case PieceType::KNIGHT:
		PieceLetter = "N";
		break;
	case PieceType::ROOK:
		PieceLetter = "R";
		break;
	case PieceType::PAWN:
		PieceLetter = "";
		break;
	}

	// Start position (only if strictly necessary)
	FString StartPositionString = "";
	PieceColor CurrentColor = GetChessPieceAt(Move.StartPosition)->GetColor();
	PieceColor OpponentColor = CurrentColor == PWHITE ? PBLACK : PWHITE;

	// Chessboard->UpdateChessboard();
	for (FVector2D Position : Chessboard->GetAllOwnedPositions(CurrentColor))
	{
		AChessPiece* TestChessPiece = GetChessPieceAt(Position);
		if (TestChessPiece->GetType() == MovedChessPiece->GetType())
		{
			if (TestChessPiece == MovedChessPiece) continue;
			for (FChessMove PossibleMove : CalculateFullyLegalMoves(Position))
			{
				if (PossibleMove.EndPosition == Move.EndPosition)
				{
					// AMBIGUITY DETECTED!
					// Always add the file if the two files are different
					if (PossibleMove.StartPosition[1] != Move.StartPosition[1])
					{
						StartPositionString += AChessboard::PositionToFileRank(Move.StartPosition).GetCharArray()[0];
					}
					// Otherwise add the rank (if the files are the same the ranks must be different)
					else
					{
						StartPositionString += AChessboard::PositionToFileRank(Move.StartPosition).GetCharArray()[1];
					}
					
				}
			}
		}
	}
	// Chessboard->UpdateChessboard();

	// Capture
	FString CaptureMark = Move.bDoesCapture ? "x" : "";

	// End position
	FString EndPositionString = AChessboard::PositionToFileRank(Move.EndPosition);

	// = Promotion
	FString Promotion = "";
	if (Move.bDoesPromote)
	{
		switch (Move.PromotionTarget)
		{
		case PieceType::BISHOP:
			Promotion = "=B";
			break;
		case PieceType::KNIGHT:
			Promotion = "=N";
			break;
		case PieceType::QUEEN:
			Promotion = "=Q";
			break;
		case PieceType::ROOK:
			Promotion = "=R";
			break;
		}
	}

	// Check
	DoMove(Move);
	FString CheckState = "";
	if (CheckCheck(OpponentColor))
	{
		CheckState = "+";
	}
	if (CheckCheckmate(OpponentColor))
	{
		CheckState = "#";
	}
	UndoLastMove();

	return PieceLetter + StartPositionString + CaptureMark + EndPositionString + Promotion + CheckState;
}

bool ACH_GameMode::DoesMoveUncheck(FChessMove Move)
{
	AChessPiece* x = Chessboard->GetChessPieceAt(Move.StartPosition);

	if (x == nullptr)
		return false;

	DoMove(Move);
	bool bResult = CheckCheck(x->GetColor());
	UndoLastMove();

	return !bResult;
}

void ACH_GameMode::UndoLastMove()
{
	FChessMove LastMove = MovesHistory.Pop();

	// Move the ChessPiece back to where it was
	Chessboard->MoveChessPiece(
		LastMove.EndPosition,
		LastMove.StartPosition,
		// undoing a move isn't expected to overwrite
		false
	);

	// Restore any captured ChessPieces
	if (LastMove.bDoesCapture)
	{
		Chessboard->RestoreLastCapturedChessPiece(LastMove.CapturePosition);
	}

	// If the last move did a promotion replace the ChessPiece
	// at the EndPosition with a PAWN
	if (LastMove.bDoesPromote)
	{
		AChessPiece* ChessPiece = GetChessPieceAt(LastMove.StartPosition);
		ChessPiece->Setup(PieceType::PAWN, ChessPiece->GetColor());
	}
}

TArray<FChessMove> ACH_GameMode::CalculatePseudoLegalMoves(FVector2D Position)
{
	// Get the ChessPiece and check that it's valid
	AChessPiece const* Piece = Chessboard->GetChessPieceAt(Position);
	if (Piece == nullptr) return TArray<FChessMove>();
	if (Piece->GetColor() == PieceColor::PCNONE) return TArray<FChessMove>();
	if (Piece->GetType() == PieceType::PTNONE) return TArray<FChessMove>();

	// This will contain all legal moves stored FVector2D
	// objects representing the end position of the move
	TArray<FChessMove> Moves;

	// Pawn is handled differently from all other pieces:
	// - it can only move in one direction
	// - its capture pattern is different from its movement
	//   pattern
	// - it can be promoted
	// - TODO: can do an "en-passant"
	if (Piece->GetType() == PieceType::PAWN)
	{
		// Since a Pawn can only move forwards, its possible moves
		// will depend on its color. ColorIndex = 0 represents a WHITE
		// Pawn and ColorIndex = 1 represents a BLACK Pawn. This variable
		// parametrizes values that would otherwise need if-statements to
		// be determined.
		uint32 ColorIndex = Piece->GetColor() == PieceColor::PWHITE ? 0 : 1;

		// The direction in which Pawns can move is always along +y
		// (YOffset = 1) for WHITE pawns and -y (YOffset = -1) for BLACK Pawns
		int32 YOffset = 2 * ColorIndex - 1;

		// A Pawn can move forwards by 2 Tiles if it's in the start position
		// The start position is y=6 for WHITE Pawns and y=1 for BLACK Pawns
		uint32 MaxLength = Position[1] == 6 - 5 * ColorIndex ? 2 : 1;

		ExploreDirection(
			Position,
			Position,
			FVector2D(0, YOffset),
			MaxLength,
			false,
			&Moves,
			Piece->GetColor()
		);

		// Capture can't be added using ExploreDirection, so it's added
		// manually instead
		for (int32 XOffset = -1; XOffset <= 1; XOffset += 2)
		{
			FVector2D TargetPos = Position + FVector2D(XOffset, YOffset);
			AChessPiece const* Target = Chessboard->GetChessPieceAt(TargetPos);
			if (Target != nullptr && Target->GetColor() != Piece->GetColor())
			{
				Moves.Add(FChessMove(
					Position,
					TargetPos,
					true,
					TargetPos,
					false,
					PieceType::PTNONE
				));
			}
		}

		// If the PAWN is at the end of the Chessboard it can promote
		for (int i = 0; i < Moves.Num(); i++)
		{
			if (Moves[i].EndPosition[1] == 7 * ColorIndex)
			{
				Moves[i].bDoesPromote = true;
			}
		}

		return Moves;
	}

	uint32 MaxLength = 8;
	TArray<FVector2D> Directions;

	// Pieces that can move diagonally
	if (Piece->GetType() == PieceType::BISHOP
		|| Piece->GetType() == PieceType::QUEEN
		|| Piece->GetType() == PieceType::KING)
	{
		Directions.Add(FVector2D(1, 1));
		Directions.Add(FVector2D(1, -1));
		Directions.Add(FVector2D(-1, 1));
		Directions.Add(FVector2D(-1, -1));
	}

	// Pieces that can move horizontally and vertically
	if (Piece->GetType() == PieceType::ROOK
		|| Piece->GetType() == PieceType::QUEEN
		|| Piece->GetType() == PieceType::KING)
	{
		Directions.Add(FVector2D(-1, 0));
		Directions.Add(FVector2D(0, -1));
		Directions.Add(FVector2D(1, 0));
		Directions.Add(FVector2D(0, 1));
	}

	// Knight
	if (Piece->GetType() == PieceType::KNIGHT)
	{
		Directions.Add(FVector2D(2, 1));
		Directions.Add(FVector2D(-2, 1));
		Directions.Add(FVector2D(2, -1));
		Directions.Add(FVector2D(-2, -1));
		Directions.Add(FVector2D(1, 2));
		Directions.Add(FVector2D(-1, 2));
		Directions.Add(FVector2D(1, -2));
		Directions.Add(FVector2D(-1, -2));
	}

	// Pieces that can only do 1 step in a given
	// direction
	if (Piece->GetType() == PieceType::KNIGHT
		|| Piece->GetType() == PieceType::KING)
	{
		MaxLength = 1;
	}
	
	// Generate moves for every direction
	for (FVector2D Direction : Directions) {
		ExploreDirection(
			Position,
			Position,
			Direction,
			MaxLength,
			true,
			&Moves,
			Piece->GetColor()
		);
	}
	return Moves;
}

TArray<FChessMove> ACH_GameMode::CalculateFullyLegalMoves(FVector2D Position)
{
	TArray<FChessMove> Result;
	for (FChessMove Move : CalculatePseudoLegalMoves(Position))
	{
		if (DoesMoveUncheck(Move))
		{
			Result.Add(Move);
		}
	}

	return Result;
}

TArray<FChessMove> ACH_GameMode::CalculateAllPseudoLegalMoves(PieceColor Color)
{
	TArray<FChessMove> Moves;

	for (FVector2D Position : Chessboard->GetAllOwnedPositions(Color))
	{
		Moves.Append(CalculatePseudoLegalMoves(Position));
	}

	return Moves;
}

TArray<FChessMove> ACH_GameMode::CalculateAllFullyLegalMoves(PieceColor Color)
{
	TArray<FChessMove> Moves;

	for (FVector2D Position : Chessboard->GetAllOwnedPositions(Color))
	{
		Moves.Append(CalculateFullyLegalMoves(Position));
	}

	return Moves;
}

TArray<FChessMove> ACH_GameMode::ExpandPromotionMoves(TArray<FChessMove> Moves)
{
	TArray<FChessMove> Result;

	for (FChessMove Move : Moves)
	{
		if (Move.bDoesPromote)
		{
			Move.PromotionTarget = PieceType::KNIGHT;
			Result.Add(Move);
			Move.PromotionTarget = PieceType::QUEEN;
			Result.Add(Move);
		}
		else
		{
			Result.Add(Move);
		}
	}

	return Result;
}

bool ACH_GameMode::CheckCheck(PieceColor Color)
{
	PieceColor OtherColor = Color == PWHITE ? PBLACK : PWHITE;
	for (FVector2D Position : Chessboard->GetAllOwnedPositions(OtherColor))
	{
		for (FChessMove Move : CalculatePseudoLegalMoves(Position))
		{
			AChessPiece* Piece = Chessboard->GetChessPieceAt(Move.CapturePosition);
			if (Piece != nullptr && Piece->GetType() == KING)
			{
				UE_LOG(LogTemp, Error, TEXT("Check! move from (%d, %d) to (%d, %d)"), Move.StartPosition[0], Move.StartPosition[1], Move.EndPosition[0], Move.EndPosition[1]);
				return true;
			}
		}
	}
	return false;
}

bool ACH_GameMode::CheckCheckmate(PieceColor Color)
{
	for (FVector2D Position : Chessboard->GetAllOwnedPositions(Color))
	{
		for (FChessMove Move : CalculateFullyLegalMoves(Position))
		{
			if (DoesMoveUncheck(Move)) return false;
		}
	}

	return true;
}

bool ACH_GameMode::CheckDraw()
{
	return (MovesSinceLastCapture >= 75);
}

void ACH_GameMode::UpdateChessboard()
{
	Chessboard->UpdateChessboard();
}

AIndicator const* ACH_GameMode::GetIndicatorForEndPos(FVector2D EndPos)
{
	for (AIndicator* Indicator : Indicators)
	{
		if (Indicator->Move.EndPosition == EndPos)
			return Indicator;
	}
	return nullptr;
}

void ACH_GameMode::RemoveAllIndicators()
{
	bool bDestroyed;
	for (AIndicator* x : Indicators)
	{
		bDestroyed = x->Destroy();
		if (!bDestroyed)
			UE_LOG(LogTemp, Error, TEXT("Could not remove indicator at (%f, %f)"), x->Move.EndPosition[0], x->Move.EndPosition[1]);
	}
	Indicators.Empty();
}
TSubclassOf<AIndicator> ACH_GameMode::IndicatorTypeToClass(FChessMove Move) const
{
	if (Move.bDoesPromote)
		return PromoteIndicatorClass;
	if (Move.bDoesCapture)
		return CaptureIndicatorClass;
	return MoveIndicatorClass;

	/*
	switch (Type) {
	case MoveType::CAPTURE:
		return CaptureIndicatorClass;
	case MoveType::MOVE:
		return MoveIndicatorClass;
	case MoveType::PROMOTE:
		return PromoteIndicatorClass;
	default:
		return nullptr;
	}
	*/
}

void ACH_GameMode::ExploreDirection(FVector2D Position, FVector2D RootPosition,
	FVector2D Direction, uint32 MaxLength, bool CanCapture, TArray<FChessMove>* Moves,
	PieceColor PlayerColor)
{
	FVector2D TargetPosition = Position + Direction;
	// Check if TargetPosition is out of bounds
	if (TargetPosition[0] < 0 || TargetPosition[0] > 7) return;
	if (TargetPosition[1] < 0 || TargetPosition[1] > 7) return;
	if (MaxLength == 0) return;

	// Check if TargetPosition is free
	AChessPiece const* TargetPiece = Chessboard->GetChessPieceAt(TargetPosition);
	if (TargetPiece == nullptr)
	{
		// CapturePosition is set to a negative vector because it's not used anyway
		// TODO: Define this value somewhere else as a const INVALID_POSITION
		Moves->Add(FChessMove(
			RootPosition,
			TargetPosition,
			false,
			FVector2D(-6, -9),
			false,
			PieceType::PTNONE
		));
		ExploreDirection(TargetPosition, RootPosition, Direction, MaxLength - 1,
			CanCapture, Moves, PlayerColor);
	}
	else {
		// If TargetPosition is occupied by a ChessPiece, it can be captured only if
		// it's owned by the opponent and the CanCapture flag is set to true
		if (CanCapture && TargetPiece->GetColor() != PlayerColor)
			Moves->Add(FChessMove(
				RootPosition,
				TargetPosition,
				true,
				TargetPosition,
				false,
				PieceType::PTNONE
			));
		return;
	}
}

void ACH_GameMode::OnHistoryClicked(uint32 MovesFromGameStart)
{
	// Reset state
	PrepareChessboard();
	RemoveAllIndicators();
	CurrentPlayer = 0;
	WidgetManager->GetMovesHistory()->Clear();

	// Do moves forwards
	for (uint32 i = 0; i < MovesFromGameStart; i++)
	{
		DoFinalMove(MovesHistory[i]);
		CurrentPlayer = (CurrentPlayer + 1) % 2;
	}
	// Update MovesHistory by removing the other ChessMoves
	int32 NewLength = MovesFromGameStart + 1;
	while (MovesHistory.Num() > NewLength)
	{
		MovesHistory.RemoveAt(NewLength);
	}

	UpdateChessboard();
	Players[CurrentPlayer]->OnTurn();
}

AChessPiece* ACH_GameMode::GetChessPieceAt(FVector2D Position)
{
	return Chessboard->GetChessPieceAt(Position);
}

AIndicator* ACH_GameMode::AddNewIndicator(FChessMove Move)
{
	// Determine the TSubclassOf for the MoveType
	TSubclassOf<AIndicator> IndicatorClass = IndicatorTypeToClass(Move);
	if (IndicatorClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Indicator class"));
		return nullptr;
	}
		

	// Spawn the Indicator
	AIndicator* Indicator = GetWorld()->SpawnActor<AIndicator>(
		IndicatorClass,
		Chessboard->GridToRealPosition(Move.EndPosition, Chessboard->ZIndexType::INDICATOR_ZINDEX),
		FRotator::ZeroRotator
	);

	// Set up Indicator
	Indicator->Move = Move;
	Indicators.Add(Indicator);
	return Indicator;
}

