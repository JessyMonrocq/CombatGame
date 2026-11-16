#include "CombatManager.h"

ACombatManager::ACombatManager()
{
	PrimaryActorTick.bCanEverTick = false;

	CombatState = ECombatState::NotStarted;
	CurrentTurn = ECombatTurn::PlayerTurn;
}

void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCombat();
}

void ACombatManager::InitializeCombat() 
{
	if (!IsValid(PlayerCharacter) || !IsValid(EnemyCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player OR Enemy character missing from Combat Manager!"));
		return;
	}

	if (PlayerCharacter->IsDead() || EnemyCharacter->IsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player OR Enemy character dead before combat start!"));
		return;
	}

	CombatState = ECombatState::InProgress;
	CurrentTurn = ECombatTurn::PlayerTurn;

	UE_LOG(LogTemp, Warning, TEXT("Combat started !"));
}
