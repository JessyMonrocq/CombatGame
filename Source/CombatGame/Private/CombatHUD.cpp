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
    CombatInfosText->SetVisibility(ESlateVisibility::Hidden);

    CombatManagerRef = Cast<ACombatManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACombatManager::StaticClass()));

    if (IsValid(CombatManagerRef)) 
    {
        CombatManagerRef->OnPlayerTurn.AddDynamic(this, &UCombatHUD::DisplayAttackButton);

        CombatManagerRef->OnCombatStateChanged.AddDynamic(this, &UCombatHUD::DisplayCombatState);

        CombatManagerRef->OnCombatInfos.AddDynamic(this, &UCombatHUD::DisplayCombatInfos);
    }
}

void UCombatHUD::DisplayAttackButton(bool Display) 
{
    AttackButton->SetVisibility(Display ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCombatHUD::DisplayCombatState(const FText CombatState)
{
    CombatStateText->SetText(CombatState);
}

void UCombatHUD::DisplayCombatInfos(const FString Character, const FString Target, const float Damage)
{
    if (CombatInfosText->GetVisibility() == ESlateVisibility::Hidden)
    {
        CombatInfosText->SetVisibility(ESlateVisibility::Visible);
    }

    FString msg = FString::Printf(TEXT("%s attacks %s for %.1f damage points!"), *Character, *Target, Damage);
    CombatInfosText->SetText(FText::FromString(msg));

    DELAY(CombatInfosTime, { CombatInfosText->SetVisibility(ESlateVisibility::Hidden); });
}
