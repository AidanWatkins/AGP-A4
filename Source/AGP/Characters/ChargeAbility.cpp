// Fill out your copyright notice in the Description page of Project Settings.

#include "AGP/Characters/ChargeAbility.h"

#include "EnemyCharacter.h"
#include "AGP/Characters/PlayerCharacter.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AChargeAbility::AChargeAbility()
{
	PrimaryActorTick.bCanEverTick = false;  // No need to enable ticking
	ChargeSpeed = 2000.0f;
	ChargeRange = 1000.0f;
	DistanceTraveled = 0.0f;

	//Configuring collision component
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionProfileName("OverlapAllDynamic");
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCapsuleHalfHeight(100.0f);
	CollisionComponent->SetCapsuleRadius(50.0f);
}

void AChargeAbility::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AChargeAbility::OnChargeHit);
}

void AChargeAbility::ActivateAbility()
{
	Super::ActivateAbility();
	UE_LOG(LogTemp, Warning, TEXT("ChargeAbility Activated"));

	// Start the timer to repeatedly call MoveForward every 0.01 seconds
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &AChargeAbility::MoveForward, 0.01f, true);
}

void AChargeAbility::InitializeAbility(APlayerCharacter* Character)
{
	PlayerCharacter = Character;
}

void AChargeAbility::MoveForward()
{
	if (PlayerCharacter)
	{
		FVector ForwardVector = PlayerCharacter->GetActorForwardVector();
		FVector NewLocation = PlayerCharacter->GetActorLocation() + ForwardVector * ChargeSpeed * 0.01f;
		PlayerCharacter->SetActorLocation(NewLocation);

		DistanceTraveled += ChargeSpeed * 0.01f;
		UE_LOG(LogTemp, Warning, TEXT("Moving Forward. DistanceTraveled: %f"), DistanceTraveled);

		if (DistanceTraveled >= ChargeRange)
		{
			DeactivateAbility();
		}
	}
}

void AChargeAbility::OnChargeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AEnemyCharacter::StaticClass()))
	{
		AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OtherActor);
		if (HitEnemy)
		{

			UGameplayStatics::ApplyDamage(HitEnemy, Damage, nullptr, this, UDamageType::StaticClass());

			DeactivateAbility();
		}
	}
}

void AChargeAbility::DeactivateAbility()
{
	Super::DeactivateAbility();
	UE_LOG(LogTemp, Warning, TEXT("ChargeAbility Deactivated"));

	DistanceTraveled = 0.0f;
	// Stop the timer
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
}
