// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_GameMode.h"
#include "ChessPiece.h"
#include "CH_PlayerController.h"
#include "CH_HumanPlayer.h"
#include "CH_RandomPlayer.h"
#include "EngineUtils.h"

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
		4 * Chessboard->GetTileSize(),
		4 * Chessboard->GetTileSize() + XOffset,
		700.0f
	};
	ACH_HumanPlayer* HumanPlayer = Cast<ACH_HumanPlayer>(*TActorIterator<ACH_HumanPlayer>(GetWorld()));
	HumanPlayer->SetActorLocationAndRotation(CameraPosition, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	// TODO: make random player
	// Random Player
	ACH_RandomPlayer* AI = GetWorld()->SpawnActor<ACH_RandomPlayer>(FVector(), FRotator());

	// TODO: make ai player
	// TODO: select player from main menu
	// MinMax Player
	//auto* AI = GetWorld()->SpawnActor<ATTT_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI);
	
	Players[CurrentPlayer]->OnTurn();
}

ACH_GameMode::ACH_GameMode()
{
	PlayerControllerClass = ACH_PlayerController::StaticClass();
	DefaultPawnClass = ACH_HumanPlayer::StaticClass();
}
double ACH_GameMode::GetTileSize() const
{
	return Chessboard->GetTileSize();
}
/*
double ACH_GameMode::GetChessboard->GetTileSize()() const { return Chessboard->GetTileSize(); }
*/
void ACH_GameMode::PrepareChessboard()
{
	// Delete all existing ChessPieces
	Chessboard->DeleteAllChessPieces();

	// Put ChessPieces on the Chessboard
	// Pawns
	for (uint32 i = 0; i < 8; i++) {
		Chessboard->AddNewChessPiece(PieceType::PAWN, PieceColor::PBLACK, FVector2D(i, 6));
		Chessboard->AddNewChessPiece(PieceType::PAWN, PieceColor::PWHITE, FVector2D(i, 1));
	}

	// Rooks
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(0, 7));
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(7, 7));
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(0, 0));
	Chessboard->AddNewChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(7, 0));

	// Bishops
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(1, 7));
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(6, 7));
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(1, 0));
	Chessboard->AddNewChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(6, 0));

	// Knights
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(2, 7));
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(5, 7));
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(2, 0));
	Chessboard->AddNewChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(5, 0));

	// Queens
	Chessboard->AddNewChessPiece(PieceType::QUEEN, PieceColor::PBLACK, FVector2D(3, 7));
	Chessboard->AddNewChessPiece(PieceType::QUEEN, PieceColor::PWHITE, FVector2D(3, 0));

	// Kings
	Chessboard->AddNewChessPiece(PieceType::KING, PieceColor::PWHITE, FVector2D(4, 0));
	Chessboard->AddNewChessPiece(PieceType::KING, PieceColor::PBLACK, FVector2D(4, 7));
}

void ACH_GameMode::TurnNextPlayer()
{
	// Find the next player
	CurrentPlayer = (CurrentPlayer + 1) % 2;

	// If the player is Checkmated end the game
	PieceColor CurrentPlayerColor = CurrentPlayer == 0 ? PWHITE : PBLACK;
	if (CheckCheckmate(CurrentPlayerColor))
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
		Move.Type == MoveType::CAPTURE);
	MovesHistory.Push(Move);
}

bool ACH_GameMode::DoesMoveUncheck(FChessMove Move)
{
	if (Chessboard->GetChessPieceAt(Move.StartPosition) == nullptr)
		return false;

	// Try the move without moving any AChessPiece Actors
	// Keep track of the captured ChessPiece so it can be
	// restored after the check is finished
	AChessPiece* CapturedChessPiece = ChessPieceMap.FindAndRemoveChecked(Move.EndPosition);
	AChessPiece* MovedChessPiece = ChessPieceMap.FindAndRemoveChecked(Move.StartPosition);
	ChessPieceMap.Add(Move.StartPosition, MovedChessPiece);

	// Check for a Check state
	bool bResult = CheckCheck(MovedChessPiece->GetColor());

	// Undo the move
	ChessPieceMap.Remove(Move.EndPosition);
	if (CapturedChessPiece != nullptr) ChessPieceMap.Add(Move.EndPosition, CapturedChessPiece);
	ChessPieceMap.Add(Move.StartPosition, MovedChessPiece);

	return bResult;
}

