// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreen.h"

UGameOverScreen::UGameOverScreen(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UGameOverScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameOverScreen::SetGameOverTextWin()
{
	GameOverText->SetText(FText::FromString("You won!"));
}

void UGameOverScreen::SetGameOverTextLose()
{
	GameOverText->SetText(FText::FromString("You lost!"));
}

void UGameOverScreen::SetGameOverTextDraw()
{
	GameOverText->SetText(FText::FromString("Draw"));
}
