// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_ExerciceCharacter.h"
#include "GAS_ExerciceProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerAbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "GeneralStatsAttributeSet.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGAS_ExerciceCharacter

AGAS_ExerciceCharacter::AGAS_ExerciceCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GenStatsAttributeSet = CreateDefaultSubobject<UGeneralStatsAttributeSet>(TEXT("CharacterAttributes"));
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	ASC = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("AbilitySystem Component"));
	ASC->SetIsReplicated(true);

}

float AGAS_ExerciceCharacter::GetAttackStat()
{
	return GenStatsAttributeSet->GetAttackAttribute().GetNumericValue(GenStatsAttributeSet);
}

UAbilitySystemComponent* AGAS_ExerciceCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AGAS_ExerciceCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}
	}

	if (ASC)
	{
		if (HasAuthority() && ShootingAbility)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(ShootingAbility.GetDefaultObject()));
		}
		ASC->InitAbilityActorInfo(this, this);
	}
	ASC->BP_ApplyGameplayEffectToSelf(InitStatsEffect, 1, ASC->MakeEffectContext());

}

//////////////////////////////////////////////////////////////////////////// Input

void AGAS_ExerciceCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_ExerciceCharacter::Move);
		// Scrolling Weapons
		EnhancedInputComponent->BindAction(ScrollWeaponAction, ETriggerEvent::Triggered, this, &AGAS_ExerciceCharacter::ScrollWeapon);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_ExerciceCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGAS_ExerciceCharacter::Fire);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGAS_ExerciceCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGAS_ExerciceCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGAS_ExerciceCharacter::Fire()
{
	if (!Weapons.IsEmpty())
	{
		if (Weapons[CurrentWeaponID])
		{
			if (ShootingAbility && Weapons[CurrentWeaponID]->OnHitEffect)
			{
				ASC->TryActivateAbilityByClass(ShootingAbility);
			}
			Weapons[CurrentWeaponID]->Fire();
		}
	}
}

void AGAS_ExerciceCharacter::ScrollWeapon(const FInputActionValue& Value)
{
	if (Weapons.Num() > 1)
	{
		float ScrollValue = Value.Get<float>();
		UE_LOG(LogTemp, Warning, TEXT("%i"), ScrollValue);
		CurrentWeaponID = (CurrentWeaponID + (int)ScrollValue) % Weapons.Num();
		if (CurrentWeaponID < 0)
		{
			CurrentWeaponID += Weapons.Num();
		}
		UpdateHeldWeapon();
	}
}

void AGAS_ExerciceCharacter::UpdateHeldWeapon()
{
	for (int i = 0; i < Weapons.Num(); ++i)
	{
		if (i != CurrentWeaponID)
		{
			Weapons[i]->SetVisibility(false, true);
		}
		else
		{
			Weapons[i]->SetVisibility(true, true);
		}
	}
}

void AGAS_ExerciceCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGAS_ExerciceCharacter::GetHasRifle()
{
	return bHasRifle;
}