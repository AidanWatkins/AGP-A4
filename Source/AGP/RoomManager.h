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
	ARoomManager();

protected:
	virtual void BeginPlay() override;

	// Spawns the initial room at the origin
	void SpawnInitialRoom();

	// Attempts to spawn a new room at a random available exit
	void SpawnRoomAtRandomExit();

	// Checks if the given center point is already occupied by another room
	bool IsRoomPositionValid(const FVector& CenterPoint);

	// Array of available room classes
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> RoomClasses;

	// Array of room classes with enemy spawners
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> EnemySpawnerRoomClasses;

	// The initial room blueprint to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TSubclassOf<ARoomBase> InitialRoomClass;

	// Interval at which enemy spawner rooms should be placed
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 SpawnerRoomInterval;

	// The size of the grid to help organize room placement
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 GridSize;

	// The spacing between rooms, should correspond to room size
	UPROPERTY(EditAnywhere, Category = "Rooms")
	float RoomSpacing;

	// List of available exit points for new rooms
	TArray<FTransform> RoomExits;

	// List of all spawned rooms
	TArray<ARoomBase*> SpawnedRooms;

	// List of occupied positions for room centers to prevent overlap
	TSet<FVector> OccupiedPositions;

	// Number of rooms spawned
	int32 RoomCount;

	// Maximum number of rooms to spawn
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 MaxRooms;

public:	
	virtual void Tick(float DeltaTime) override;
};
