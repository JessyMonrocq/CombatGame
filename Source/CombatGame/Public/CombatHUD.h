#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatManager.h"
#include "CombatHUD.generated.h"

class UButton;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class COMBATGAME_API UCombatHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AttackButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CombatStateText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CombatInfosText;

	UPROPERTY()
	TObjectPtr<ACombatManager> CombatManagerRef;

	const float CombatInfosTime = 1.0f;

public:

	UFUNCTION()
	void DisplayAttackButton(bool Display);

	UFUNCTION()
	void DisplayCombatState(const FText CombatState);

	UFUNCTION()
	void DisplayCombatInfos(const FString Character, const FString Target, const float Damage);
};
