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
	virtual void NativeDestruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AttackButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CombatStateTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CombatInfosTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerHealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnemyHealthBar;

	UPROPERTY()
	TObjectPtr<ACombatManager> CombatManagerRef;

	const float CombatInfosTime = 1.0f;

public:

	UFUNCTION()
	void DisplayAttackButton(bool Display);

	UFUNCTION()
	void DisplayCombatState(const FText CombatState);

	UFUNCTION()
	void DisplayCombatInfos(const FText CombatInfosText);

private:

	UFUNCTION()
	void UpdatePlayerHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	void UpdateEnemyHealth(float CurrentHealth, float MaxHealth);
};
