// Copyright Epic Games, Inc. All Rights Reserved.


#include "GAS_ExercicePlayerController.h"
#include "EnhancedInputSubsystems.h"

void AGAS_ExercicePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
		Subsystem->AddMappingContext(FireMappingContext, 1);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
}