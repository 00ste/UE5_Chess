// The 8x8 Chessboard made up of 64 black and white Tiles.
// The Chessboard also stores information about the
// ChessPieces that live on it.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "ChessPiece.h"
#include "Indicator.h"
#include "Chessboard.generated.h"

UCLASS()
class UE5_CHESS_API AChessboard : public AActor
{
	GENERATED_BODY()

public:

	// ZIndex is: 1 for Tiles, 2 for ChessPieces, 3 for Indicators,
	// -1 for captured ChessPieces
	enum ZIndexType : int32
	{
		TILE_ZINDEX = 1, CHESSPIECE_ZINDEX = 2, INDICATOR_ZINDEX = 3,
		CAPTURED_ZINDEX = -1
	};

	// Sets default values for this actor's properties
	AChessboard();

	double GetTileSize() const;

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// Creates the Tiles that make up the Chessboard
	void CreateChessboard();

	// Creates AND SPAWNS a new ChessPiece
	AChessPiece* AddNewChessPiece(PieceType Type, PieceColor Color, FVector2D Position);

	// Removes the ChessPiece at the given Position and saves it
	// to RemovedPieces
	bool RemoveChessPiece(FVector2D Position);

	// Deletes AND DESTROYS the ChessPiece at the given Position
	// without saving it anywhere. Does nothing and returns true if there was
	// no ChessPiece at the given Position or if the ChessPiece could
	// not be destroyed, returns true otherwise.
	bool DeleteChessPiece(FVector2D Position);

	// Deletes AND DESTROYS all ChessPieces on the Chessboard without
	// saving them anywhere.
	void DeleteAllChessPieces();

	// Moves the ChessPiece in the StartPosition to the EndPosition
	// automatically removes any existing ChessPieces at the EndPosition.
	// If bCanOverwrite is true, any existing ChessPiece at the NewPosition
	// will be removed and added to the RemovedHistory, returning true.
	// If bCanOverwrite is false, if there is already a ChessPiece at the
	// NewPosition false will be returned and nothing will happen.
	bool MoveChessPiece(FVector2D OldPosition, FVector2D NewPosition, bool bCanOverwrite);

	// Returns a pointer to the ChessPiece at the given Position
	AChessPiece* GetChessPieceAt(FVector2D Position);

	// Restores the last captured ChessPiece. Returns false if there
	// was no ChessPiece to restore, otherwise returns true
	AChessPiece* GetLastCapturedChessPiece();

	// Moves all the AChessPiece Actors to the location they're assigned
	// to in the ChessPieceMap and hides the AChessPiece Actors that have
	// been captured
	void UpdateChessboard();

	// Deletes AND DESTROYS all Indicators
	void DeleteAllIndicators();

	// Conversions between Real position and Grid position
	inline FVector2D RealToGridPosition(FVector RealPosition);
	inline FVector GridToRealPosition(FVector2D GridPosition, uint32 ZIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Scale factors
	double TileSize;
	double ChessPieceSize;

	// History of removed ChessPieces
	TArray<AChessPiece*> RemovedPieces;

	// Height difference between Actors occupying the same position
	const FVector HeightDiff = FVector(0, 0, 0.05);

	// Map that stores the ATile Actors that make up the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	// Map that stores the AChessPiece Actors on the Chessboard
	// each mapped to a FVector2D logical position on the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, AChessPiece*> ChessPieceMap;

	// Array that stores the AIndicator Actors
	// TODO: Possibly a TMap would be better?
	UPROPERTY(Transient)
	TArray<AIndicator*> Indicators;

	// Subclasses
	// Tiles
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> BlackTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> WhiteTileClass;
	// Black ChessPieces
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
	// White ChessPieces
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

	// Utility function to map enum parameters to TSubclasses
	TSubclassOf<AChessPiece> ColorTypeToClass(PieceColor Color, PieceType Type) const;

};
