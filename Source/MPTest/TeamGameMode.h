// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Team.h"
#include "TeamGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MPTEST_API ATeamGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void HandleMatchHasStarted() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShooterCharacter> RedCharacter;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterCharacter> BlueCharacter;

public:
	
};
