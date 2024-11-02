#include "RoomManager.h"
#include "RoomBase.h"
#include "Kismet/KismetMathLibrary.h"

ARoomManager::ARoomManager()
{
	PrimaryActorTick.bCanEverTick = true;

	RoomCount = 0;
	MaxRooms = 10;
	GridSize = 20;
	RoomSpacing = 1000.0f;
}

void ARoomManager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnInitialRoom();

	// Spawns rooms until MaxRooms is reached or no exits remain
	for (int32 i = 1; i < MaxRooms; i++)
	{
		if (RoomExits.Num() > 0)
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
	if (InitialRoomClass)
	{
		ARoomBase* InitialRoom = GetWorld()->SpawnActor<ARoomBase>(InitialRoomClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (InitialRoom)
		{
			RoomExits = InitialRoom->GetExitLocations();
			OccupiedPositions.Add(FVector::ZeroVector);
			RoomCount++;
		}
	}
}

void ARoomManager::SpawnRoomAtRandomExit()
{
	if (RoomExits.Num() == 0 || RoomClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No available exits or room classes."));
		return;
	}

	TArray<int32> ValidExitIndices;

	// Check each exit to see if it provides a valid location
	for (int32 i = 0; i < RoomExits.Num(); i++)
	{
		FTransform ExitTransform = RoomExits[i];
		FVector ExitLocation = ExitTransform.GetLocation();
		FVector CenterPoint = ExitLocation + (ExitTransform.GetRotation().Vector() * RoomSpacing);

		if (IsRoomPositionValid(CenterPoint))
		{
			ValidExitIndices.Add(i);
		}
	}

	if (ValidExitIndices.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid exit found, stopping spawn."));
		return;
	}

	// Pick a random valid exit and room class
	int32 SelectedExitIndex = ValidExitIndices[UKismetMathLibrary::RandomInteger(ValidExitIndices.Num())];
	FTransform ExitTransform = RoomExits[SelectedExitIndex];
	FVector ExitLocation = ExitTransform.GetLocation();
	FRotator ExitRotation = ExitTransform.GetRotation().Rotator();

	TSubclassOf<ARoomBase> RoomToSpawn;

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

	// Set spawn collision handling
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the room and update tracking data
	ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomToSpawn, ExitLocation, ExitRotation, SpawnParams);

	if (NewRoom)
	{
		SpawnedRooms.Add(NewRoom);
		OccupiedPositions.Add(ExitLocation);

		RoomExits.RemoveAt(SelectedExitIndex);

		// Add remaining exits from the new room, excluding the one connecting to the current exit
		TArray<FTransform> NewExits = NewRoom->GetExitLocations();
		NewExits.RemoveAll([&](const FTransform& Exit) {
			return Exit.GetLocation().Equals(ExitLocation, 1.0f);
		});
		RoomExits.Append(NewExits);

		UE_LOG(LogTemp, Warning, TEXT("Room %d spawned at location: %s"), RoomCount + 1, *ExitLocation.ToString());
		RoomCount++;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn room."));
	}
}

bool ARoomManager::IsRoomPositionValid(const FVector& CenterPoint)
{
	for (const FVector& OccupiedPosition : OccupiedPositions)
	{
		if (FVector::DistSquared(OccupiedPosition, CenterPoint) < FMath::Square(RoomSpacing))
		{
			return false;
		}
	}
	return true;
}

void ARoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
