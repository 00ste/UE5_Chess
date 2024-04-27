// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameOverScreen.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API UGameOverScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UGameOverScreen(const FObjectInitializer& ObjectInitializer);
	void SetGameOverTextWin();
	void SetGameOverTextLose();
	void SetGameOverTextDraw();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> GameOverText = nullptr;
};
