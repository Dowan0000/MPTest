// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "ShooterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

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
	Box->OnComponentEndOverlap.AddDynamic(this, &AWeapon::BoxEndOverlap);
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
	DOREPLIFETIME(AWeapon, ItemType);
}


void AWeapon::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AShooterCharacter>(OtherActor);
	if (Character)
	{
		Character->SetOverlappingWeapon(this);
	}
}

void AWeapon::BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Character == nullptr)
	{
		Character = Cast<AShooterCharacter>(OtherActor);
	}
	if (Character)
	{
		Character->SetOverlappingWeapon(nullptr);
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

	ReqApplyDamage();
}

void AWeapon::ReqApplyDamage_Implementation()
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	ViewportSize.X /= 2;
	ViewportSize.Y /= 2;

	FVector WorldPosition;
	FVector WorldDirection;

	if (Character)
	{
		UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(Character, 0),
			ViewportSize, WorldPosition, WorldDirection);
	}

	FVector Start = WorldPosition;
	FVector End = WorldPosition + WorldDirection * 50'000.f;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End,
		ECollisionChannel::ECC_Camera);

	if (Hit.Actor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit.Actor.IsValid"));
		FDamageEvent Damage;
		Hit.Actor->TakeDamage(10.f, Damage,
			Character->GetController(), this);
		
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
	
}

void AWeapon::PressPickUpItem_Implementation()
{
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
				Character->SetNumberOfWeapon();
				Character->SetCurWeaponNumber(Character->GetNumberOfWeapon());

				SetItemState(EItemState::EIS_Equipped);
			}
			else
			{
				SocketName->AttachActor(this, Character->GetMesh());
				SetOwner(Character);
				Character->SetEquipWeapon(this);
				Character->Inventory.Add(this);
				Character->SetNumberOfWeapon();
				Character->SetCurWeaponNumber(Character->GetNumberOfWeapon());

				SetItemState(EItemState::EIS_Equipped);
			}
		}
	}
}

void AWeapon::SetItemState(EItemState NewItemState)
{
	switch (NewItemState)
	{
	case EItemState::EIS_Dropped:
		Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Box->SetSimulatePhysics(true);
		Mesh->SetVisibility(true);

		break;
	case EItemState::EIS_Equipped:
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Box->SetSimulatePhysics(false);
		Mesh->SetVisibility(true);

		break;
	case EItemState::EIS_NonEquipped:
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Box->SetSimulatePhysics(false);
		Mesh->SetVisibility(false);

		break;
	}
}