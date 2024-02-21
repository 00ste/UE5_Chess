// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH_PlayerInterface.h"
#include "ChessPiece.h"
#include "Chessboard.h"
#include "Indicator.h"
#include "CH_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API ACH_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACH_GameMode();

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

	// Changes the CurrentPlayer index to the next player
	// and calls that player's OnTurn() method
	void TurnNextPlayer();

	// Recursively explores a line of Tiles starting from Position
	// along Direction, and stops either when an obstacle is found
	// or when MaxLength Tiles have been explored. The Tiles' positions
	// are all appended to Moves. If CanCapture is true, a Tile that
	// contains a ChessPiece owned by the opponent will count as a
	// legal move (capturing).
	void ExploreDirection(FVector2D Position, FVector2D Direction,
		uint32 MaxLength, bool CanCapture, TArray<FVector2D>* Moves,
		PieceColor PlayerColor);

	// Spawns Indicators on the Chessboard that indicate
	// where a piece at Position can be moved by the player
	void ShowLegalMoves(FVector2D Position);

	

protected:
	// Initializes the two players and the Chessboard and
	// starts the game
	virtual void BeginPlay() override;

private:
	bool IsGameOver;

	// 0 = HumanPlayer (WHITE)
	// 1 = AIPlayer / RandomPlayer (BLACK)
	TArray<ICH_PlayerInterface*> Players;
	uint32 CurrentPlayer;

	AChessboard* Chessboard;

	// Scale factors
	double TileSize;
	double ChessPieceSize;

	// Map that stores the ChessPieces on the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, AChessPiece*> ChessPieceMap;

	// Array of Indicators that will show all possible moves
	// for the currently selected ChessPiece
	UPROPERTY(Transient)
	TArray<AIndicator*> Indicators;


	// Subclasses
	//Chessboard
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;
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
	// Indicators
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIndicator> MoveIndicatorClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIndicator> CaptureIndicatorClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIndicator> PromoteIndicatorClass;

	// Utility function to map a PieceColor and PieceType
	// to a UClass* of a ChessPiece
	TSubclassOf<AChessPiece>* ColorTypeToClass(PieceColor Color, PieceType Type);

	/*
	// Spawn Indicator at given end position, checking the bounds
	// of the Chessboard and checking for collisions with other
	// ChessPieces, preventing movement or allowing capture
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	AIndicator* SpawnIndicatorSafe(FVector2D StartPosition, FVector2D EndPosition);
	*/

	AIndicator* SpawnIndicator(FVector2D StartPosition, FVector2D EndPosition);

	// TODO: handle this better
	void MissingClass();
};
