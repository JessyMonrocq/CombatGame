#include "CombatCharacter.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CurrentHealth = 0.0f;
	CurrentDamage = 0.0f;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// --- Public Methods ---

void ACombatCharacter::InitializeCharacter() 
{
	if (!CharacterStats) 
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterStats missing from character script !"));
		return;
	}

	CurrentHealth = CharacterStats->MaxHealth;
	CurrentDamage = CharacterStats->Damage;

	OnHealthChanged.Broadcast(CurrentHealth, CharacterStats->MaxHealth);
}

void ACombatCharacter::ReceiveDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || IsDead()) 
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
	OnHealthChanged.Broadcast(CurrentHealth, CharacterStats->MaxHealth);

	if (IsDead()) 
	{
		Die();
	}
}

bool ACombatCharacter::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

float ACombatCharacter::GetCurrentDamage()
{
	return CurrentDamage;
}

// --- Private Methods ---

void ACombatCharacter::Die() 
{
	UE_LOG(LogTemp, Warning, TEXT("Character %s is dead !"), *this->GetName());
	// Animate Death later...
}
