// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class MPTEST_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	AShooterCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void UpDown(float Value);
	void LeftRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);

	void PressShoot();

	UFUNCTION(Server, Reliable)
	void ReqPressShoot();

	UFUNCTION(NetMulticast, Reliable)
	void ResPressShoot();

	void PressPickUpItem();

	UFUNCTION(Server, Reliable)
	void ReqPressPickUpItem();

	UFUNCTION(NetMulticast, Reliable)
	void ResPressPickUpItem();

	void PressChangeWeapon();

	UFUNCTION(Server, Reliable)
	void ReqPressChangeWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void ResPressChangeWeapon();

	void PressDropWeapon();

	UFUNCTION(Server, Reliable)
	void ReqPressDropWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void ResPressDropWeapon();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquipWeapon;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeapon* OverlappingWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int NumberOfWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int CurWeaponNumber;

public:	
	FORCEINLINE AWeapon* GetEquipWeapon() const { return EquipWeapon; }
	FORCEINLINE void SetEquipWeapon(AWeapon* NewWeapon) { EquipWeapon = NewWeapon; }
	
	FORCEINLINE void SetOverlappingWeapon(AWeapon* NewWeapon) { OverlappingWeapon = NewWeapon; }

	FORCEINLINE int GetNumberOfWeapon() const { return NumberOfWeapon; }
	FORCEINLINE void SetNumberOfWeapon() { NumberOfWeapon++; }
	FORCEINLINE void SetCurWeaponNumber(int NewWeaponNumber) { CurWeaponNumber = NewWeaponNumber; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<AWeapon*> Inventory;
	
};
