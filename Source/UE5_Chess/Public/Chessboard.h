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

	// Gets the ChessPiece object at the given position.
	// Returns a pointer to the ChessPiece, or nullptr if
	// the Tile at the given position is empty.
	AChessPiece* getChessPieceAt(FVector2D position) const;

	// Creates the Tiles that make up the Chessboard
	void createChessboard();

	// Prepares the Chessboard by placing each ChessPiece
	// in the correct position
	void prepareChessboard();

	// Removes the ChessPiece at the given position from
	// the Chessboard. Returns a pointer to the removed
	// ChessPiece. Does nothing and returns nullptr if
	// there's no ChessPiece at the given position.
	AChessPiece* removeChessPiece(FVector2D position);

	// Puts a new ChessPiece of the given PieceType and
	// PieceColor at the given position, returning a pointer
	// to the old ChessPiece, or nullptr if there was none,
	// without checking if the move is legal!!
	AChessPiece* putChessPiece(PieceType type, PieceColor color, FVector2D position);

	// Moves a ChessPiece from one position to another,
	// returning a pointer to the old ChessPiece, or nullptr
	// if there was none, without checking if the move is legal!!
	AChessPiece* moveChessPiece(FVector2D oldPosition, FVector2D newPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// TODO: may not be useful
	// Map that stores the Tiles that make up the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	// Map that stores the ChessPieces on the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, AChessPiece*> ChessPieceMap;

	// Scale factor of the length of one Tile
	const double TileSize;

};
