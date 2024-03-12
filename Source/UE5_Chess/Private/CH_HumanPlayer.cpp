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
	IsMyTurn = true;
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your turn"));
	// GameInstance->SetTurnMessage(TEXT("Your turn"));
}

void ACH_HumanPlayer::OnWin()
{
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You won!"));
	// GameInstance->SetTurnMessage(TEXT("You won!"));
	// GameInstance->IncrementScoreHumanPlayer();
}

void ACH_HumanPlayer::OnLose()
{
	// TODO: Remove this
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You lost!"));
	// GameInstance->SetTurnMessage(TEXT("You lost!"));
	// GameInstance->IncrementScoreHumanPlayer();
}

void ACH_HumanPlayer::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && IsMyTurn) {
		// Get hit position expressed in grid coordinates
		ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
		double TileSize = GameMode->GetTileSize();
		FVector2D HitGridPos = {
			FMath::Floor(Hit.Location[0] / TileSize),
			FMath::Floor(Hit.Location[1] / TileSize)
		};

		// If an Indicator was clicked, the move gets executed and the turn ends,
		// then the currently selected ChessPiece is deselected and all Indicators
		// are removed
		const AIndicator* HitIndicator = GameMode->GetIndicatorForEndPos(HitGridPos);
		if (HitIndicator != nullptr)
		{
			GameMode->DoMove(HitIndicator);
			SelectedPosition = DESELECTED;
			GameMode->RemoveIndicators();
			IsMyTurn = false;
		}

		// If an empty Tile or a BLACK ChessPiece was clicked, deselect currently
		// selected ChessPiece and remove all Indicators
		const AChessPiece* HitPiece = GameMode->GetChessPieceAt(HitGridPos);
		if (HitPiece == nullptr || HitPiece->GetColor() == PieceColor::PBLACK)
		{
			SelectedPosition = DESELECTED;
			GameMode->RemoveIndicators();
		}
		// When clicking on a White ChessPiece, it becomes the currently selected ChessPiece
		// and the possible moves are calculated and shown
		if (HitPiece->GetColor() == PieceColor::PWHITE)
		{
			SelectedPosition = HitGridPos;
			GameMode->ShowLegalMoves(SelectedPosition);
		}
	}

}

