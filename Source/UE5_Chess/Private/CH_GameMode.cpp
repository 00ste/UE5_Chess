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
	// The Chessboard will be aligned to the left, so there will be
	// an offset along the x-axis
	double XOffset = 290.0;

	// Init players and add to list
	FVector CameraPosition = {
		4 * TileSize * 100,
		4 * TileSize * 100 + XOffset,
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

ACH_GameMode::ACH_GameMode()
{
	PlayerControllerClass = ACH_PlayerController::StaticClass();
	DefaultPawnClass = ACH_HumanPlayer::StaticClass();
	TileSize = 1.0;
	ChessPieceSize = 0.9;
}

double ACH_GameMode::GetTileSize() const { return TileSize; }

void ACH_GameMode::PrepareChessboard()
{
	// Delete all existing ChessPieces
	RemoveAllChessPieces();

	// Put ChessPieces on the Chessboard
	// Pawns
	for (uint32 i = 0; i < 8; i++) {
		PutChessPiece(PieceType::PAWN, PieceColor::PBLACK, FVector2D(i, 6));
		PutChessPiece(PieceType::PAWN, PieceColor::PWHITE, FVector2D(i, 1));
	}

	// Rooks
	PutChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(0, 7));
	PutChessPiece(PieceType::ROOK, PieceColor::PBLACK, FVector2D(7, 7));
	PutChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(0, 0));
	PutChessPiece(PieceType::ROOK, PieceColor::PWHITE, FVector2D(7, 0));

	// Bishops
	PutChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(1, 7));
	PutChessPiece(PieceType::BISHOP, PieceColor::PBLACK, FVector2D(6, 7));
	PutChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(1, 0));
	PutChessPiece(PieceType::BISHOP, PieceColor::PWHITE, FVector2D(6, 0));

	// Knights
	PutChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(2, 7));
	PutChessPiece(PieceType::KNIGHT, PieceColor::PBLACK, FVector2D(5, 7));
	PutChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(2, 0));
	PutChessPiece(PieceType::KNIGHT, PieceColor::PWHITE, FVector2D(5, 0));

	// Queens
	PutChessPiece(PieceType::QUEEN, PieceColor::PBLACK, FVector2D(3, 7));
	PutChessPiece(PieceType::QUEEN, PieceColor::PWHITE, FVector2D(3, 0));

	// Kings
	PutChessPiece(PieceType::KING, PieceColor::PWHITE, FVector2D(4, 0));
	PutChessPiece(PieceType::KING, PieceColor::PBLACK, FVector2D(4, 7));
}

void ACH_GameMode::TurnNextPlayer()
{
	CurrentPlayer = (CurrentPlayer + 1) % 2;
	Players[CurrentPlayer]->OnTurn();
}

void ACH_GameMode::DoMove(AIndicator const* Indicator)
{
	MoveChessPiece(Indicator->GetStartPosition(), Indicator->GetEndPosition());
	RemoveAllIndicators();
}

TArray<TArray<FVector2D>> ACH_GameMode::CalculateLegalMoves(FVector2D Position)
{
	RemoveAllIndicators();

	// Get the ChessPiece and check that it's valid
	// AChessPiece* Piece = *ChessPieceMap.Find(Position);
	AChessPiece const* Piece = GetChessPieceAt(Position);
	if (Piece == nullptr) return TArray<TArray<FVector2D>>();
	if (Piece->GetColor() == PieceColor::PCNONE) return TArray<TArray<FVector2D>>();
	if (Piece->GetType() == PieceType::PTNONE) return TArray<TArray<FVector2D>>();

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

		// The direction in which Pawns can move is always along +y
		// (YOffset = 1) for WHITE pawns and -y (YOffset = -1) for BLACK Pawns
		uint32 YOffset = 1 - 2 * ColorIndex;

		// A Pawn can move forwards by 2 tiles if it's in the start position
		// The start position is y=1 for WHITE Pawns and y=6 for BLACK Pawns
		uint32 MaxLength = Position[1] == 5 * ColorIndex + 1 ? 2 : 1;

		ExploreDirection(
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
			AChessPiece const* Target = GetChessPieceAt(TargetPos);
			if (Target != nullptr && Target->GetColor() != Piece->GetColor())
			{
				Moves.Add(TargetPos);
			}
		}

		// A PAWN can be promoted to another type when it reaches the opposite
		// end of the ChessBoard
		if (Position[1] == 7 * (1 - ColorIndex))
		{
			Moves.Add(Position);
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
	
	// Generate the final TArray of moves including
	// start position (Position) and EndPosition
	// TODO: (optimisation) use array of start-end positions from the beginning
	TArray<TArray<FVector2D>> Result;
	for (FVector2D EndPosition : Moves)
		Result.Add({Position, EndPosition});
	return Result;
}

TArray<TArray<FVector2D>> ACH_GameMode::CalculateAllMoves(PieceColor Color)
{
	TArray<TArray<FVector2D>> Moves;
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);
	for (FVector2D Position : Positions)
	{
		if (GetChessPieceAt(Position)->GetColor() == Color)
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
		if (GetChessPieceAt(Position)->GetColor() == OtherColor)
		{
			for (TArray<FVector2D> Move : CalculateLegalMoves(Position))
			{
				AChessPiece* Piece = GetChessPieceAt(Move[1]);
				if (Piece != nullptr && Piece->GetType() == KING)
				{
					return true;
				}
			}
		}
	}
	return false;
}

