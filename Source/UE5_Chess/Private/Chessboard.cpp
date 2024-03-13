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
		UE_LOG(LogTemp, Error, TEXT("Missing Tile Class!!"));
	}
	for (int32 x = 0; x < 8; x++)
	{
		for (int32 y = 0; y < 8; y++)
		{
			// Tiles are offset by 0.5 so the x,y coordinate indicates their bottom-left
			// corner rather than their center. This facilitates calculating the clicked
			// Tile from the Hit ImpactPoint
			ATile* Obj = GetWorld()->SpawnActor<ATile>(
				(x + y) % 2 ? WhiteTileClass : BlackTileClass,
				
				FVector(x+0.5, y+0.5, 0) * TileSize * 100,
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
	/// CreateChessboard(1.0);	// !! THIS WAS MISSING (BECAUSE GAMEMODE SHOULD DO IT!!!!)
}

