// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "Net/UnrealNetwork.h"

ARifle::ARifle()
{
	ItemType = EItemType::EIT_Rifle;
}

void ARifle::PressShoot_Implementation()
{
	if (bPressShoot == false) return;

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
