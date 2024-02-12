#include "Chessboard.h"

// Sets default values
AChessboard::AChessboard()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

AChessPiece* AChessboard::GetChessPieceAt(FVector2D Position) const
{
	if (ChessPieceMap.Contains(Position))
		return ChessPieceMap[Position];
	return nullptr;
}

void AChessboard::CreateChessboard()
{
	for (int32 x = 0; x < 8; x++)
	{
		for (int32 y = 0; y < 8; y++)
		{
			ATile* Obj = GetWorld()->SpawnActor<ATile>(
				(x + y) % 2 ? WhiteTileClass : BlackTileClass,
				FVector(x, y, 0) * TileSize,
				FRotator::ZeroRotator
			);
			Obj->SetActorScale3D(FVector(1.0, 1.0, 0.1));
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}

void AChessboard::PrepareChessboard()
{
	// Delete all existing ChessPieces
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);
	for (FVector2D Position : Positions) {
		ChessPieceMap[Position]->Destroy();
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

AChessPiece* AChessboard::RemoveChessPiece(FVector2D Position)
{
	if (!ChessPieceMap.Contains(Position))
		return nullptr;

	AChessPiece* x = ChessPieceMap[Position];
	ChessPieceMap.Remove(Position);
	return x;
}

AChessPiece* AChessboard::PutChessPiece(PieceType Type, PieceColor Color, FVector2D Position)
{
	AChessPiece* OldChessPiece;
	if (!ChessPieceMap.Contains(Position))
		OldChessPiece = nullptr;
	
	OldChessPiece = ChessPieceMap[Position];
	ChessPieceMap.Remove(Position);

	// Add ChessPiece in the newPosition
	// TODO: Create a new ChessPiece
	AChessPiece* NewChessPiece;
	ChessPieceMap.Add(Position, NewChessPiece);

	return OldChessPiece;
}

AChessPiece* AChessboard::moveChessPiece(FVector2D OldPosition, FVector2D NewPosition)
{
	// check that there is a ChessPiece in the oldPosition
	if (!ChessPieceMap.Contains(OldPosition))
		return;

	// Remove ChessPiece from oldPosition
	AChessPiece* OldChessPiece = ChessPieceMap[OldPosition];
	ChessPieceMap.Remove(OldPosition);

	// Add ChessPiece in the newPosition
	ChessPieceMap.Add(NewPosition, OldChessPiece);
}


// Called when the game starts or when spawned
void AChessboard::BeginPlay()
{
	Super::BeginPlay();

	CreateChessboard();
	PrepareChessboard();
}

/*
// Called every frame
void AChessboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
