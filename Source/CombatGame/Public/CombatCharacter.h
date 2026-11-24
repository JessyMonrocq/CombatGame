#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterDataAsset.h"
#include "CombatCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnCharacterAttack,
	FString, Character,
	FString, Target,
	float, Damage
);

UCLASS()
class COMBATGAME_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

// --- Properties ---
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Stats")
	TObjectPtr<UCharacterDataAsset> CharacterStats;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterAttack OnCharacterAttack;

private:

	UPROPERTY(VisibleAnywhere, Category = "Character Stats")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, Category = "Character Stats")
	float CurrentDamage;

// --- Methods ---
public:
	ACombatCharacter();

	void InitializeCharacter();
	void ReceiveDamage(float DamageAmount);
	void Attack(ACombatCharacter* Target);

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

private:
	
	void Die();
};
