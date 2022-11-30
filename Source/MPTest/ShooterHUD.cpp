// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCharacter.h"

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