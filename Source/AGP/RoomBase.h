// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RoomBase.generated.h"

UCLASS()
class AGP_API ARoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomBase();

	// Retrieves a list of exit locations represented as FTransforms
	TArray<FTransform> GetExitLocations();

	// Retrieves the array of exit components for this room
	TArray<UArrowComponent*> GetExitComponents();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Array storing arrow components, each representing an exit point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exits")
	TArray<UArrowComponent*> Exits;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
