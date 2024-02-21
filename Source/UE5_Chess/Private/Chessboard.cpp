#include "Chessboard.h"


AChessboard::AChessboard()
{
 	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;
}

void AChessboard::CreateChessboard(double TileSize)
{
	if (WhiteTileClass == nullptr || BlackTileClass == nullptr)
	{
		// TODO: handle this better
		UE_LOG(LogTemp, Error, TEXT("Missing Tile Class!!"));
	}
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

}

