#include "RoomManager.h"
#include "RoomBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ARoomManager::ARoomManager()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    RoomCount = 0;
    MaxRooms = 10;
    GridSize = 20;
    RoomSpacing = 1000.0f;
}

void ARoomManager::BeginPlay()
{
    Super::BeginPlay();
    
    if (HasAuthority())
    {
        GenerateMapSeed();  // Server generates the seed
    }
    else if (MapSeed != 0)
    {
        GenerateMap(MapSeed);  // Client already has seed (e.g., in a respawn or reload scenario)
    }
}

void ARoomManager::GenerateMapSeed_Implementation()
{
    MapSeed = FMath::Rand();  // Generate a random seed
    GenerateMap(MapSeed);      // Server generates map with this seed
}

void ARoomManager::GenerateMap(int32 Seed)
{
    FRandomStream Stream(Seed);  // Initialize the random stream with the seed
    
    SpawnInitialRoom(Stream);
    
    for (int32 i = 1; i < MaxRooms; i++)
    {
        if (RoomExits.Num() > 0)
        {
            SpawnRoomAtRandomExit(Stream);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No available exits, stopping spawn."));
            break;
        }
    }
}

// Callback for when MapSeed is replicated
void ARoomManager::OnRep_MapSeed()
{
    GenerateMap(MapSeed);  // Generate map using replicated seed on clients
}

void ARoomManager::SpawnInitialRoom(FRandomStream& Stream)
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

void ARoomManager::SpawnRoomAtRandomExit(FRandomStream& Stream)
{
    if (RoomExits.Num() == 0 || RoomClasses.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No available exits or room classes."));
        return;
    }

    TArray<int32> ValidExitIndices;
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

    int32 SelectedExitIndex = ValidExitIndices[Stream.RandRange(0, ValidExitIndices.Num() - 1)];
    FTransform ExitTransform = RoomExits[SelectedExitIndex];
    FVector ExitLocation = ExitTransform.GetLocation();
    FRotator ExitRotation = ExitTransform.GetRotation().Rotator();

    TSubclassOf<ARoomBase> RoomToSpawn;
    if (SpawnerRoomInterval > 0 && (RoomCount + 1) % SpawnerRoomInterval == 0 && EnemySpawnerRoomClasses.Num() > 0)
    {
        RoomToSpawn = EnemySpawnerRoomClasses[Stream.RandRange(0, EnemySpawnerRoomClasses.Num() - 1)];
    }
    else
    {
        RoomToSpawn = RoomClasses[Stream.RandRange(0, RoomClasses.Num() - 1)];
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomToSpawn, ExitLocation, ExitRotation, SpawnParams);

    if (NewRoom)
    {
        SpawnedRooms.Add(NewRoom);
        OccupiedPositions.Add(ExitLocation);

        RoomExits.RemoveAt(SelectedExitIndex);

        TArray<FTransform> NewExits = NewRoom->GetExitLocations();
        NewExits.RemoveAll([&](const FTransform& Exit) {
            return Exit.GetLocation().Equals(ExitLocation, 1.0f);
        });
        RoomExits.Append(NewExits);

        UE_LOG(LogTemp, Warning, TEXT("Room %d spawned at location: %s"), RoomCount + 1, *ExitLocation.ToString());
        RoomCount++;
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

void ARoomManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARoomManager, MapSeed);
}
