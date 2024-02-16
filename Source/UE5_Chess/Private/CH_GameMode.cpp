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
		MissingClass(0);
		
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

void ACH_GameMode::ShowLegalMoves(FVector2D Position)
{
	AChessPiece* Piece = *ChessPieceMap.Find(Position);
	if (Piece == nullptr) return;

	// Initialise a list that will contain all legal moves for this
	// ChessPiece stored as Indicator elements
	TArray<AIndicator*> LegalMoves;

	// Determine which moves are legal based on the ChessPiece
	switch (Piece->GetType())
	{
		// PAWN
		// - Moves forward by 1
		// - Moves forward by 2 if it's at the start row
		// - Moves forward-diagonally if there's an opposing ChessPiece
	case PieceType::PAWN:
		// Calculate color value, 0 if WHITE, 1 if BLACK
		// color is used to handle forward movement in different directions
		// using a single expression
		uint32 color;
		if (Piece->GetColor() == PieceColor::WHITE) color = 0;
		else if (Piece->GetColor() == PieceColor::BLACK) color = 1;
		else return;

		// Temporary variables for better readability
		AChessPiece* TargetPiece;
		FVector2D TargetPosition;

		// Pawn can move forward by 1
		if (Position[1] < 7 && Position[1] > 0)
		{
			AIndicator* Indicator = SpawnIndicatorSafe(
				Position,
				Position + FVector2D(0, 2 * color - 1)
			);

			if (Indicator != nullptr)
				LegalMoves.Add(Indicator);
			/*
			uint32 deltaY = 2 * color - 1;
			TargetPiece = *ChessPieceMap.Find(Position + FVector2D(0, deltaY));
			TargetPosition = Position + FVector2D(0, deltaY);

			if (TargetPiece == nullptr)
				LegalMoves.Add(TargetPosition);
			*/
		}
		
		// If Pawn is at the start position it can also move forward by 2
		if (Position[1] == 1 + 6 * color)
		{
			AIndicator* Indicator = SpawnIndicatorSafe(
				Position,
				Position + FVector2D(0, 3 + color)
			);

			if (Indicator != nullptr)
				LegalMoves.Add(Indicator);
			/*
			uint32 deltaY = 3 + color;
			TargetPiece = *ChessPieceMap.Find(Position + FVector2D(0, deltaY));
			TargetPosition = Position + FVector2D(0, deltaY);

			if (TargetPiece == nullptr)
				LegalMoves.Add(TargetPosition);
			*/
		}

		// Opposing ChessPiece to the left and to the right
		// TODO: Fix this double declaration when writing SpawnIndicatorSafe()
		AIndicator* Indicator = SpawnIndicatorSafe(
			Position,
			Position + FVector2D(1, 2 * color - 1)
		);

		if (Indicator != nullptr)
			LegalMoves.Add(Indicator);

		AIndicator* Indicator = SpawnIndicatorSafe(
			Position,
			Position + FVector2D(-1, 2 * color - 1)
		);

		if (Indicator != nullptr)
			LegalMoves.Add(Indicator);
		/*
			uint32 deltaY = 2 * color - 1;
			uint32 deltaX = 1;
			TargetPiece = *ChessPieceMap.Find(Position + FVector2D(deltaX, deltaY));
			TargetPosition = Position + FVector2D(0, deltaY);

			if (TargetPiece != nullptr && TargetPiece->GetColor() != Piece->GetColor())
				LegalMoves.Add(TargetPosition);
		*/
		}
		break;
		// ROOK
		// - moves horizontally and vertically anywhere, even if there's
		//   an opposing ChessPiece
	case PieceType::ROOK:
		AIndicator* Indicator;
		for (uint32 i = 0; i < 7; i++) {
			Indicator = SpawnIndicatorSafe(
				Position,
				FVector2D(Position[0], i)
			);

			if (Indicator != nullptr)
				LegalMoves.Add(Indicator);
			
			Indicator = SpawnIndicatorSafe(
				Position,
				FVector2D(i, Position[1])
			);

			if (Indicator != nullptr)
				LegalMoves.Add(Indicator);
		}

		break;
		// BISHOP
		// - moves diagonally anywhere, even if there's
		//   an opposing ChessPiece
	case PieceType::BISHOP:
		break;
		// KNIGHT
			// - moves in an L-shape, even if there's an opposing ChessPiece
	case PieceType::KNIGHT:
		break;
		// QUEEN
		// - moves horizontally, vertically and diagonally anywhere, even if
		//   there's an opposing ChessPiece
	case PieceType::QUEEN:
		break;
		// QUEEN
		// - moves horizontally, vertically and diagonally by one tile, even
		//   if there's an opposing ChessPiece, however his moves are limited
		//   during a check
	case PieceType::KING:
		break;
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
		PutChessPiece(PieceType::PAWN, PieceColor::BLACK, FVector2D(i, 1));
		PutChessPiece(PieceType::PAWN, PieceColor::WHITE, FVector2D(i, 6));
	}

	// Rooks
	PutChessPiece(PieceType::ROOK, PieceColor::BLACK, FVector2D(0, 0));
	PutChessPiece(PieceType::ROOK, PieceColor::BLACK, FVector2D(7, 0));
	PutChessPiece(PieceType::ROOK, PieceColor::WHITE, FVector2D(0, 7));
	PutChessPiece(PieceType::ROOK, PieceColor::WHITE, FVector2D(7, 7));

	// Bishops
	PutChessPiece(PieceType::BISHOP, PieceColor::BLACK, FVector2D(1, 0));
	PutChessPiece(PieceType::BISHOP, PieceColor::BLACK, FVector2D(6, 0));
	PutChessPiece(PieceType::BISHOP, PieceColor::WHITE, FVector2D(1, 7));
	PutChessPiece(PieceType::BISHOP, PieceColor::WHITE, FVector2D(6, 7));

	// Knights
	PutChessPiece(PieceType::KNIGHT, PieceColor::BLACK, FVector2D(2, 0));
	PutChessPiece(PieceType::KNIGHT, PieceColor::BLACK, FVector2D(5, 0));
	PutChessPiece(PieceType::KNIGHT, PieceColor::WHITE, FVector2D(2, 7));
	PutChessPiece(PieceType::KNIGHT, PieceColor::WHITE, FVector2D(5, 7));

	// Queens
	PutChessPiece(PieceType::QUEEN, PieceColor::BLACK, FVector2D(3, 0));
	PutChessPiece(PieceType::QUEEN, PieceColor::WHITE, FVector2D(3, 7));

	// Kings
	PutChessPiece(PieceType::QUEEN, PieceColor::WHITE, FVector2D(4, 7));
	PutChessPiece(PieceType::QUEEN, PieceColor::BLACK, FVector2D(4, 0));
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
		MissingClass(1);
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
	if (Color == PieceColor::BLACK)
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
	if (Color == PieceColor::WHITE)
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

AIndicator* ACH_GameMode::SpawnIndicatorSafe(FVector2D StartPosition, FVector2D EndPosition)
{
	return nullptr;
}

// TODO: Handle this better
void ACH_GameMode::MissingClass(uint32 ErrorCode)
{
	UE_LOG(LogTemp, Error, TEXT("Missing class error! Code: ", ErrorCode));
}
