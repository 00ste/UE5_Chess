// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_GameMode.h"
#include "ChessPiece.h"
#include "CH_PlayerController.h"
#include "CH_HumanPlayer.h"
#include "CH_RandomPlayer.h"
#include "EngineUtils.h"


ACH_GameMode::ACH_GameMode()
{
	PlayerControllerClass = ACH_PlayerController::StaticClass();
	DefaultPawnClass = ACH_HumanPlayer::StaticClass();
	TileSize = 1.0;
	ChessPieceSize = 0.9;
}

double ACH_GameMode::GetTileSize() const { return TileSize; }

void ACH_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	// Init players and add to list
	FVector CameraPosition = {
		4 * TileSize * 100,
		4 * TileSize * 100,
		1000.0f
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
	Chessboard->CreateChessboard(TileSize);
	PrepareChessboard();
	
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
	// AChessPiece* TargetPiece = *ChessPieceMap.Find(TargetPosition);
	AChessPiece const* TargetPiece = GetChessPieceAt(TargetPosition);
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
		if (GetChessPieceAt(StartPosition) == nullptr)
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
		UE_LOG(LogTemp, Error, TEXT("Missing Indicator class"));

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

void ACH_GameMode::RemoveIndicators()
{
	// TODO: Not sure if this is the right way to do it
	for (AIndicator* x : Indicators)
	{
		Indicators.Remove(x);
		x->Destroy();
	}
}

void ACH_GameMode::DoMove(AIndicator const* Indicator)
{
	MoveChessPiece(Indicator->GetStartPosition(), Indicator->GetEndPosition());
	RemoveIndicators();
}

AIndicator const* ACH_GameMode::GetIndicatorForEndPos(FVector2D EndPos)
{
	for (AIndicator* Indicator : Indicators)
	{
		if (Indicator->GetEndPosition() == EndPos)
			return Indicator;
	}
	return nullptr;
}

void ACH_GameMode::ShowLegalMoves(FVector2D Position)
{
	RemoveIndicators();

	// Get the ChessPiece and check that it's valid
	// AChessPiece* Piece = *ChessPieceMap.Find(Position);
	AChessPiece const* Piece = GetChessPieceAt(Position);
	if (Piece == nullptr) return;
	if (Piece->GetColor() == PieceColor::PCNONE) return;
	if (Piece->GetType() == PieceType::PTNONE) return;

	// This will contain all legal moves stored FVector2D
	// objects representing the end position of the move
	TArray<FVector2D> Moves;

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
		uint32 yOffset = 2 * ColorIndex + 1;

		// Movement
		// If the pawn is at the start position (which is a function of the
		// ColorIndex), it can move by 2 tiles instead of 1
		uint32 MaxLength = Position[1] == 6 - 5 * ColorIndex ? 2 : 1;
		ExploreDirection(
			Position,
			FVector2D(0, yOffset),
			MaxLength,
			false,
			&Moves,
			Piece->GetColor()
		);

		// Capture can't be added using ExploreDirection, so it's added
		// manually instead
		for (uint32 xOffset = -1; xOffset <= 1; xOffset += 2)
		{
			FVector2D TargetPos = Position + FVector2D(xOffset, yOffset);
			AChessPiece const* Target = GetChessPieceAt(TargetPos);
			if (Target != nullptr && Target->GetColor() != Piece->GetColor())
			{
				Moves.Add(TargetPos);
			}
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
			Direction,
			MaxLength,
			true,
			&Moves,
			Piece->GetColor()
		);
	}
	
	// Spawn the Indicators
	for (FVector2D EndPosition : Moves)
		Indicators.Add(SpawnIndicator(Position, EndPosition));
}

AChessPiece const* ACH_GameMode::GetChessPieceAt(FVector2D Position) const
{
	AChessPiece* const* result = ChessPieceMap.Find(Position);
	return result != nullptr ? *result : nullptr;
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
	TSubclassOf<AChessPiece> PieceClass = ColorTypeToClass(Color, Type);
	if (PieceClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing ChessPiece class"));
	}
	AChessPiece* ChessPiece = GetWorld()->SpawnActor<AChessPiece>(
		PieceClass,
		FVector(Position[1], Position[0], 0) * ChessPieceSize * 100 * TileSize/ChessPieceSize, // FOR SOME REASON THIS IS NEEDED
		FRotator::ZeroRotator
	);

	// Set up ChessPiece
	ChessPiece->Setup(Type, Color);

	// Add ChessPiece to ChessPieceMap
	ChessPiece->SetActorScale3D(FVector(1.0, 1.0, 0.1));
	ChessPieceMap.Add(Position, ChessPiece);

	return ChessPiece;
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

TSubclassOf<AChessPiece> ACH_GameMode::ColorTypeToClass(PieceColor Color, PieceType Type)
{
	if (Color == PieceColor::PBLACK)
	{
		switch (Type) {
		case PieceType::BISHOP:
			return BlackBishopClass;
		case PieceType::KNIGHT:
			return BlackKnightClass;
		case PieceType::KING:
			return BlackKingClass;
		case PieceType::QUEEN:
			return BlackQueenClass;
		case PieceType::PAWN:
			return BlackPawnClass;
		case PieceType::ROOK:
			return BlackRookClass;
		default:
			return nullptr;
		}
	}
	if (Color == PieceColor::PWHITE)
	{
		switch (Type) {
		case PieceType::BISHOP:
			return WhiteBishopClass;
		case PieceType::KNIGHT:
			return WhiteKnightClass;
		case PieceType::KING:
			return WhiteKingClass;
		case PieceType::QUEEN:
			return WhiteQueenClass;
		case PieceType::PAWN:
			return WhitePawnClass;
		case PieceType::ROOK:
			return WhiteRookClass;
		default:
			return nullptr;
		}
	}
	return nullptr;
}

