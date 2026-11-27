#include "CombatManager.h"

// Delay macro from @AGuappo
#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}

ACombatManager::ACombatManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACombatManager::BeginPlay()
{
	Super::BeginPlay();

	SetCombatState(ECombatState::NotStarted);

	DELAY(StartingDelay, {
		InitializeCombat();
	});
}

void ACombatManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

// --- Public Methods ---

void ACombatManager::InitializeCombat()
{
	if (!IsValid(PlayerCharacter) || !IsValid(EnemyCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player OR Enemy character missing from Combat Manager!"));
		return;
	}

	PlayerCharacter->InitializeCharacter();
	EnemyCharacter->InitializeCharacter();

	if (PlayerCharacter->IsDead() || EnemyCharacter->IsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player OR Enemy character dead before combat start!"));
		return;
	}

	SetCombatState(ECombatState::InProgress);
	CombatPhase = ECombatPhase::WaitingForAction;

	CurrentTurn = ECombatTurn::PlayerTurn;
	OnPlayerTurn.Broadcast(CurrentTurn == ECombatTurn::PlayerTurn);
}

void ACombatManager::PlayerAttack()
{
	AttackTarget(PlayerCharacter, EnemyCharacter);
}

void ACombatManager::AttackTarget(ACombatCharacter* Character, ACombatCharacter* Target) 
{
	if (CombatState != ECombatState::InProgress || CombatPhase != ECombatPhase::WaitingForAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat not in Progress or Awaiting action!"));
		return;
	}

	if (!IsValid(Character) || !IsValid(Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Character OR Target not valid references!"));
		return;
	}

	FText AttackLogText = FText::Format(
		NSLOCTEXT("Combat", "AttackLog", "{0} attacks {1} for {2} damage!"),
		FText::FromString(Character->GetActorNameOrLabel()),
		FText::FromString(Target->GetActorNameOrLabel()),
		FText::AsNumber(Character->GetCurrentDamage())
	);
	OnCombatInfos.Broadcast(AttackLogText);

	Target->ReceiveDamage(Character->GetCurrentDamage());

	CheckCombatEnd();

	if (CombatState == ECombatState::InProgress)
	{
		EndTurn();

		if (CurrentTurn == ECombatTurn::EnemyTurn)
		{
			// TODO : Handle Enemy's turn in different script
			DELAY(StartingDelay, { AttackTarget(EnemyCharacter, PlayerCharacter); });
		}
	}
}

void ACombatManager::EndTurn()
{
	CurrentTurn = (CurrentTurn == ECombatTurn::PlayerTurn) ? ECombatTurn::EnemyTurn : ECombatTurn::PlayerTurn;

	OnPlayerTurn.Broadcast(CurrentTurn == ECombatTurn::PlayerTurn);
}

// --- Private Methods --- 

void ACombatManager::SetCombatState(ECombatState NewState) 
{
	CombatState = NewState;

	const FText& msg = UEnum::GetDisplayValueAsText(CombatState);
	OnCombatStateChanged.Broadcast(msg);
}

void ACombatManager::HandleCombatPhases() 
{
	if (CombatState != ECombatState::InProgress || CombatPhase == ECombatPhase::WaitingForAction) 
	{
		return;
	}

	// Display Attack message
	// Delay
	// Display Health bar change
	// Delay
	// Display next character turn
	// Delay
	// Transition Turn
}

void ACombatManager::CheckCombatEnd()
{
	if (EnemyCharacter->IsDead())
	{
		EndCombat(ECombatState::Victory);
		return;
	}

	if (PlayerCharacter->IsDead())
	{
		EndCombat(ECombatState::Defeat);
		return;
	}
}

void ACombatManager::EndCombat(ECombatState FinalState)
{
	if (CombatState != ECombatState::InProgress)
	{
		return;
	}

	SetCombatState(FinalState);
}
