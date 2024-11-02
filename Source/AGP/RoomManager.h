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
    
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	// Generates and replicates the map seed
	UFUNCTION(Server, Reliable)
	void GenerateMapSeed();

	// Initializes map generation with the provided seed
	void GenerateMap(int32 Seed);

	// Callback to generate map on clients once MapSeed is replicated
	UFUNCTION()
	void OnRep_MapSeed();

	// Spawn methods
	void SpawnInitialRoom(FRandomStream& Stream);
	void SpawnRoomAtRandomExit(FRandomStream& Stream);

	// Checks if room position is valid
	bool IsRoomPositionValid(const FVector& CenterPoint);
    
	// Room-related properties
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> RoomClasses;
    
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> EnemySpawnerRoomClasses;
    
	UPROPERTY(EditDefaultsOnly, Category = "Rooms")
	TSubclassOf<ARoomBase> InitialRoomClass;
    
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 SpawnerRoomInterval;

	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 GridSize;

	UPROPERTY(EditAnywhere, Category = "Rooms")
	float RoomSpacing;

	// The seed to sync map generation across clients
	UPROPERTY(ReplicatedUsing = OnRep_MapSeed)
	int32 MapSeed;
    
	TArray<FTransform> RoomExits;
	TSet<FVector> OccupiedPositions;
	TArray<ARoomBase*> SpawnedRooms;
    
	int32 RoomCount;
	UPROPERTY(EditAnywhere, Category = "Rooms")
	int32 MaxRooms;

public:    
	virtual void Tick(float DeltaTime) override;
};
