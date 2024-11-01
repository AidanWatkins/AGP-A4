// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawningComponent.h"

#include "EngineUtils.h"

// Sets default values for this component's properties
UEnemySpawningComponent::UEnemySpawningComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default values for spawning properties
	SpawnInterval = 5.0f;
	MaxEnemies = 3;
	SpawnedEnemiesCount = 0;
}


// Called when the game starts
void UEnemySpawningComponent::BeginPlay()
{
	Super::BeginPlay();

	// Finding the enemy manager in the world
	for (TActorIterator<AEnemyManager> It(GetWorld()); It; ++It)
	{
		EnemyManager = *It;
		break;
	}

	// If the enemy type and spawn limit are valid, start the timer
	if (EnemyType && MaxEnemies > 0)
	{
		// Spawning enemies at a set interval
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UEnemySpawningComponent::SpawnEnemy, SpawnInterval, true);
	}
}

// Spawns an enemy at specified intervals whilst not exceeding the max limits
void UEnemySpawningComponent::SpawnEnemy()
{
	if (!EnemyManager || !EnemyManager->CanSpawnEnemy() || SpawnedEnemiesCount >= MaxEnemies)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	// Get the room's origin and rotation
	FVector RoomOrigin = GetOwner()->GetActorLocation();
	FRotator RoomRotation = GetOwner()->GetActorRotation();

	// Set a center offset (based on an approximate room size)
	FVector CenterOffset(500.0f, 0.0f, 100.0f);  // Adjust this offset based on room dimensions

	// Calculate spawn location at the room's center with an additional Z offset
	FVector SpawnLocation = RoomOrigin + RoomRotation.RotateVector(CenterOffset);

	// Spawn enemy at the calculated location and with room’s rotation
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyType, SpawnLocation, RoomRotation, SpawnParams);

	if (SpawnedEnemy)
	{
		SpawnedEnemiesCount++;
	}
}

// Called every frame
void UEnemySpawningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

