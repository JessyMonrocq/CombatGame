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

	UE_LOG(LogTemp, Warning, TEXT("Character's current health : %f"), CurrentHealth);
	UE_LOG(LogTemp, Warning, TEXT("Character's current damage : %f"), CurrentDamage);
}

void ACombatCharacter::ReceiveDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || IsDead()) 
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("Remaining Health : %f"), CurrentHealth);

	if (IsDead()) 
	{
		Die();
	}
}

void ACombatCharacter::Attack(ACombatCharacter* Target) 
{
	if (!IsValid(Target) || Target->IsDead() || IsDead() || Target == this)
	{
		return;
	}

	Target->ReceiveDamage(CurrentDamage);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("%s attacks %s for %.1f damage!"), *this->GetActorNameOrLabel(), *Target->GetActorNameOrLabel(), CurrentDamage));
	OnCharacterAttack.Broadcast(*this->GetActorNameOrLabel(), *Target->GetActorNameOrLabel(), CurrentDamage);
}

bool ACombatCharacter::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

// --- Private Methods ---

void ACombatCharacter::Die() 
{
	UE_LOG(LogTemp, Warning, TEXT("Character %s is dead !"), *this->GetName());
	// Animate Death later...
}
