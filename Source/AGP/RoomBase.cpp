// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBase.h"

// Sets default values
ARoomBase::ARoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TArray<FTransform> ARoomBase::GetExitLocations()
{
	TArray<FTransform> ExitLocations;
	// Iterate over all exit arrow components and retrieve their transforms
	for (UArrowComponent* Exit : Exits)
	{
		if (Exit)
		{
			FTransform Location = Exit->GetComponentTransform();
			ExitLocations.Add(Location);
			UE_LOG(LogTemp, Warning, TEXT("Exit Location: %s"), *Location.ToString());
		}
	}
	return ExitLocations;
}

TArray<UArrowComponent*> ARoomBase::GetExitComponents()
{
	// Return the array of arrow components
	return Exits;
}

TArray<USceneComponent*> ARoomBase::GetWeaponSpawnPoints()
{
	return WeaponSpawnPoints;
}


// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();

	// Clear Exits array
	Exits.Empty();

	//Retrieve all components of type UArrowComponent attached to this actor
	TArray<UArrowComponent*> FoundComponents;
	GetComponents(FoundComponents);

	// Filter components by name and add only those containing "Exit" to the Exits array
	for (UArrowComponent* Arrow : FoundComponents)
	{
		if (Arrow->GetName().Contains("Exit"))
		{
			Exits.Add(Arrow);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Number of exits found: %d"), Exits.Num());

	TArray<USceneComponent*> AllComponents;
	GetComponents<USceneComponent>(AllComponents);
	for(USceneComponent* Comp : AllComponents)
	{
		if (Comp->GetName().Contains("WeaponSpawnPoint"))
		{
			WeaponSpawnPoints.Add(Comp);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Number of weapon spawn points found: %d"), WeaponSpawnPoints.Num());
}

// Called every frame
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

