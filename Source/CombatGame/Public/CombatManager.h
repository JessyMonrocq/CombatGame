#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatCharacter.h"
#include "CombatManager.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	NotStarted,
	InProgress,
	Victory,
	Defeat
};

UENUM(BlueprintType)
enum class ECombatPhase : uint8
{
	WaitingForAction,
	DisplayMessage,
	ExecutingAction,
	TurnTransition
};

// TODO : Replace enum by a List of Character's play order !
UENUM()
enum class ECombatTurn : uint8
{
	PlayerTurn,
	EnemyTurn
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCombatStateChanged,
	FText,
	CombatState
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPlayerTurn,
	bool,
	PlayerTurn
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCombatInfos,
	FText,
	CombatInfosText
);

UCLASS()
class COMBATGAME_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
// --- Properties ---
public:

	UPROPERTY()
	FOnCombatStateChanged OnCombatStateChanged;

	UPROPERTY()
	FOnPlayerTurn OnPlayerTurn;

	UPROPERTY()
	FOnCombatInfos OnCombatInfos;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ACombatCharacter> PlayerCharacter;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ACombatCharacter> EnemyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatState CombatState = ECombatState::NotStarted;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatPhase CombatPhase = ECombatPhase::TurnTransition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatTurn CurrentTurn = ECombatTurn::PlayerTurn;

private:

	const float StartingDelay = 2.5f;
	const float CombatPhaseDelay = 1.5f;

// --- Methods ---
public:	
	ACombatManager();

	void InitializeCombat();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PlayerAttack();
	void EnemyAttack();
	void EndTurn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SetCombatState(ECombatState NewState);
	void HandleCombatPhases();
	void CheckCombatEnd();
	void EndCombat(ECombatState FinalState);

	UFUNCTION()
	void ForwardCombatInfos(const FText CombatInfosText);
};
