// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "PlayerAbilitySystemComponent.h"
#include "GeneralStatsAttributeSet.h"
#include "HitableActor.generated.h"

class UPlayerAbilitySystemComponent;
class UGeneralStatsAttributeSet;
class UStaticMeshComponent;

UCLASS()
class GAS_EXERCICE_API AHitableActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitableActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeneralStatsAttributeSet* GenStatsAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> InitStatsEffect;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
