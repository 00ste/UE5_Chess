// The 8x8 Chessboard made up of 64 black and white Tiles.
// The Chessboard also stores information about the
// ChessPieces that live on it.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
// #include "ChessPiece.h" (!! maybe not necessary?)
#include "Chessboard.generated.h"

UCLASS()
class UE5_CHESS_API AChessboard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChessboard();

	// Called when an instance of this class is placed (in editor) or spawned (!! THIS WAS MISSING)
	virtual void OnConstruction(const FTransform& Transform) override;

	// Creates the Tiles that make up the Chessboard
	void CreateChessboard(double TileSize);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Map that stores the Tiles that make up the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	// Subclasses for black and white tiles
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> BlackTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> WhiteTileClass;

};
