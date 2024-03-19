// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH_PlayerInterface.h"
#include "ChessPiece.h"
#include "Chessboard.h"
#include "Indicator.h"
#include "CH_GameMode.generated.h"


UCLASS()
class UE5_CHESS_API ACH_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Initializes the two players and the Chessboard and
	// starts the game
	virtual void BeginPlay() override;

public:
	// ===== GAMEMODE ===== //

	// Sets default values for this actor's properties
	ACH_GameMode();
	
	// Returns the TileSize
	double GetTileSize() const;

	// Prepares the Chessboard by placing each ChessPiece
	// in the correct Position
	void PrepareChessboard();

	// Changes the CurrentPlayer index to the next player
	// and calls that player's OnTurn() method
	void TurnNextPlayer();

	// Executes the move indicated by the Indicator and removes
	// all Indicators afterwards
	void DoMove(AIndicator const* Indicator);

	// Calculates all the possible legal moves for a ChessPiece
	// at the given Position
	TArray<TArray<FVector2D>> CalculateLegalMoves(FVector2D Position);

	// Calculates all the possible legale moves for all ChessPieces
	// with a given Color
	TArray<TArray<FVector2D>> CalculateAllMoves(PieceColor Color);

	// Checks if the given player is in a Check state
	bool CheckCheck(PieceColor Color);

	// ===== CHESSPIECE OPERATIONS ===== //

	// Returns a pointer to a const ChessPiece at the specified Position,
	// Returns nullptr if there is no ChessPiece at that Position
	AChessPiece const* GetConstChessPieceAt(FVector2D Position) const;

	// Removes all ChessPieces from the Chessboard
	void RemoveAllChessPieces();
	
	// ===== INDICATOR OPERATIONS ===== //

	// Spawns Indicator Actors for the given TArray of moves.
	void ShowIndicatorsForMoves(const TArray<TArray<FVector2D>>& Moves);

	// Removes all Indicators from the Chessboard
	void RemoveAllIndicators();

	// Returns an Indicator which has the given EndPosition,
	// returns nullptr if no such Indicator exists
	AIndicator const* GetIndicatorForEndPos(FVector2D EndPos);

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
	// Chessboard
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
	TSubclassOf<AChessPiece> ColorTypeToClass(PieceColor Color, PieceType Type);

	// Recursively explores a line of Tiles starting from Position
	// along Direction, and stops either when an obstacle is found
	// or when MaxLength Tiles have been explored. The Tiles' positions
	// are all appended to Moves. If CanCapture is true, a Tile that
	// contains a ChessPiece owned by the opponent will count as a
	// legal move (capturing).
	void ExploreDirection(FVector2D Position, FVector2D Direction,
		uint32 MaxLength, bool CanCapture, TArray<FVector2D>* Moves,
		PieceColor PlayerColor);

	// ===== CHESSPIECE OPERATIONS ===== //

	// Returns a pointer to the ChessPiece at the specified Position,
	// Returns nullptr if there is no ChessPiece at that Position
	AChessPiece* GetChessPieceAt(FVector2D Position);

	// Puts a new ChessPiece of the given PieceType and
	// PieceColor at the given Position, returning a pointer
	// to the new ChessPiece that was placed, or nullptr if
	// there was already a ChessPiece in that position.
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	AChessPiece* PutChessPiece(PieceType type,
		PieceColor color, FVector2D Position);

	// Removes the ChessPiece at the given Position from
	// the Chessboard and Destroys it. Does nothing if
	// there's no ChessPiece at the given Position.
	// Returns true if the ChessPiece was Destroyed
	// successfully or if there was no ChessPiece at the
	// given Position, otherwise returns false.
	bool RemoveChessPiece(FVector2D Position);

	// Moves a ChessPiece from one Position to another,
	// returning true if the ChessPiece was moved
	// successfully, returns false if there was no piece
	// in the OldPosition
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	bool MoveChessPiece(FVector2D OldPosition,
		FVector2D NewPosition);

	// ===== INDICATOR OPERATIONS ===== //

	// Creates a new indicator on the EndPosition representing
	// a possible move from StartPosition to EndPosition, adding
	// it to the internal TArray of Indicators.
	// Returns a pointer to the newly spawned Indicator, or nullptr
	// if a different Indicator existed with the same EndPosition.
	// The type of the Indicator will be calculated as follows:
	// - PROMOTE Indicator if StartPosition == EndPosition
	// - MOVE Indicator if the EndPosition is free
	// - CAPTURE Indicator if the EndPosition is occupied
	AIndicator* PutIndicator(FVector2D StartPosition, FVector2D EndPosition);
};
