#include "Chessboard.h"

// Sets default values
AChessboard::AChessboard()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

AChessPiece* AChessboard::getChessPieceAt(FVector2D Position) const
{
	if (ChessPieceMap.Contains(position))
		return ChessPieceMap[position];
	return nullptr;
}

void AChessboard::createChessboard()
{
	for (int32 x = 0; x < 8; x++)
	{
		for (int32 y = 0; y < 8; y++)
		{
			FVector Location = FVector2D(x*TileSize, y*TileSize);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}

void AChessboard::prepareChessboard()
{
}

AChessPiece* AChessboard::removeChessPiece(FVector2D Position)
{
	if (!ChessPieceMap.Contains(Position))
		return nullptr;

	AChessPiece* x = ChessPieceMap[Position];
	ChessPieceMap.Remove(Position);
	return x;
}

AChessPiece* AChessboard::putChessPiece(PieceType Type, PieceColor Color, FVector2D Position)
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

	createChessboard();
	prepareChessboard();
}

/*
// Called every frame
void AChessboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
