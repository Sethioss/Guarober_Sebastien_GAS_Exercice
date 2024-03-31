// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_ExerciceGameMode.h"
#include "GAS_ExerciceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_ExerciceGameMode::AGAS_ExerciceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
