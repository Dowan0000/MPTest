// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WeaponInterface.h"
#include "Net/UnrealNetwork.h"
#include "Weapon.h"

AShooterCharacter::AShooterCharacter() : 
	NumberOfWeapon(0), CurWeaponNumber(0)

{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterCharacter, EquipWeapon);
	DOREPLIFETIME(AShooterCharacter, OverlappingWeapon);
	
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AShooterCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AShooterCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressShoot);
	
	PlayerInputComponent->BindAction(TEXT("PickUpItem"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressPickUpItem);

	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressChangeWeapon);

}

void AShooterCharacter::UpDown(float Value)
{
	if (Controller && Value)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::LeftRight(float Value)
{
	if (Controller && Value)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AShooterCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value);
}

void AShooterCharacter::PressShoot()
{
	ReqPressShoot();
}

void AShooterCharacter::ReqPressShoot_Implementation()
{
	ResPressShoot();
}

void AShooterCharacter::ResPressShoot_Implementation()
{
	if (EquipWeapon)
	{
		IWeaponInterface* Interface = Cast<IWeaponInterface>(EquipWeapon);
		if (Interface)
		{
			Interface->Execute_PressShoot(EquipWeapon);
		}
	}
}

void AShooterCharacter::PressPickUpItem()
{
	ReqPressPickUpItem();
}

void AShooterCharacter::ReqPressPickUpItem_Implementation()
{
	ResPressPickUpItem();
}

void AShooterCharacter::ResPressPickUpItem_Implementation()
{
	if (OverlappingWeapon)
	{
		IWeaponInterface* Interface = Cast<IWeaponInterface>(OverlappingWeapon);
		if (Interface)
		{
			Interface->Execute_PressPickUpItem(OverlappingWeapon);
		}
	}
}

void AShooterCharacter::PressChangeWeapon()
{
	ReqPressChangeWeapon();
}

void AShooterCharacter::ReqPressChangeWeapon_Implementation()
{
	ResPressChangeWeapon();
}

void AShooterCharacter::ResPressChangeWeapon_Implementation()
{
	if (CurWeaponNumber == NumberOfWeapon)
	{
		if (EquipWeapon == nullptr) return;

		EquipWeapon->GetMesh()->SetVisibility(false);

		EquipWeapon = Inventory[0];
		EquipWeapon->GetMesh()->SetVisibility(true);
		CurWeaponNumber = 1;
	}
	else
	{
		if (EquipWeapon == nullptr) return;

		EquipWeapon->GetMesh()->SetVisibility(false);

		EquipWeapon = Inventory[CurWeaponNumber];
		EquipWeapon->GetMesh()->SetVisibility(true);
		CurWeaponNumber++;
	}
}