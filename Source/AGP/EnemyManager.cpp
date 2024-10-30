// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set maximum allowed enemies and initialise spawn count
	MaxTotalEnemies = 10;
	TotalEnemiesSpawned = 0;
}

// Checks if enemies can be spawned based on max limits
bool AEnemyManager::CanSpawnEnemy() const
{
	return TotalEnemiesSpawned < MaxTotalEnemies;
}

// Registers an enemy spawn
void AEnemyManager::RegisterSpawnedEnemy()
{
	// Increment the enemy count if the spawn limit isn't reached
	if (CanSpawnEnemy())
	{
		TotalEnemiesSpawned++;
	}
}

// Deregister an enemy, reducing the count when an enemy is destroyed
void AEnemyManager::DeregisterEnemy()
{
	// Decrement the count
	if (TotalEnemiesSpawned > 0)
	{
		TotalEnemiesSpawned--;
	}
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

