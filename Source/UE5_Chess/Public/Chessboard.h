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
	AChessboard(double TileScale, double ChessPieceScale);

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
	// the Chessboard and Destroys it. Does nothing if
	// there's no ChessPiece at the given Position.
	// Returns true if the ChessPiece was Destroyed
	// successfully, otherwise returns false.
	bool RemoveChessPiece(FVector2D Position);

	// Puts a new ChessPiece of the given PieceType and
	// PieceColor at the given Position, returning a pointer
	// to the new ChessPiece that was placed, or nullptr if
	// there was already a ChessPiece in that position.
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	AChessPiece* PutChessPiece(PieceType type,
		PieceColor color, FVector2D Position);

	// Moves a ChessPiece from one Position to another,
	// returning true if the ChessPiece was moved
	// successfully, returns false if there was already
	// another ChessPiece in NewPosition
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	bool MoveChessPiece(FVector2D OldPosition,
		FVector2D NewPosition);

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

	// Scale factor of the side length of one Tile
	const double TileSize;
	// Scale factor of the diameter of one ChessPiece
	const double ChessPieceSize;
	
	// Subclasses for black and white tiles
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> BlackTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> WhiteTileClass;

	// Subclasses for every chess piece
	// Blacks
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> BlackBishopClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> BlackKingClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> BlackKnightClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> BlackQueenClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> BlackPawnClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> BlackRookClass;
	// Whites
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> WhiteBishopClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> WhiteKingClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> WhiteKnightClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> WhiteQueenClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> WhitePawnClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPiece> WhiteRookClass;

	// Utility function to map a PieceColor and PieceType
	// to a UClass* of a ChessPiece
	TSubclassOf<AChessPiece>* ColorTypeToClass(PieceColor Color, PieceType Type);

};
