// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

class ARoomBase;

UCLASS()
class AGP_API ARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Spawns the initial room at the origin
	void SpawnInitialRoom();

	TArray<FTransform> RoomExits; // List of available exit points for new rooms
	TArray<ARoomBase*> SpawnedRooms; // List of all spawned rooms
	TArray<FVector> SpawnedRoomPositions; // List of room center points to track occupied positions

	// The initial room blueprint to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TSubclassOf<ARoomBase> InitialRoomClass;

	// Array of room classes for standard spawns
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> RoomClasses;

	// Array of room classes with enemy spawners
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> EnemySpawnerRoomClasses;

	// Interval at which enemy spawner rooms should be placed
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 SpawnerRoomInterval;

	// Spawns a new room at a random available exit
	void SpawnRoomAtRandomExit();

	// Keeps track of the total number of rooms spawned
	int32 RoomCount;

	// Maximum number of rooms to spawn in the level
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 MaxRooms;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
