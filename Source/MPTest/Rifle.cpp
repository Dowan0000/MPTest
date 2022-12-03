// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

ARifle::ARifle()
{
	ItemType = EItemType::EIT_Rifle;
}

void ARifle::PressShoot_Implementation()
{
	Super::PressShoot_Implementation();

	PressShootTimer();
}

void ARifle::PressShootTimer()
{
	if (bPressShoot)
	{
		GetWorldTimerManager().SetTimer(ShootTimer, this,
			&ARifle::PressShoot_Implementation, 0.2f);
	}
	
}
