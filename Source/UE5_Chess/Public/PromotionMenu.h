// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PromotionMenu.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API UPromotionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPromotionMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> QueenButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> KnightButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> BishopButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> RookButton = nullptr;

	UFUNCTION(BlueprintCallable)
	void OnQueenSelected();
	UFUNCTION(BlueprintCallable)
	void OnKnightSelected();
	UFUNCTION(BlueprintCallable)
	void OnBishopSelected();
	UFUNCTION(BlueprintCallable)
	void OnRookSelected();
};
