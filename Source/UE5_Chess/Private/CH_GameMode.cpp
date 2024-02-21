// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_GameMode.h"
#include "ChessPiece.h"


ACH_GameMode::ACH_GameMode()
	: TileSize{1.0}, ChessPieceSize{0.9}
{
	// TODO:
	// PlayerControllerClass = ATTT_PlayerController::StaticClass();
	// DefaultPawnClass = ATTT_HumanPlayer::StaticClass();
}

void ACH_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	// TODO: Init players and add to list

	if (ChessboardClass == nullptr) 
	{
		MissingClass();
		
	}

	Chessboard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	Chessboard->CreateChessboard(TileSize);
	
	Players[CurrentPlayer]->OnTurn();
}

void ACH_GameMode::TurnNextPlayer()
{
	CurrentPlayer = (CurrentPlayer + 1) % 2;
	Players[CurrentPlayer]->OnTurn();
}

void ACH_GameMode::ExploreDirection(FVector2D Position, FVector2D Direction,
	uint32 MaxLength, bool CanCapture, TArray<FVector2D>* Moves, PieceColor PlayerColor)
{
	FVector2D TargetPosition = Position + Direction;
	// Check if TargetPosition is out of bounds
	if (TargetPosition[0] < 0 || TargetPosition[0] > 7) return;
	if (TargetPosition[1] < 0 || TargetPosition[1] > 7) return;

	// Check if TargetPosition is free
	AChessPiece* TargetPiece = *ChessPieceMap.Find(TargetPosition);
	if (TargetPiece == nullptr)
	{
		Moves->Add(TargetPosition);
		ExploreDirection(TargetPosition, Direction, MaxLength - 1, CanCapture, Moves, PlayerColor);
	}
	else {
		// If TargetPosition is occupied by a ChessPiece, it can be captured only if
		// it's owned by the opponent and the CanCapture flag is set to true
		if (CanCapture && TargetPiece->GetColor() != PlayerColor)
			Moves->Add(TargetPosition);
		return;
	}
}

AIndicator* ACH_GameMode::SpawnIndicator(FVector2D StartPosition, FVector2D EndPosition)
{
	UClass* IndicatorClass;
	MoveType Type;
	if (StartPosition == EndPosition)
	{
		IndicatorClass = Cast<UClass>(PromoteIndicatorClass);
		Type = MoveType::PROMOTE;
	}
	else
	{
		if (*ChessPieceMap.Find(StartPosition) == nullptr)
		{
			IndicatorClass = Cast<UClass>(MoveIndicatorClass);
			Type = MoveType::MOVE;
		}
		else
		{
			IndicatorClass = Cast<UClass>(CaptureIndicatorClass);
			Type = MoveType::CAPTURE;
		}
	}

	if (IndicatorClass == nullptr)
		MissingClass();

	AIndicator* Indicator = GetWorld()->SpawnActor<AIndicator>(
		IndicatorClass,
		FVector(EndPosition[0], EndPosition[1], 0.3) * TileSize,
		FRotator::ZeroRotator
	);

	// Set up ChessPiece
	Indicator->Setup(Type, StartPosition, EndPosition);

	// Add ChessPiece to ChessPieceMap
	Indicator->SetActorScale3D(FVector(1.0, 1.0, 0.1));
	return Indicator;
}

