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
	// Check if an enemy can be spawned based on current limits
	if (!EnemyManager || !EnemyManager->CanSpawnEnemy() || SpawnedEnemiesCount >= MaxEnemies)
	{
		// Stop the timer if conditions for spawning are no longer met
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	// Set spawn location above owners current location to avoid ground overlap
	FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// Spawn parameters for collision handling
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the enemy actor of the specified type at the given location whilst using the set SpawnParams
	AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyType, SpawnLocation, SpawnRotation, SpawnParams);
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

