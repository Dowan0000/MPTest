// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPC.h"
#include "ShooterHUD.h"

AShooterPC::AShooterPC()
{

}

void AShooterPC::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

	SetHUDTime();

	if (!HasAuthority())
	{
		ServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AShooterPC::BeginPlay()
{
	Super::BeginPlay();

}

void AShooterPC::SetHUDTime()
{
	int Minutes;
	int Seconds;

	if (HasAuthority())
	{
		float Time = GetWorld()->GetTimeSeconds();

		Minutes = FMath::FloorToInt(Time / 60.f);
		Seconds = Time - (Minutes * 60);
	}
	else
	{
		Minutes = FMath::FloorToInt(CurServerTime / 60.f);
		Seconds = CurServerTime - (Minutes * 60);
	}
	
	AShooterHUD* HUD = Cast<AShooterHUD>(GetHUD());
	if (HUD)
	{
		HUD->DrawHUDTime(Minutes, Seconds);
	}
}

void AShooterPC::ServerTime_Implementation(float TimeFromClient)
{
	float ServerTime = GetWorld()->GetTimeSeconds();
	ClientTime(TimeFromClient, ServerTime);
}

void AShooterPC::ClientTime_Implementation(float TimeFromServer, float ServerTime)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeFromServer;
	CurServerTime = ServerTime + RoundTripTime / 2.f;
}