AChessPiece const* ACH_GameMode::GetConstChessPieceAt(FVector2D Position) const
{
	AChessPiece* const* result = ChessPieceMap.Find(Position);
	return result != nullptr ? *result : nullptr;
}

void ACH_GameMode::RemoveAllChessPieces()
{
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);

	for (FVector2D Position : Positions) {
		GetChessPieceAt(Position)->Destroy();
	}
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

void ACH_GameMode::ShowIndicatorsForMoves(const TArray<TArray<FVector2D>>& Moves)
{
	for (auto Move : Moves)
	{
		Indicators.Add(PutIndicator(Move[0], Move[1]));
	}
}

void ACH_GameMode::RemoveAllIndicators()
{	
	for (AIndicator* x : Indicators)
	{
		x->Destroy();
	}
	Indicators.Empty();
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

void ACH_GameMode::ExploreDirection(FVector2D Position, FVector2D Direction,
	uint32 MaxLength, bool CanCapture, TArray<FVector2D>* Moves, PieceColor PlayerColor)
{
	FVector2D TargetPosition = Position + Direction;
	// Check if TargetPosition is out of bounds
	if (TargetPosition[0] < 0 || TargetPosition[0] > 7) return;
	if (TargetPosition[1] < 0 || TargetPosition[1] > 7) return;
	if (MaxLength == 0) return;

	// Check if TargetPosition is free
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

AChessPiece* ACH_GameMode::GetChessPieceAt(FVector2D Position)
{
	if (ChessPieceMap.Contains(Position))
		return ChessPieceMap[Position];
	return nullptr;
}

bool ACH_GameMode::RemoveChessPiece(FVector2D Position)
{
	AChessPiece* x = GetChessPieceAt(Position);
	if (x == nullptr) return true;
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
		FVector(Position[1] + 0.5, Position[0] + 0.5, 0.001) * ChessPieceSize * 100 * TileSize/ChessPieceSize, // FOR SOME REASON THIS IS NEEDED
		FRotator::ZeroRotator
	);

	// Set up ChessPiece
	ChessPiece->Setup(Type, Color);

	// Add ChessPiece to ChessPieceMap
	ChessPieceMap.Add(Position, ChessPiece);

	return ChessPiece;
}

bool ACH_GameMode::MoveChessPiece(FVector2D OldPosition, FVector2D NewPosition)
{
	if (GetChessPieceAt(OldPosition) == nullptr) return false;

	// Remove any already existing ChessPieces in the NewPosition
	AChessPiece* TempChessPiece = GetChessPieceAt(NewPosition);
	if (TempChessPiece != nullptr)
	{
		ChessPieceMap.FindAndRemoveChecked(NewPosition);
		TempChessPiece->Destroy();
		// TODO: add to captured pieces
	}

	// Change the record inside the ChessPieceMap
	TempChessPiece = GetChessPieceAt(OldPosition);
	ChessPieceMap.FindAndRemoveChecked(OldPosition);
	/*
	ChessPieceMap.Add(NewPosition, TempChessPiece);

	// Move Actor in the new position of the scene
	// TODO: adjust position vector element order
	FHitResult temp; // needed by K2_SetActorLocation, will get ignored
	TempChessPiece->K2_SetActorLocation(
		FVector(NewPosition[0], NewPosition[1], 0.001) * TileSize,
		false,
		temp,
		true
	);
	*/
	PieceType Type = TempChessPiece->GetType();
	PieceColor Color = TempChessPiece->GetColor();
	TempChessPiece->Destroy();

	PutChessPiece(Type, Color, NewPosition);
	return true;
}

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
		if (GetChessPieceAt(EndPosition) == nullptr)
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
		FVector(EndPosition[1] + 0.5, EndPosition[0] + 0.5, 0.002) * TileSize * 100,
		FRotator::ZeroRotator
	);

	// Set up Indicator
	Indicator->Setup(Type, StartPosition, EndPosition);
	return Indicator;
}

