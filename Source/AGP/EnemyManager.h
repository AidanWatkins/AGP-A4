// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class AGP_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

	// Maximum number of enemies to be spawned in the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	int32 MaxTotalEnemies;

	// Check if an enemy can be spawned
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	bool CanSpawnEnemy() const;

	// Function to register a spawned enemy
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void RegisterSpawnedEnemy();

	// Function to deregister an enemy when destroyed
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void DeregisterEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Tracking the current number of enemies spawned
	int32 TotalEnemiesSpawned;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
