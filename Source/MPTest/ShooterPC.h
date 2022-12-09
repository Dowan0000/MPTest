// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPC.generated.h"

/**
 * 
 */
UCLASS()
class MPTEST_API AShooterPC : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPC();

	virtual void Tick(float DeltaTimes) override;

protected:
	void SetHUDTime();

	UFUNCTION(Server, Reliable)
	void ServerTime(float TimeFromClient);

	UFUNCTION(Client, Reliable)
	void ClientTime(float TimeFromServer, float ServerTime);

private:
	float CurServerTime;
};
