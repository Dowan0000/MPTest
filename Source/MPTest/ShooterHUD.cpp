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

	
}
