// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_GameMode.h"


ACH_GameMode::ACH_GameMode()
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

	if (ChessboardClass != nullptr)
	{
		Chessboard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Chessboard is null"));
	}
	
	Players[CurrentPlayer]->OnTurn();
}

void ACH_GameMode::TurnNextPlayer()
{
	CurrentPlayer = (CurrentPlayer + 1) % 2;
	Players[CurrentPlayer]->OnTurn();
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
	AChessPiece* ChessPiece = GetWorld()->SpawnActor<AChessPiece>(
		Cast<UClass>(ColorTypeToClass(Color, Type)),
		FVector(Position[0], Position[1], 0) * ChessPieceSize,
		FRotator::ZeroRotator
	);

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
