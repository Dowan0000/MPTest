// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCharacter.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	EITnECH();
}

void AShooterHUD::BeginPlay()
{
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	BindHealthDele();
}

void AShooterHUD::BindHealthDele()
{
	Character = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Cast in HUD"));
		Character->UpdateHealthDele.AddUFunction(this, FName("SetHealth"));
		SetHealth(Character->GetMaxHealth());
		return;
	}

	GetWorldTimerManager().SetTimer(BindTimer, this, &AShooterHUD::BindHealthDele, 0.1f);
}

void AShooterHUD::SetHealth_Implementation(float HealthRate)
{

}

void AShooterHUD::EITnECH()
{
	if (Character == nullptr)
	{
		Character = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}
	if (Character && Character->GetEquipWeapon())
	{
		if (Character->GetEquipWeapon()->GetItemType() == EItemType::EIT_Pistol)
		{
			if (Character->GetVelocity().Size() || Character->GetEquipWeapon()->GetbIsShoot())
			{
				SetCrossHairState(ECrossHair::ECH_PistolMove);
			}
			else
			{
				SetCrossHairState(ECrossHair::ECH_Pistol);
			}
		}
		if (Character->GetEquipWeapon()->GetItemType() == EItemType::EIT_Rifle)
		{
			if (Character->GetVelocity().Size() || Character->GetEquipWeapon()->GetbIsShoot())
			{
				SetCrossHairState(ECrossHair::ECH_RifleMove);
			}
			else
			{
				SetCrossHairState(ECrossHair::ECH_Rifle);
			}
		}
		//
		if (Character->GetEquipWeapon()->GetItemType() == EItemType::EIT_Sniper)
		{
			SetCrossHairState(ECrossHair::ECH_Sniper);
		}
		if (Character->GetEquipWeapon()->GetItemType() == EItemType::EIT_Bazooka)
		{
			SetCrossHairState(ECrossHair::ECH_Bazooka);
		}
	}
	
}

void AShooterHUD::SetCrossHairState(ECrossHair NewCrossHair)
{
	if (Character == nullptr)
	{
		Character = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}
	if (Character == nullptr) return;

	FVector2D WalkSpeed(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
	FVector2D VelocityRange(0.f, 1.f);
	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	float VelocitySize = Velocity.Size();

	float CrosshairVelocity =  FMath::GetMappedRangeValueClamped(WalkSpeed, VelocityRange, VelocitySize) + 1;

	// Crosshair When Jump
	if (Character->GetCharacterMovement()->IsFalling())
	{
		CrosshairInAir = FMath::FInterpTo(CrosshairInAir, 10.f, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 2.f);
	}
	else
	{
		CrosshairInAir = FMath::FInterpTo(CrosshairInAir, 0.f, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 10.f);
	}
	// Crosshair When Shoot
	if (Character->GetEquipWeapon()->GetbIsShoot())
	{
		CrosshairIsShoot = FMath::FInterpTo(CrosshairIsShoot, 10.f, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 10.f);
		// LookUp When Shoot
		LookUpWhenShoot = FMath::FInterpTo(LookUpWhenShoot, 0.3f, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 30.f);
		Character->LookUp(-LookUpWhenShoot);
	}
	else
	{
		CrosshairIsShoot = FMath::FInterpTo(CrosshairIsShoot, 0.f, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5.f);
	}

	switch (NewCrossHair)
	{
	case ECrossHair::ECH_Pistol:
		DrawCrossHairXAxis = 10.f * CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;
		DrawCrossHairYAxis = 10.f * CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_PistolMove:
		DrawCrossHairXAxis = 10.f * CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;
		DrawCrossHairYAxis = 10.f * CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_Rifle:
		DrawCrossHairXAxis = CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;
		DrawCrossHairYAxis = CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_RifleMove:
		DrawCrossHairXAxis = 5.f * CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;
		DrawCrossHairYAxis = 5.f * CrosshairVelocity + CrosshairInAir + CrosshairIsShoot;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_Sniper:

		break;
	case ECrossHair::ECH_Bazooka:

		break;
	}

}

void AShooterHUD::DrawCrossHair(UTexture2D* Texture, float XAxis, float YAxis)
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D ViewportCenter(ViewportSize.X / 2, ViewportSize.Y / 2);

	float TextureWidth = CrossHairTop->GetSizeX() / 1.2f;
	float TextureHeight = CrossHairTop->GetSizeY() / 1.2f;

	FVector2D TextureDrawPoint(ViewportCenter.X - TextureWidth / 2.f + XAxis,
		ViewportCenter.Y - TextureHeight / 2.f + YAxis);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y,
		TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f);
}