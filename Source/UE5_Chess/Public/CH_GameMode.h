// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH_PlayerInterface.h"
#include "ChessPiece.h"
#include "Chessboard.h"
#include "CH_GameMode.generated.h"

UENUM()

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
		
	// Map that stores the ChessPieces on the Chessboard
	UPROPERTY(Transient)
	TMap<FVector2D, AChessPiece*> ChessPieceMap;

	// Chessboard subclass
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;

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
