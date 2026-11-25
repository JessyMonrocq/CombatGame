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
	if (IsValid(PlayerCharacter) && IsValid(EnemyCharacter))
	{
		PlayerCharacter->OnCharacterAction.RemoveDynamic(this, &ACombatManager::ForwardCombatInfos);
		EnemyCharacter->OnCharacterAction.RemoveDynamic(this, &ACombatManager::ForwardCombatInfos);
	}

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

	PlayerCharacter->OnCharacterAction.AddDynamic(this, &ACombatManager::ForwardCombatInfos);
	EnemyCharacter->OnCharacterAction.AddDynamic(this, &ACombatManager::ForwardCombatInfos);

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
	if (CombatState != ECombatState::InProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat not in Progress!"));
		return;
	}

	if (CombatPhase != ECombatPhase::WaitingForAction || CurrentTurn != ECombatTurn::PlayerTurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not player's turn OR not awaiting action yet!"));
		return;
	}

	PlayerCharacter->Attack(EnemyCharacter);

	CheckCombatEnd();

	if (CombatState == ECombatState::InProgress)
	{
		EndTurn();
		// TODO : Handle Enemy's turn in different script
		DELAY(StartingDelay, { EnemyAttack(); });
	}
}

void ACombatManager::EnemyAttack()
{
	if (CombatState != ECombatState::InProgress || CurrentTurn != ECombatTurn::EnemyTurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat not in Progress OR not enemy's turn!"));
		return;
	}

	EnemyCharacter->Attack(PlayerCharacter);

	CheckCombatEnd();

	if (CombatState == ECombatState::InProgress)
	{
		EndTurn();
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

void ACombatManager::ForwardCombatInfos(const FText CombatInfosText)
{
	OnCombatInfos.Broadcast(CombatInfosText);
}
