// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_HumanPlayer.h"

// Sets default values
ACH_HumanPlayer::ACH_HumanPlayer()
{
 	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(Camera);
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
		// TODO: write onclick routine
	}

}

