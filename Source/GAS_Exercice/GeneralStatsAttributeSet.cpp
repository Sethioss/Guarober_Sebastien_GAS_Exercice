// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralStatsAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Math/UnrealMathUtility.h"

void UGeneralStatsAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGeneralStatsAttributeSet, Health, OldHealth);
}

void UGeneralStatsAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGeneralStatsAttributeSet, MaxHealth, OldMaxHealth);
}

void UGeneralStatsAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGeneralStatsAttributeSet, Attack, OldAttack);
}

void UGeneralStatsAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGeneralStatsAttributeSet, Defense, OldDefense);
}

void UGeneralStatsAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGeneralStatsAttributeSet, Mana, OldMana);
}

void UGeneralStatsAttributeSet::OnRep_HealingPower(const FGameplayAttributeData& OldHealingPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGeneralStatsAttributeSet, HealingPower, OldHealingPower);
}

void UGeneralStatsAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGeneralStatsAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGeneralStatsAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGeneralStatsAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGeneralStatsAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGeneralStatsAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGeneralStatsAttributeSet, HealingPower, COND_None, REPNOTIFY_Always);
}

void UGeneralStatsAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
}
