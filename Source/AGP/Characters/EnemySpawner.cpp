// Fill out your copyright notice in the Description page of Project Settings.


#include "AGP/Characters/EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnInterval = 5.0f;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true);
	
}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation);
	}
}




