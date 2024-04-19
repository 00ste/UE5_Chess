// Fill out your copyright notice in the Description page of Project Settings.


#include "PromotionMenu.h"
#include "CH_HumanPlayer.h"

UPromotionMenu::UPromotionMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UPromotionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	QueenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQueenSelected);
	KnightButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnKnightSelected);
	BishopButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBishopSelected);
	RookButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnRookSelected);
}

void UPromotionMenu::OnQueenSelected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Queen selected for promotion!"));
	Cast<ACH_HumanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->OnQueenSelected();
}
void UPromotionMenu::OnKnightSelected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Knight selected for promotion!"));
	Cast<ACH_HumanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->OnKnightSelected();
}
void UPromotionMenu::OnBishopSelected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Bishop selected for promotion!"));
	Cast<ACH_HumanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->OnBishopSelected();
}
void UPromotionMenu::OnRookSelected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Rook selected for promotion!"));
	Cast<ACH_HumanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->OnRookSelected();
}