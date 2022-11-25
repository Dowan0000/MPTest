// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "ShooterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	
	RootComponent = Box;
	Mesh->SetupAttachment(RootComponent);

	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BoxBeginOverlap);
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, ShootMontage);
	DOREPLIFETIME(AWeapon, ShootEffect);
}


void AWeapon::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AShooterCharacter>(OtherActor);
	if (Character)
	{
		const USkeletalMeshSocket* SocketName = Character->GetMesh()->GetSocketByName(FName("hand_r_socket"));
		if (SocketName)
		{
			if (Character->GetEquipWeapon())
			{
				Character->GetEquipWeapon()->GetMesh()->SetVisibility(false);
				//widget

				SocketName->AttachActor(this, Character->GetMesh());
				SetOwner(Character);
				Character->SetEquipWeapon(this);
				Character->Inventory.Add(this);
			
			}
			else
			{
				SocketName->AttachActor(this, Character->GetMesh());
				SetOwner(Character);
				Character->SetEquipWeapon(this);
				Character->Inventory.Add(this);

				Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			}

			
		}
	}
}

void AWeapon::PressShoot_Implementation()
{
	if (Character)
	{
		if (ShootMontage)
		{
			Character->PlayAnimMontage(ShootMontage);
		}
		
		const USkeletalMeshSocket* SocketName = Mesh->GetSocketByName(FName("MuzzleFlash"));
		if (SocketName)
		{
			const FTransform SocketTreansform = SocketName->GetSocketTransform(Mesh);
			if (ShootEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect, SocketTreansform);
			}
		}
	}

	
}
