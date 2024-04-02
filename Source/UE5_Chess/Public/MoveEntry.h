// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Indicator.h"
#include "MoveEntry.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE5_CHESS_API UMoveEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	UMoveEntry(const FObjectInitializer& ObjectInitializer);
	void SetWhiteMove(FText MoveText);
	void SetBlackMove(FText MoveText);
	void SetMoveIndex(uint32 Index);

protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveIndex = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveWhiteText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveBlackText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> MoveWhiteButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> MoveBlackButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> BackgroundColor = nullptr;

};
