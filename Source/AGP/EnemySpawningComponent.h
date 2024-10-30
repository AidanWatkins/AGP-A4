// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyManager.h"
#include "Components/ActorComponent.h"
#include "EnemySpawningComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UEnemySpawningComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemySpawningComponent();

	// The type of enemy to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> EnemyType;

	// The time interval between enemy spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval;

	// The maximum number of enemies that can be spawned by this component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxEnemies;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	// Timer handler to control enemy spawning intervals
	FTimerHandle SpawnTimerHandle;

	// Counter to track number of enemies spawned
	int32 SpawnedEnemiesCount;

	// Spawns a new enemy
	void SpawnEnemy();

	// Reference to the EnemyManager for centralised enemy tracking
	AEnemyManager* EnemyManager;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
