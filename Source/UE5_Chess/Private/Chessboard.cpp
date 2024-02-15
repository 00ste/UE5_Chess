#include "Chessboard.h"


AChessboard::AChessboard(double TileScale, double ChessPieceScale)
	: TileSize{TileScale}, ChessPieceSize{ChessPieceScale}
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

void AChessboard::BeginPlay()
{
	Super::BeginPlay();

	CreateChessboard();
	PrepareChessboard();
}

