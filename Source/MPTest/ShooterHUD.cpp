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
			SetCrossHairState(ECrossHair::ECH_Pistol);
		}
		if (Character->GetEquipWeapon()->GetItemType() == EItemType::EIT_Rifle)
		{
			SetCrossHairState(ECrossHair::ECH_Rifle);
		}
		//
	}
	
}

void AShooterHUD::SetCrossHairState(ECrossHair NewCrossHair)
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D ViewportCenter(ViewportSize.X / 2, ViewportSize.Y / 2);

	float TextureWidth = CrossHairTop->GetSizeX() / 1.2f;
	float TextureHeight = CrossHairTop->GetSizeY() / 1.2f;
	FVector2D TextureDrawPoint;

	switch (NewCrossHair)
	{
	case ECrossHair::ECH_Pistol:
		TextureWidth;
		TextureHeight;
		/*TextureDrawPoint = FVector2D(ViewportCenter.X - TextureWidth / 2.f,
			ViewportCenter.Y - TextureHeight / 2.f);*/

		break;
	case ECrossHair::ECH_Rifle:
		break;
	case ECrossHair::ECH_Sniper:
		break;
	case ECrossHair::ECH_Bazooka:
		break;
	}

	DrawTexture(CrossHairTop, ViewportCenter.X, ViewportCenter.Y,
		TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f);
	DrawTexture(CrossHairBottom, ViewportCenter.X, ViewportCenter.Y,
		TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f);
	DrawTexture(CrossHairLeft, ViewportCenter.X, ViewportCenter.Y,
		TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f);
	DrawTexture(CrossHairRight, ViewportCenter.X, ViewportCenter.Y,
		TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f);
	
}
