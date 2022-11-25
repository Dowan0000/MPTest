// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"
#include "Weapon.generated.h"

UCLASS()
class MPTEST_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PressShoot();
	virtual void PressShoot_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class AShooterCharacter* Character;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	class UAnimMontage* ShootMontage;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	class UParticleSystem* ShootEffect;

public:	
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }

};
