// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WeaponInterface.h"
#include "Net/UnrealNetwork.h"
#include "Weapon.h"
#include "Rifle.h"
#include "ShooterHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

AShooterCharacter::AShooterCharacter() : 
	NumberOfWeapon(0), CurWeaponNumber(0),
	Health(100), MaxHealth(100), ZoomControlValue(1.f)

{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 250.f;
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
	DOREPLIFETIME(AShooterCharacter, Health);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AShooterCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AShooterCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressShoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AShooterCharacter::ReleasedShoot);

	PlayerInputComponent->BindAction(TEXT("PickUpItem"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressPickUpItem);

	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressChangeWeapon);

	PlayerInputComponent->BindAction(TEXT("DropWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressDropWeapon);

	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PressZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Released, this, &AShooterCharacter::ReleasedZoom);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= AppliedDamage;
	OnRep_Health();

	return AppliedDamage;
}

void AShooterCharacter::OnRep_Health()
{
	// delegate
	if (UpdateHealthDele.IsBound())
		UpdateHealthDele.Broadcast(Health / MaxHealth);
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
	AddControllerPitchInput(Value * ZoomControlValue);
}

void AShooterCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value * ZoomControlValue);
}

void AShooterCharacter::PressShoot()
{
	if (EquipWeapon)
	{
		EquipWeapon->SetPressShoot(true);
	}

	ReqPressShoot();
}

void AShooterCharacter::ReleasedShoot()
{
	if (EquipWeapon)
	{
		EquipWeapon->SetPressShoot(false);
	}
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

		EquipWeapon->SetItemState(EItemState::EIS_NonEquipped);

		EquipWeapon = Inventory[0];
		EquipWeapon->SetItemState(EItemState::EIS_Equipped);
		CurWeaponNumber = 1;
	}
	else
	{
		if (EquipWeapon == nullptr) return;

		EquipWeapon->SetItemState(EItemState::EIS_NonEquipped);

		EquipWeapon = Inventory[CurWeaponNumber];
		EquipWeapon->SetItemState(EItemState::EIS_Equipped);
		CurWeaponNumber++;
	}
}

void AShooterCharacter::PressDropWeapon()
{
	ReqPressDropWeapon();
}

void AShooterCharacter::ReqPressDropWeapon_Implementation()
{
	ResPressDropWeapon();
}

void AShooterCharacter::ResPressDropWeapon_Implementation()
{
	if (Inventory.Num() >= 2)
	{
		Inventory[CurWeaponNumber - 1]->SetItemState(EItemState::EIS_Dropped);
		Inventory[CurWeaponNumber - 1]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Inventory[CurWeaponNumber - 1]->SetOwner(nullptr);

		Inventory.RemoveAt(CurWeaponNumber - 1);

		if (CurWeaponNumber <= Inventory.Num() - 1)
		{
			EquipWeapon = Inventory[CurWeaponNumber - 1];
			EquipWeapon->SetItemState(EItemState::EIS_Equipped);

			NumberOfWeapon--;
		}
		else
		{
			EquipWeapon = Inventory[0];
			EquipWeapon->SetItemState(EItemState::EIS_Equipped);

			NumberOfWeapon--;
			CurWeaponNumber = 1;
		}
	}
}

void AShooterCharacter::PressZoom()
{
	if (EquipWeapon && EquipWeapon->GetItemType() == EItemType::EIT_Sniper)
	{
		if (ZoomWidgetClass)
		{
			ZoomWidget = CreateWidget<UUserWidget>(GetWorld(), ZoomWidgetClass);
			if (ZoomWidget)
			{
				ZoomWidget->AddToViewport();
				
				SpringArm->TargetArmLength = -4000.f;
				ZoomControlValue = 0.2f;
			}
		}
	}
}

void AShooterCharacter::ReleasedZoom()
{
	if (EquipWeapon && EquipWeapon->GetItemType() == EItemType::EIT_Sniper)
	{
		if (ZoomWidgetClass && ZoomWidget)
		{
			ZoomWidget->RemoveFromParent();
			
			SpringArm->TargetArmLength = 250.f;
			ZoomControlValue = 1.f;
		}
	}
}