// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_HumanPlayer.h"
#include "CH_GameMode.h"

// Sets default values
ACH_HumanPlayer::ACH_HumanPlayer()
{
 	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(Camera);

	SelectedPosition = DESELECTED;
	// PlayerNumber = -1;

}

// Called when the game starts or when spawned
void ACH_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACH_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACH_HumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACH_HumanPlayer::OnTurn()
{
	bIsMyTurn = true;
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Your turn"));
	// GameInstance->SetTurnMessage(TEXT("Your turn"));
}

void ACH_HumanPlayer::OnWin()
{
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("You won!"));
	// GameInstance->SetTurnMessage(TEXT("You won!"));
	// GameInstance->IncrementScoreHumanPlayer();
	WidgetManager->ShowGameOverScreen();
	WidgetManager->GetGameOverScreen()->SetGameOverTextWin();
}

void ACH_HumanPlayer::OnLose()
{
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("You lost!"));
	// GameInstance->SetTurnMessage(TEXT("You lost!"));
	// GameInstance->IncrementScoreHumanPlayer();
	WidgetManager->ShowGameOverScreen();
	WidgetManager->GetGameOverScreen()->SetGameOverTextLose();
}

void ACH_HumanPlayer::OnDraw()
{
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Draw!"));
	// GameInstance->SetTurnMessage(TEXT("Draw!"));
	// GameInstance->IncrementScoreHumanPlayer();
	WidgetManager->ShowGameOverScreen();
	WidgetManager->GetGameOverScreen()->SetGameOverTextDraw();
}

void ACH_HumanPlayer::SetWidgetManager(ACH_WidgetManager* WidgetManager_)
{
	this->WidgetManager = WidgetManager_;
}

void ACH_HumanPlayer::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && bIsMyTurn) {
		// Get hit position expressed in grid coordinates
		ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());

		double TileSize = GameMode->GetTileSize();
		FVector2D HitGridPos = {
			FMath::Floor(Hit.ImpactPoint[0] / TileSize),
			FMath::Floor(Hit.ImpactPoint[1] / TileSize)
		};

		// If an Indicator was clicked, the move gets executed and the turn ends,
		// then the currently selected ChessPiece is deselected and all Indicators
		// are removed
		const AIndicator* HitIndicator = GameMode->GetIndicatorForEndPos(HitGridPos);
		if (HitIndicator != nullptr)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Move selected!"));
			SelectedMove = HitIndicator->Move;

			// If the ChessMove does a promotion and no promotion target was set,
			// show a menu to allow the player to select which ChessPiece to promote to
			if (SelectedMove.bDoesPromote && SelectedMove.PromotionTarget == PieceType::PTNONE)
			{
				// Disable clicking after spawning the menu
				bIsMyTurn = false;
				WidgetManager->ShowPromotionMenu();
			}
			else
			{
				GameMode->DoFinalMove(SelectedMove);
				SelectedPosition = DESELECTED;
				GameMode->RemoveAllIndicators();
				bIsMyTurn = false;

				GameMode->UpdateChessboard();
				GameMode->TurnNextPlayer();
			}

			return;
		}

		// If an empty Tile or a BLACK ChessPiece was clicked, deselect currently
		// selected ChessPiece and remove all Indicators
 		const AChessPiece* HitPiece = GameMode->GetChessPieceAt(HitGridPos);
		if (HitPiece == nullptr || HitPiece->GetColor() != OwnedColor)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No actions here!"));
			SelectedPosition = DESELECTED;
			GameMode->RemoveAllIndicators();

			return;
		}
		// When clicking on a White ChessPiece, it becomes the currently selected ChessPiece
		// and the possible moves are calculated and shown
		if (HitPiece->GetColor() == OwnedColor)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Calculating moves!"));
			GameMode->RemoveAllIndicators();
			SelectedPosition = HitGridPos;
			for (FChessMove Move : GameMode->CalculateFullyLegalMoves(SelectedPosition))
			{
				GameMode->AddNewIndicator(Move);
			}
			return;
		}
	}

}

void ACH_HumanPlayer::OnQueenSelected()
{
	WidgetManager->HidePromotionMenu();
	if (SelectedMove.bDoesPromote)
		SelectedMove.PromotionTarget = PieceType::QUEEN;

	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DoFinalMove(SelectedMove);
	SelectedPosition = DESELECTED;
	GameMode->RemoveAllIndicators();
	bIsMyTurn = false;

	GameMode->UpdateChessboard();
	GameMode->TurnNextPlayer();
}

void ACH_HumanPlayer::OnKnightSelected()
{
	WidgetManager->HidePromotionMenu();
	if (SelectedMove.bDoesPromote)
		SelectedMove.PromotionTarget = PieceType::KNIGHT;

	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DoFinalMove(SelectedMove);
	SelectedPosition = DESELECTED;
	GameMode->RemoveAllIndicators();
	bIsMyTurn = false;

	GameMode->UpdateChessboard();
	GameMode->TurnNextPlayer();
}

void ACH_HumanPlayer::OnBishopSelected()
{
	WidgetManager->HidePromotionMenu();
	if (SelectedMove.bDoesPromote)
		SelectedMove.PromotionTarget = PieceType::BISHOP;

	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DoFinalMove(SelectedMove);
	SelectedPosition = DESELECTED;
	GameMode->RemoveAllIndicators();
	bIsMyTurn = false;

	GameMode->UpdateChessboard();
	GameMode->TurnNextPlayer();
}

void ACH_HumanPlayer::OnRookSelected()
{
	WidgetManager->HidePromotionMenu();
	if (SelectedMove.bDoesPromote)
		SelectedMove.PromotionTarget = PieceType::ROOK;

	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DoFinalMove(SelectedMove);
	SelectedPosition = DESELECTED;
	GameMode->RemoveAllIndicators();
	bIsMyTurn = false;

	GameMode->UpdateChessboard();
	GameMode->TurnNextPlayer();
}

