#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGAME_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float Damage;
};