void ACH_GameMode::ShowLegalMoves(FVector2D Position)
{
	// Get the ChessPiece and check that it's valid
	AChessPiece* Piece = *ChessPieceMap.Find(Position);
	if (Piece == nullptr) return;
	if (Piece->GetColor() == PieceColor::PCNONE) return;
	if (Piece->GetType() == PieceType::PTNONE) return;

	// This will contain all legal moves stored FVector2D
	// objects representing the end position of the move
	TArray<FVector2D> Moves;

	// Common exploration directions used by multiple
	// ChessPieces
	TArray<FVector2D> AxisDirections = {
			FVector2D(1, 0), FVector2D(-1, 0),
			FVector2D(0, 1), FVector2D(0, -1)
	};
	TArray<FVector2D> DiagDirections = {
			FVector2D(1, 1), FVector2D(-1, -1),
			FVector2D(-1, 1), FVector2D(1, -1)
	};
	TArray<FVector2D> AllDirections = {
		FVector2D(1, 0), FVector2D(-1, 0),
		FVector2D(0, 1), FVector2D(0, -1),
		FVector2D(1, 1), FVector2D(-1, -1),
		FVector2D(-1, 1), FVector2D(1, -1)
	};
	switch (Piece->GetType()) {
	case PieceType::PAWN:
		// Since a Pawn can only move forwards, its possible moves
		// depend on its color! In order to avoid writing two different
		// routines for each color or using many if-statements, this
		// variable is needed.
		// The value of the variable represents the color:
		// WHITE -> 0, BLACK, 1
		uint32 ColorIndex = Piece->GetColor() == PieceColor::PWHITE ? 0 : 1;

		// If the Pawn is at the start position, it can move by 2 Tiles
		uint32 MaxLength = Position[1] == 6 - 5 * ColorIndex ? 2 : 1;
		ExploreDirection(
			Position,
			FVector2D(0, 2 * ColorIndex + 1),
			MaxLength,
			false,
			&Moves,
			Piece->GetColor()
		);

		// Capture
		for (uint32 i = -1; i <= 1; i += 2)
		{
			ExploreDirection(
				Position,
				FVector2D(i, 2 * ColorIndex + 1),
				1,
				true,
				&Moves,
				Piece->GetColor()
			);
		}

		break;

	case PieceType::ROOK:
		// Rook moves horizontally and vertically (AxisDirections)
		for (FVector2D Direction : AxisDirections)
		{
			ExploreDirection(
				Position,
				Direction,
				8,
				true,
				&Moves,
				Piece->GetColor()
			);
		}

		break;

	case PieceType::BISHOP:
		// Bishop moves diagonally (DiagDirections)
		for (FVector2D Direction : DiagDirections)
		{
			ExploreDirection(
				Position,
				Direction,
				8,
				true,
				&Moves,
				Piece->GetColor()
			);
		}

		break;

	case PieceType::QUEEN:
		// Queen moves in all directions
		for (FVector2D Direction : AllDirections)
		{
			ExploreDirection(
				Position,
				Direction,
				8,
				true,
				&Moves,
				Piece->GetColor()
			);
		}

		break;

		// TODO: Check will limit the possible moves
	case PieceType::KING:
		// King moves in all directions by 1
		for (FVector2D Direction : AllDirections)
		{
			ExploreDirection(
				Position,
				Direction,
				1,
				true,
				&Moves,
				Piece->GetColor()
			);
		}

	case PieceType::KNIGHT:
		// Knight moves in L-shaped directions (by 1)
		TArray<FVector2D> KnightDirections = {
			FVector2D(1, 2), FVector2D(-1, -2),
			FVector2D(1, -2), FVector2D(-1, 2),
			FVector2D(2, 1), FVector2D(-2, -1),
			FVector2D(2, -1), FVector2D(-2, 1)
		};

		for (FVector2D Direction : KnightDirections)
		{
			ExploreDirection(
				Position,
				Direction,
				1,
				true,
				&Moves,
				Piece->GetColor()
			);
		}

		break;
	}
	
	for (FVector2D EndPosition : Moves)
		SpawnIndicator(Position, EndPosition);
}

