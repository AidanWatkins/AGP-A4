// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "EngineUtils.h"
#include "WeaponPickup.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/RoomBase.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PossibleSpawnLocations.IsEmpty())
	{
		PopulateSpawnLocations();
	}

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		SpawnWeaponPickup();
		TimeSinceLastSpawn = 0.0f;
	}
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	if (PossibleSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No weapon spawn locations available"));
		return;
	}

	if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>())
	{
		FVector SpawnPosition = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num() - 1)];
        
		// Optional Z offset to avoid spawning in the floor
		SpawnPosition.Z += 130.0f;

		GetWorld()->SpawnActor<AWeaponPickup>(
			GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator);

		UE_LOG(LogTemp, Display, TEXT("Weapon Pickup Spawned at %s"), *SpawnPosition.ToString());
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	// Clear all previous locations to prevent mixing
	PossibleSpawnLocations.Empty();

	// Populate with new weapon spawn points from each room
	for (TActorIterator<ARoomBase> RoomItr(GetWorld()); RoomItr; ++RoomItr)
	{
		ARoomBase* Room = *RoomItr;
		if (Room)
		{
			TArray<USceneComponent*> SpawnPoints = Room->GetWeaponSpawnPoints();

			for (USceneComponent* SpawnPoint : SpawnPoints)
			{
				if (SpawnPoint)
				{
					PossibleSpawnLocations.Add(SpawnPoint->GetComponentTransform().GetLocation());
				}
			}
		}
	}
    
	UE_LOG(LogTemp, Warning, TEXT("Total valid spawn locations: %d"), PossibleSpawnLocations.Num());
}