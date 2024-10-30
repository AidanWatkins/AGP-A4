// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "RoomBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoomManager::ARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomCount = 0; // Tracks the number of rooms generated
	MaxRooms = 10; // Sets the maximum number of rooms to spawn
}

// Called when the game starts or when spawned
void ARoomManager::BeginPlay()
{
	Super::BeginPlay();

	// Spawns the set initial room at the origin
	SpawnInitialRoom();

	// Spawns rooms until MaxRooms is reached or no exits remain
	for (int32 i = 1; i < MaxRooms; i++)
	{
		if (RoomExits.Num() > 0) // Check for available exits
		{
			SpawnRoomAtRandomExit();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No available exits, stopping spawn."));
			break;
		}
	}
}

void ARoomManager::SpawnInitialRoom()
{
	if (InitialRoomClass) // Ensure initial room class is assigned
	{
		ARoomBase* InitialRoom = GetWorld()->SpawnActor<ARoomBase>(InitialRoomClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (InitialRoom)
		{
			RoomExits = InitialRoom->GetExitLocations(); // Store exits of the initial room
			UE_LOG(LogTemp, Warning, TEXT("Initial room spawned. Number of exits: %d"), RoomExits.Num());
			RoomCount++;
		}
	}
}

void ARoomManager::SpawnRoomAtRandomExit()
{
    if (RoomExits.Num() > 0 && RoomClasses.Num() > 0) // Check for available exits and room classes
    {
        TArray<int32> ValidExitIndices;
        FTransform ExitTransform;
        FVector ExitLocation;
        FVector CenterPoint;

        // Loop through exits to find all that do not overlap with existing rooms
        for (int32 i = 0; i < RoomExits.Num(); i++)
        {
            ExitTransform = RoomExits[i];
            ExitLocation = ExitTransform.GetLocation();

            // Determine the center point of the new room based on the position of the exit
            FVector ForwardVector = ExitTransform.GetRotation().Vector();
            CenterPoint = ExitLocation + (ForwardVector * 500.0f);

            // Check if the center point overlaps with existing rooms
            bool bPositionOccupied = false;
            for (const FVector& Position : SpawnedRoomPositions)
            {
                if (CenterPoint.Equals(Position, 1.0f)) // Avoid overlap with a tolerance
                {
                    bPositionOccupied = true;
                    break;
                }
            }

            // If no overlap, add this exit index to the valid list
            if (!bPositionOccupied)
            {
                ValidExitIndices.Add(i);
            }
        }

        // If no valid exits were found, stop spawning
        if (ValidExitIndices.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("No valid exit found, stopping spawn."));
            return;
        }

        // Select a random exit from the list of valid exits
        int32 SelectedExitIndex = ValidExitIndices[UKismetMathLibrary::RandomInteger(ValidExitIndices.Num())];
        ExitTransform = RoomExits[SelectedExitIndex];
        ExitLocation = ExitTransform.GetLocation();
        FRotator ExitRotation = ExitTransform.GetRotation().Rotator();

        TSubclassOf<ARoomBase> RoomToSpawn;

        // Check if the next room should contain an enemy spawner
        if (SpawnerRoomInterval > 0 && (RoomCount + 1) % SpawnerRoomInterval == 0 && EnemySpawnerRoomClasses.Num() > 0)
        {
            int32 SpawnerRoomIndex = UKismetMathLibrary::RandomInteger(EnemySpawnerRoomClasses.Num());
            RoomToSpawn = EnemySpawnerRoomClasses[SpawnerRoomIndex];
        }
        else
        {
            int32 RoomIndex = UKismetMathLibrary::RandomInteger(RoomClasses.Num());
            RoomToSpawn = RoomClasses[RoomIndex];
        }

        // Spawn the room at the calculated location and rotation
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomToSpawn, ExitLocation, ExitRotation, SpawnParams);

        if (NewRoom) // If spawning is successful, add the room's details to lists
        {
            SpawnedRooms.Add(NewRoom);
            SpawnedRoomPositions.Add(CenterPoint);

            RoomExits.RemoveAt(SelectedExitIndex); // Remove used exit from list

            // Add remaining exits from the new room, except the connecting one
            TArray<FTransform> NewExits = NewRoom->GetExitLocations();
            NewExits.RemoveAll([&](const FTransform& Exit) -> bool {
                return Exit.GetLocation().Equals(ExitLocation, 1.0f);
            });

            RoomExits.Append(NewExits); // Add valid exits to the available list

            UE_LOG(LogTemp, Warning, TEXT("Room %d spawned at location: %s"), RoomCount + 1, *CenterPoint.ToString());
            RoomCount++;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to spawn room."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No available exits or rooms left to spawn."));
    }
}

// Called every frame
void ARoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