bool ACH_GameMode::UndoLastMove()
{
	FChessMove LastMove = MovesHistory.Pop();
	bool bSuccess = true;

	switch (LastMove.Type)
	{
	case MoveType::MOVE:
		bSuccess = Chessboard->MoveChessPiece(
			LastMove.EndPosition,
			LastMove.StartPosition,
			// undoing a move isn't expected to overwrite
			false
		);
		break;
	case MoveType::CAPTURE:
		// Move the moved ChessPiece back
		bSuccess = Chessboard->MoveChessPiece(
			LastMove.EndPosition,
			LastMove.StartPosition,
			// undoing a move isn't expected to overwrite
			false
		);

		// Restore the captured ChessPiece
		if (bSuccess)
		{
			AChessPiece* RestoredPiece = Chessboard->GetLastCapturedChessPiece();
			if (RestoredPiece == nullptr) return false;
			Chessboard->AddNewChessPiece(
				RestoredPiece->GetType(),
				RestoredPiece->GetColor(),
				LastMove.CapturePosition
			);
		}

		break;
		// TODO: Handle PROMOTE and CASTLE moves
	}

	// Restore MoveHistory if something went wrong
	if (!bSuccess)
	{
		MovesHistory.Push(LastMove);
		return false;
	}
	return true;
}

TArray<FChessMove> ACH_GameMode::CalculateLegalMoves(FVector2D Position)
{
	RemoveAllIndicators();

	// Get the ChessPiece and check that it's valid
	// AChessPiece* Piece = *ChessPieceMap.Find(Position);
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
		uint32 YOffset = 1 - 2 * ColorIndex;

		// A Pawn can move forwards by 2 GetTileSize()s if it's in the start position
		// The start position is y=1 for WHITE Pawns and y=6 for BLACK Pawns
		uint32 MaxLength = Position[1] == 5 * ColorIndex + 1 ? 2 : 1;

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
		for (int32 xOffset = -1; xOffset <= 1; xOffset += 2)
		{
			FVector2D TargetPos = Position + FVector2D(xOffset, YOffset);
			AChessPiece const* Target = Chessboard->GetChessPieceAt(TargetPos);
			if (Target != nullptr && Target->GetColor() != Piece->GetColor())
			{
				Moves.Add(FChessMove(
					Position,
					TargetPos,
					TargetPos,
					MoveType::CAPTURE
				));
			}
		}

		// A PAWN can be promoted to another type when it reaches the opposite
		// end of the ChessBoard
		if (Position[1] == 7 * (1 - ColorIndex))
		{
			Moves.Add(FChessMove(
				Position,
				Position,
				FVector2D(-6, -9),
				MoveType::PROMOTE
			));
		}
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

TArray<FChessMove> ACH_GameMode::CalculateAllMoves(PieceColor Color)
{
	TArray<FChessMove> Moves;
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);
	for (FVector2D Position : Positions)
	{
		if (Chessboard->GetChessPieceAt(Position)->GetColor() == Color)
		{
			Moves.Append(CalculateLegalMoves(Position));
		}
	}

	return Moves;
}

