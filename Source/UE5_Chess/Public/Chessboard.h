// The 8x8 Chessboard made up of 64 black and white Tiles.
// The Chessboard also stores information about the
// ChessPieces that live on it.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "ChessPiece.h"
#include "Chessboard.generated.h"

UCLASS()
class UE5_CHESS_API AChessboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessboard();

	// Gets the ChessPiece object at the given Position.
	// Returns a pointer to the ChessPiece, or nullptr if
	// the Tile at the given Position is empty.
	AChessPiece* GetChessPieceAt(FVector2D Position) const;

	// Creates the Tiles that make up the Chessboard
	void CreateChessboard();

	// Prepares the Chessboard by placing each ChessPiece
	// in the correct Position
	void PrepareChessboard();

	// Removes the ChessPiece at the given Position from
	// the Chessboard. Returns a pointer to the removed
	// ChessPiece. Does nothing and returns nullptr if
	// there's no ChessPiece at the given Position.
	AChessPiece* RemoveChessPiece(FVector2D Position);

	// Puts a new ChessPiece of the given PieceType and
	// PieceColor at the given Position, returning a pointer
	// to the old ChessPiece, or nullptr if there was none,
	// without checking if the move is legal!!
	AChessPiece* PutChessPiece(PieceType type,
		PieceColor color, FVector2D Position);

	// Moves a ChessPiece from one Position to another,
	// returning a pointer to the old ChessPiece, or nullptr
	// if there was none, without checking if the move is legal!!
	AChessPiece* moveChessPiece(FVector2D oldPosition,
		FVector2D newPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Map that stores the Tiles that make up the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	// Map that stores the ChessPieces on the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, AChessPiece*> ChessPieceMap;

	// Scale factor of the length of one Tile
	const double TileSize;
	
	// Subclasses for black and white tiles
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> BlackTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> WhiteTileClass;

};
