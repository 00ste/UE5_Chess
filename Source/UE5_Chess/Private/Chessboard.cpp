#include "Chessboard.h"


AChessboard::AChessboard()
{
	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;
}

// !! THIS WAS MISSING
void AChessboard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AChessboard::CreateChessboard(double TileSize)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Setting up chessboard..."));
	UE_LOG(LogTemp, Error, TEXT("Setting up chessboard..."));
	if (WhiteTileClass == nullptr || BlackTileClass == nullptr)
	{
		// TODO: handle this better
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error! Missing Tile classes"));
		UE_LOG(LogTemp, Error, TEXT("Missing Tile Class!!"));
	}
	for (int32 x = 0; x < 8; x++)
	{
		for (int32 y = 0; y < 8; y++)
		{
			ATile* Obj = GetWorld()->SpawnActor<ATile>(
				(x + y) % 2 ? WhiteTileClass : BlackTileClass,
				FVector(x * TileSize * 100, y * TileSize * 100, 0),
				FRotator::ZeroRotator
			);
			Obj->SetActorScale3D(FVector(TileSize, TileSize, 0.1));
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Finished spawning!"));
	UE_LOG(LogTemp, Error, TEXT("Finished spawning!"));
}

void AChessboard::BeginPlay()
{
	Super::BeginPlay();
	CreateChessboard(1.0);	// !! THIS WAS MISSING
}