bool ACH_GameMode::CheckCheck(PieceColor Color)
{
	PieceColor OtherColor = Color == PWHITE ? PBLACK : PWHITE;
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);
	for (FVector2D Position : Positions)
	{
		if (Chessboard->GetChessPieceAt(Position)->GetColor() == OtherColor)
		{
			for (FChessMove Move : CalculateLegalMoves(Position))
			{
				AChessPiece* Piece = Chessboard->GetChessPieceAt(Move.EndPosition);
				if (Piece != nullptr && Piece->GetType() == KING)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ACH_GameMode::CheckCheckmate(PieceColor Color)
{
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);
	for (FVector2D Position : Positions)
	{
		if (Chessboard->GetChessPieceAt(Position)->GetColor() == Color)
		{
			for (FChessMove Move : CalculateLegalMoves(Position))
			{
				if (DoesMoveUncheck(Move)) return true;
			}
		}
	}
	return true;
}
void ACH_GameMode::UpdateChessboard()
{
	Chessboard->UpdateChessboard();
}
/*
AChessPiece const* ACH_GameMode::GetConstChessPieceAt(FVector2D Position) const
{
	AChessPiece* const* result = ChessPieceMap.Find(Position);
	return result != nullptr ? *result : nullptr;
}
*/
/*
void ACH_GameMode::RemoveAllChessPieces()
{
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);

	for (FVector2D Position : Positions) {
		Chessboard->GetChessPieceAt(Position)->Destroy();
	}
}
*/
AIndicator const* ACH_GameMode::GetIndicatorForEndPos(FVector2D EndPos)
{
	for (AIndicator* Indicator : Indicators)
	{
		if (Indicator->GetMove().EndPosition == EndPos)
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
			UE_LOG(LogTemp, Error, TEXT("Could not remove indicator at (%f, %f)"), x->GetMove().EndPosition[0], x->GetMove().EndPosition[1]);
	}
	Indicators.Empty();
}
TSubclassOf<AIndicator> ACH_GameMode::IndicatorTypeToClass(MoveType Type) const
{
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
			FVector2D(-6,  -9),
			MoveType::MOVE
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
				TargetPosition,
				MoveType::CAPTURE
			));
		return;
	}
}
AChessPiece* ACH_GameMode::GetChessPieceAt(FVector2D Position)
{
	return Chessboard->GetChessPieceAt(Position);
}
/*
AChessPiece* ACH_GameMode::Chessboard->GetChessPieceAt(FVector2D Position)
{
	if (ChessPieceMap.Contains(Position))
		return ChessPieceMap[Position];
	return nullptr;
}
*/
/*
bool ACH_GameMode::RemoveChessPiece(FVector2D Position)
{
	AChessPiece* x = Chessboard->GetChessPieceAt(Position);
	if (x == nullptr) return true;
	return x->Destroy();
}
*/
/*
AChessPiece* ACH_GameMode::Chessboard->AddNewChessPiece(PieceType Type, PieceColor Color, FVector2D Position)
{
	if (ChessPieceMap.Contains(Position)) return nullptr;

	// Spawn ChessPiece in the scene
	TSubclassOf<AChessPiece> PieceClass = ColorTypeToClass(Color, Type);
	if (PieceClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing ChessPiece class"));
	}
	AChessPiece* ChessPiece = GetWorld()->SpawnActor<AChessPiece>(
		PieceClass,
		FVector(Position[1] + 0.5, Position[0] + 0.5, 0.001) * Chessboard->GetTileSize(),
		FRotator::ZeroRotator
	);

	// Set up ChessPiece
	ChessPiece->Setup(Type, Color);

	// Add ChessPiece to ChessPieceMap
	ChessPieceMap.Add(Position, ChessPiece);

	return ChessPiece;
}
*/
/*
bool ACH_GameMode::MoveChessPiece(FVector2D OldPosition, FVector2D NewPosition)
{
	if (Chessboard->GetChessPieceAt(OldPosition) == nullptr) return false;

	// Remove any already existing ChessPieces in the NewPosition
	AChessPiece* TempChessPiece = Chessboard->GetChessPieceAt(NewPosition);
	if (TempChessPiece != nullptr)
	{
		ChessPieceMap.FindAndRemoveChecked(NewPosition);
		TempChessPiece->Destroy();
		// TODO: add to captured pieces
	}

	// Change the record inside the ChessPieceMap
	TempChessPiece = Chessboard->GetChessPieceAt(OldPosition);
	ChessPieceMap.FindAndRemoveChecked(OldPosition);
	
	// ChessPieceMap.Add(NewPosition, TempChessPiece);

	// // Move Actor in the new position of the scene
	// // TODO: adjust position vector element order
	// FHitResult temp; // needed by K2_SetActorLocation, will get ignored
	// TempChessPiece->K2_SetActorLocation(
	// 	FVector(NewPosition[0], NewPosition[1], 0.001) * Chessboard->GetTileSize(),
	//	false,
	//	temp,
	//	true
	// );
	
	PieceType Type = TempChessPiece->GetType();
	PieceColor Color = TempChessPiece->GetColor();
	TempChessPiece->Destroy();

	Chessboard->AddNewChessPiece(Type, Color, NewPosition);
	return true;
}
*/
/*
AIndicator* ACH_GameMode::PutIndicator(FVector2D StartPosition, FVector2D EndPosition)
{
	// Detect the type of Indicator based on the StartPosition, EndPosition,
	// and the Chessboard's state
	UClass* IndicatorClass;
	MoveType Type;

	// PROMOTION === StartPosition and EndPosition are the same
	if (StartPosition == EndPosition)
	{
		IndicatorClass = Cast<UClass>(PromoteIndicatorClass);
		Type = MoveType::PROMOTE;
	}
	else
	{
		// MOVE === EndPosition is empty
		if (Chessboard->GetChessPieceAt(EndPosition) == nullptr)
		{
			IndicatorClass = Cast<UClass>(MoveIndicatorClass);
			Type = MoveType::MOVE;
		}
		else
		// CAPTURE === EndPosition is not empty
		{
			IndicatorClass = Cast<UClass>(CaptureIndicatorClass);
			Type = MoveType::CAPTURE;
		}
	}

	// Once the type of Indicator is Determined, create the Indicator
	if (IndicatorClass == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Missing Indicator class"));

	AIndicator* Indicator = GetWorld()->SpawnActor<AIndicator>(
		IndicatorClass,
		FVector(EndPosition[1] + 0.5, EndPosition[0] + 0.5, 0.002) * Chessboard->GetTileSize(),
		FRotator::ZeroRotator
	);

	// Set up Indicator
	Indicator->Setup(Type, StartPosition, EndPosition);
	return Indicator;
}
*/


AIndicator* ACH_GameMode::AddNewIndicator(FChessMove Move)
{
	// Determine the TSubclassOf for the MoveType
	TSubclassOf<AIndicator> IndicatorClass = IndicatorTypeToClass(Move.Type);
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
	Indicator->BindToMove(Move);
	Indicators.Add(Indicator);
	return Indicator;
}

