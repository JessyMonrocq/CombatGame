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
	FOnCombatEnded,
	ECombatState,
	FinalState
);

UCLASS()
class COMBATGAME_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
// Properties
public:
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCombatEnded OnCombatEnded;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ACombatCharacter> PlayerCharacter;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ACombatCharacter> EnemyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatState CombatState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatTurn CurrentTurn;

// Methods
public:	
	ACombatManager();

	void InitializeCombat();

protected:
	virtual void BeginPlay() override;
};