void ACH_GameMode::PrepareChessboard()
{
	// Delete all existing ChessPieces
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);
	for (FVector2D Position : Positions) {
		RemoveChessPiece(Position);
	}

	// Put ChessPieces on the Chessboard
	// Pawns
	for (uint32 i = 0; i < 8; i++) {
		PutChessPiece(PieceType::PAWN, PieceColor::PBLACK, FVector2D(i, 1));
		PutChessPiece(PieceType::PAWN, PieceColor::PWHITE, FVector2D(i, 6));
	}

	// Rooks
	PutChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(0, 0));
	PutChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(7, 0));
	PutChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(0, 7));
	PutChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(7, 7));

	// Bishops
	PutChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(1, 0));
	PutChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(6, 0));
	PutChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(1, 7));
	PutChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(6, 7));

	// Knights
	PutChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(2, 0));
	PutChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(5, 0));
	PutChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(2, 7));
	PutChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(5, 7));

	// Queens
	PutChessPiece(PieceType::QUEEN, PieceColor::PBLACK, FVector2D(3, 0));
	PutChessPiece(PieceType::QUEEN, PieceColor::PWHITE, FVector2D(3, 7));

	// Kings
	PutChessPiece(PieceType::QUEEN, PieceColor::PWHITE, FVector2D(4, 7));
	PutChessPiece(PieceType::QUEEN, PieceColor::PBLACK, FVector2D(4, 0));
}

bool ACH_GameMode::RemoveChessPiece(FVector2D Position)
{
	if (!ChessPieceMap.Contains(Position))
		return true;

	AChessPiece* x = ChessPieceMap[Position];
	ChessPieceMap.Remove(Position);
	return x->Destroy();
}

AChessPiece* ACH_GameMode::PutChessPiece(PieceType Type, PieceColor Color, FVector2D Position)
{
	if (ChessPieceMap.Contains(Position)) return nullptr;

	// Spawn ChessPiece in the scene
	UClass* PieceClass = Cast<UClass>(ColorTypeToClass(Color, Type));
	if (PieceClass == nullptr)
	{
		MissingClass();
	}
	AChessPiece* ChessPiece = GetWorld()->SpawnActor<AChessPiece>(
		PieceClass,
		FVector(Position[0], Position[1], 0) * ChessPieceSize,
		FRotator::ZeroRotator
	);

	// Set up ChessPiece
	ChessPiece->Setup(Type, Color);

	// Add ChessPiece to ChessPieceMap
	ChessPiece->SetActorScale3D(FVector(1.0, 1.0, 0.1));
	ChessPieceMap.Add(Position, ChessPiece);
}

bool ACH_GameMode::MoveChessPiece(FVector2D OldPosition, FVector2D NewPosition)
{
	if (!ChessPieceMap.Contains(OldPosition)) return true;
	if (ChessPieceMap.Contains(NewPosition)) return false;

	// Update entry from ChessPieceMap
	ChessPieceMap[NewPosition] = ChessPieceMap[OldPosition];
	ChessPieceMap.Remove(OldPosition);

	// Move Actor in the new position of the scene
	FHitResult temp; // needed by K2_SetActorLocation, will get ignored
	ChessPieceMap[NewPosition]->K2_SetActorLocation(
		FVector(NewPosition[0], NewPosition[1], 0.1) * TileSize,
		false,
		temp,
		true
	);
	return true;
}

TSubclassOf<AChessPiece>* ACH_GameMode::ColorTypeToClass(PieceColor Color, PieceType Type)
{
	if (Color == PieceColor::PBLACK)
	{
		switch (Type) {
		case PieceType::BISHOP:
			return &BlackBishopClass;
		case PieceType::KNIGHT:
			return &BlackKnightClass;
		case PieceType::KING:
			return &BlackKingClass;
		case PieceType::QUEEN:
			return &BlackQueenClass;
		case PieceType::PAWN:
			return &BlackPawnClass;
		case PieceType::ROOK:
			return &BlackRookClass;
		default:
			return nullptr;
		}
	}
	if (Color == PieceColor::PWHITE)
	{
		switch (Type) {
		case PieceType::BISHOP:
			return &WhiteBishopClass;
		case PieceType::KNIGHT:
			return &WhiteKnightClass;
		case PieceType::KING:
			return &WhiteKingClass;
		case PieceType::QUEEN:
			return &WhiteQueenClass;
		case PieceType::PAWN:
			return &WhitePawnClass;
		case PieceType::ROOK:
			return &WhiteRookClass;
		default:
			return nullptr;
		}
	}
	return nullptr;
}

// TODO: Handle this better
void ACH_GameMode::MissingClass()
{
	UE_LOG(LogTemp, Error, TEXT("Missing class error"));
}
