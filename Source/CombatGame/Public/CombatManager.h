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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnCombatInfos,
	FString, Character,
	FString, Target,
	float, Damage
);


UCLASS()
class COMBATGAME_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
// --- Properties ---
public:

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCombatStateChanged OnCombatStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnPlayerTurn OnPlayerTurn;

	UPROPERTY(BlueprintAssignable)
	FOnCombatInfos OnCombatInfos;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ACombatCharacter> PlayerCharacter;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ACombatCharacter> EnemyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatState CombatState = ECombatState::NotStarted;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatTurn CurrentTurn = ECombatTurn::PlayerTurn;

private:

	FTimerHandle StartingDelayTimer;

	const float StartingDelay = 2.0f;

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

private:
	void SetCombatState(ECombatState NewState);
	void CheckCombatEnd();
	void EndCombat(ECombatState FinalState);

	UFUNCTION()
	void ForwardCombatInfos(const FString Character, const FString Target, float Damage);
};
