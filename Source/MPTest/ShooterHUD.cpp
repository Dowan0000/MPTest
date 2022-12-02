// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCharacter.h"
#include "Weapon.h"

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
	}
	
}

void AShooterHUD::SetCrossHairState(ECrossHair NewCrossHair)
{
	switch (NewCrossHair)
	{
	case ECrossHair::ECH_Pistol:
		DrawCrossHairXAxis = 10.f;
		DrawCrossHairYAxis = 10.f;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_PistolMove:
		DrawCrossHairXAxis = 25.f;
		DrawCrossHairYAxis = 25.f;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_Rifle:
		DrawCrossHairXAxis = 0.f;
		DrawCrossHairYAxis = 0.f;

		DrawCrossHair(CrossHairTop, 0.f, -DrawCrossHairYAxis);
		DrawCrossHair(CrossHairBottom, 0.f, DrawCrossHairYAxis);
		DrawCrossHair(CrossHairLeft, -DrawCrossHairXAxis, 0.f);
		DrawCrossHair(CrossHairRight, DrawCrossHairXAxis, 0.f);

		break;
	case ECrossHair::ECH_RifleMove:
		DrawCrossHairXAxis = 15.f;
		DrawCrossHairYAxis = 15.f;

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