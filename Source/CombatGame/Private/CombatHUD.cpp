#include "CombatHUD.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

// Delay macro from @AGuappo
#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}

void UCombatHUD::NativeConstruct()
{
    Super::NativeConstruct();

    AttackButton->SetVisibility(ESlateVisibility::Hidden);
    CombatInfosTextBlock->SetVisibility(ESlateVisibility::Hidden);

    if (!IsDesignTime()) 
    {
        CombatManagerRef = Cast<ACombatManager>(UGameplayStatics::GetActorOfClass(this, ACombatManager::StaticClass()));
    }

    if (IsValid(CombatManagerRef)) 
    {
        CombatManagerRef->OnPlayerTurn.AddDynamic(this, &UCombatHUD::DisplayAttackButton);

        CombatManagerRef->OnCombatStateChanged.AddDynamic(this, &UCombatHUD::DisplayCombatState);

        CombatManagerRef->OnCombatInfos.AddDynamic(this, &UCombatHUD::DisplayCombatInfos);

        CombatManagerRef->PlayerCharacter->OnHealthChanged.AddDynamic(this, &UCombatHUD::UpdatePlayerHealth);

        CombatManagerRef->EnemyCharacter->OnHealthChanged.AddDynamic(this, &UCombatHUD::UpdateEnemyHealth);
    }
}

void UCombatHUD::NativeDestruct()
{
    if (IsValid(CombatManagerRef))
    {
        CombatManagerRef->OnPlayerTurn.RemoveDynamic(this, &UCombatHUD::DisplayAttackButton);

        CombatManagerRef->OnCombatStateChanged.RemoveDynamic(this, &UCombatHUD::DisplayCombatState);

        CombatManagerRef->OnCombatInfos.RemoveDynamic(this, &UCombatHUD::DisplayCombatInfos);

        CombatManagerRef->PlayerCharacter->OnHealthChanged.RemoveDynamic(this, &UCombatHUD::UpdatePlayerHealth);

        CombatManagerRef->EnemyCharacter->OnHealthChanged.RemoveDynamic(this, &UCombatHUD::UpdateEnemyHealth);
    }

    Super::NativeDestruct();
}

// --- Public Methods ---

void UCombatHUD::DisplayAttackButton(bool Display) 
{
    AttackButton->SetVisibility(Display ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCombatHUD::DisplayCombatState(const FText CombatState)
{
    CombatStateTextBlock->SetText(CombatState);
}

void UCombatHUD::DisplayCombatInfos(const FText CombatInfosText)
{
    if (CombatInfosTextBlock->GetVisibility() == ESlateVisibility::Hidden)
    {
        CombatInfosTextBlock->SetVisibility(ESlateVisibility::Visible);
    }

    CombatInfosTextBlock->SetText(CombatInfosText);

    DELAY(CombatInfosTime, { CombatInfosTextBlock->SetVisibility(ESlateVisibility::Hidden); });
}

// --- Private Methods ---

void UCombatHUD::UpdatePlayerHealth(float CurrentHealth, float MaxHealth)
{
    PlayerHealthBar->SetPercent(CurrentHealth / MaxHealth);
}

void UCombatHUD::UpdateEnemyHealth(float CurrentHealth, float MaxHealth)
{
    EnemyHealthBar->SetPercent(CurrentHealth / MaxHealth);
}