// Fill out your copyright notice in the Description page of Project Settings.


#include "HitableActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHitableActor::AHitableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GenStatsAttributeSet = CreateDefaultSubobject<UGeneralStatsAttributeSet>(TEXT("CharacterAttributes"));
	ASC = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("AbilitySystem Component"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AHitableActor::BeginPlay()
{
	Super::BeginPlay();

	ASC->BP_ApplyGameplayEffectToSelf(InitStatsEffect, 1, ASC->MakeEffectContext());
	
}

// Called every frame
void AHitableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AHitableActor::GetAbilitySystemComponent() const
{
	return ASC;
}

