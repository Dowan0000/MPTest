// Fill out your copyright notice in the Description page of Project Settings.


#include "Shell.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AShell::AShell() : 
	Damage(10.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(Box);
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AShell::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &AShell::OnBoxOverlap);
}

void AShell::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShell, HitEffect);
}

void AShell::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Shell Overlap!"));

	if (HitEffect && HitSound)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorTransform());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}

	// Damage 처리
	TArray<FHitResult> Hits;
	bool bResult = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(),
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(200.f));
	
	if (bResult)
	{
		//뼈마다 다처맞는거같음 ...
		for (FHitResult& HitResult : Hits)
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(Damage, DamageEvent,
				GetWorld()->GetFirstPlayerController(), this);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.BoneName.ToString());
		}
	}

	Destroy();
}

// Called every frame
void AShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

