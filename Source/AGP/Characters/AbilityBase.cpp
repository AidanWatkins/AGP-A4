// Fill out your copyright notice in the Description page of Project Settings.


#include "AGP/Characters/AbilityBase.h"

// Sets default values
AAbilityBase::AAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cooldown = 1.0f;
	Damage = 10.0f;
	Duration = 5.0f;

}

void AAbilityBase::ActivateAbility()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Activated: %s"), *GetNameSafe(this));
}

void AAbilityBase::DeactivateAbility()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Deactivated: %s"), *GetNameSafe(this));
}

// Called when the game starts or when spawned
void AAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

