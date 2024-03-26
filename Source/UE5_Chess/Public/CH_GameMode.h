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

	// Executes the Move given by changing the record in
	// ChessPieceMap and moving the AChessPiece Actors.
	void DoMove(FChessMove Move);

	// Returns true if the Move results in a non-Check state,
	// returns false otherwise
	bool DoesMoveUncheck(FChessMove Move);

	// Undoes the last ChessMove from the MoveHistory
	// If the Move was successfully undone returns true and
	// removes the ChessMove from MoveHistory, otherwise
	// returns false and leaves MoveHistory untouched
	bool UndoLastMove();

	// Calculates all the pseudo-legal moves for a ChessPiece
	// at the given Position. Pseudo-legal moves include both the
	// fully legal moves and the ones that result in the player
	// being checked.
	TArray<FChessMove> CalculatePseudoLegalMoves(FVector2D Position);

	// Calculates all the fully legal moves for a ChessPiece at
	// the given Position. Fully legal moves are the ones that
	// don't result in the player being checked.
	TArray<FChessMove> CalculateFullyLegalMoves(FVector2D Position);

	// Calculates all the pseudo-legal moves for all ChessPieces
	// with a given Color
	TArray<FChessMove> CalculateAllPseudoLegalMoves(PieceColor Color);

	// Calculates all the fully legal moves for all ChessPieces
	// with a given Color
	TArray<FChessMove> CalculateAllFullyLegalMoves(PieceColor Color);

	// Checks if the given player is in a Check state
	bool CheckCheck(PieceColor Color);

	// Checks of the given player is in a Checkmate state
	// if he is, then the OnWin and OnLose methods are
	// called for the winner and loser players respectively
	bool CheckCheckmate(PieceColor Color);

	// Wrapper for AChessboard::UpdateChessboard()
	void UpdateChessboard();

	// ===== CHESSPIECE OPERATIONS ===== //

	// Returns a pointer to the ChessPiece at the specified Position,
	// Returns nullptr if there is no ChessPiece at that Position
	// (Wrapper of AChessboard::GetChessPieceAt)
	AChessPiece* GetChessPieceAt(FVector2D Position);

	/*
	// Returns a const pointer to a ChessPiece at the specified Position,
	// Returns nullptr if there is no ChessPiece at that Position
	AChessPiece const* GetConstChessPieceAt(FVector2D Position) const;

	// Removes all ChessPieces from the Chessboard
	void RemoveAllChessPieces();
	*/
	
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
	AIndicator* AddNewIndicator(FChessMove Move);

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

	// Array of Indicators that will show all possible moves
	// for the currently selected ChessPiece
	UPROPERTY(Transient)
	TArray<AIndicator*> Indicators;

	// History of moves
	TArray<FChessMove> MovesHistory;

	// Subclasses
	// Chessboard
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;
	// Indicators
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIndicator> MoveIndicatorClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIndicator> CaptureIndicatorClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIndicator> PromoteIndicatorClass;

	// Utility function to map enum parameters to TSubclasses
	TSubclassOf<AIndicator> IndicatorTypeToClass(MoveType Type) const;

	// Recursively explores a line of Tiles starting from Position
	// along Direction, and stops either when an obstacle is found
	// or when MaxLength Tiles have been explored. The Tiles' positions
	// are all appended to Moves. If CanCapture is true, a Tile that
	// contains a ChessPiece owned by the opponent will count as a
	// legal move (capturing).
	void ExploreDirection(FVector2D Position, FVector2D RootPosition,
		FVector2D Direction, uint32 MaxLength, bool CanCapture,
		TArray<FChessMove>* Moves, PieceColor PlayerColor);

	// ===== CHESSPIECE OPERATIONS ===== //
	/*
	// Puts a new ChessPiece of the given PieceType and
	// PieceColor at the given Position, returning a pointer
	// to the new ChessPiece that was placed, or nullptr if
	// there was already a ChessPiece in that position.
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	AChessPiece* PutChessPiece(PieceType type,
		PieceColor color, FVector2D Position);
	*/
	/*
	// Removes the ChessPiece at the given Position from
	// the Chessboard and Destroys it. Does nothing if
	// there's no ChessPiece at the given Position.
	// Returns true if the ChessPiece was Destroyed
	// successfully or if there was no ChessPiece at the
	// given Position, otherwise returns false.
	bool RemoveChessPiece(FVector2D Position);
	*/
	/*
	// Moves a ChessPiece from one Position to another,
	// returning true if the ChessPiece was moved
	// successfully, returns false if there was no piece
	// in the OldPosition
	// DOES NOT CHECK IF THE MOVE IS LEGAL!!
	bool MoveChessPiece(FVector2D OldPosition,
		FVector2D NewPosition);
	*/
};
