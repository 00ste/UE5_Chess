// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH_PlayerInterface.h"
#include "PlayersList.h"
#include "ChessPiece.h"
#include "Chessboard.h"
#include "Indicator.h"
#include "CH_WidgetManager.h"
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
	// Sets default values for this actor's properties
	ACH_GameMode();

	// TODO: Making this public might not be the best thing
	// Widget Manager
	ACH_WidgetManager* WidgetManager;
	
	// Returns the TileSize
	double GetTileSize() const;
	
	// Prepares the Chessboard by placing each ChessPiece
	// in the correct Position
	void PrepareChessboard();

	// Changes the CurrentPlayer index to the next player
	// and calls that player's OnTurn() method
	void TurnNextPlayer();

	// Executes the Move given by changing the record in
	// ChessPieceMap without updating the Chessboard.
	void DoMove(FChessMove Move);

	// Executes the Move given by changing the record in
	// ChessPieceMap, adding the SAN of the move in the
	// MovesHistory widget and updating the Chessboard.
	void DoFinalMove(FChessMove Move);

	// Returns a FString representing the representation of the
	// given Move in Standard Chess Notation (SAN).
	FString GenerateSANForMove(FChessMove Move);

	// Returns true if the Move results in a non-Check state,
	// returns false otherwise
	bool DoesMoveUncheck(FChessMove Move);

	// Undoes the last ChessMove from the MoveHistory
	// If the Move was successfully undone returns true and
	// removes the ChessMove from MoveHistory, otherwise
	// returns false and leaves MoveHistory untouched
	void UndoLastMove();

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

	// Converts incomplete promotion moves (with bDoesPromote = true)
	// and PromotionTarget = PieceType::PTNONE to all possible complete
	// promotion moves from the given TArray of FChessMoves. The promotion
	// candidates are only QUEEN and KNIGHT to increase efficiency
	TArray<FChessMove> ExpandPromotionMoves(TArray<FChessMove> Moves);

	// Checks if the given player is in a Check state
	bool CheckCheck(PieceColor Color);

	// Checks of the given player is in a Checkmate state
	// if he is, then the OnWin and OnLose methods are
	// called for the winner and loser players respectively
	bool CheckCheckmate(PieceColor Color);

	// Checks if the current board is a draw, which happens after 75
	// moves since the last pawn move or capture
	bool CheckDraw();

	// Wrapper for AChessboard::UpdateChessboard()
	void UpdateChessboard();

	// Puts the current game state to the one at the given
	// number of ChessMoves from the start of the game. The game
	// state includes the position of all ChessPieces on the
	// Chessboard as well as the CurrentPlayer index.
	UFUNCTION()
	void OnHistoryClicked(uint32 MovesFromGameStart);

	// Returns a pointer to the ChessPiece at the specified Position,
	// Returns nullptr if there is no ChessPiece at that Position
	// (Wrapper of AChessboard::GetChessPieceAt)
	AChessPiece* GetChessPieceAt(FVector2D Position);
	
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
	uint32 MovesSinceLastCapture;

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
	// Widget Manager
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACH_WidgetManager> WidgetManagerClass;


	// Utility function to map FChessMoves to TSubclasses based on their type
	TSubclassOf<AIndicator> IndicatorTypeToClass(FChessMove Move) const;

	// Recursively explores a line of Tiles starting from Position
	// along Direction, and stops either when an obstacle is found
	// or when MaxLength Tiles have been explored. The Tiles' positions
	// are all appended to Moves. If CanCapture is true, a Tile that
	// contains a ChessPiece owned by the opponent will count as a
	// legal move (capturing).
	void ExploreDirection(FVector2D Position, FVector2D RootPosition,
		FVector2D Direction, uint32 MaxLength, bool CanCapture,
		TArray<FChessMove>* Moves, PieceColor PlayerColor);
};
