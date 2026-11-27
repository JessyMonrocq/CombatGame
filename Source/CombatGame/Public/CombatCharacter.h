#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterDataAsset.h"
#include "CombatCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnHealthChanged,
	float, CharacterHealth,
	float, MaxHealth
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
	FOnHealthChanged OnHealthChanged;

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

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsDead() const;
	UFUNCTION()
	float GetCurrentDamage();

protected:
	virtual void BeginPlay() override;

private:
	
	void Die();
};
