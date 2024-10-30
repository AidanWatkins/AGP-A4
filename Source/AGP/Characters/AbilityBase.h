// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Charge UMETA(DisplayName = "Charge"),
	Shield UMETA(DiplayName = "Shield"),
	HealCircle UMETA(DiplayName = "Heal Circle"),
	Fireball UMETA(DiplayName = "Fireball"),
	ConeBlast UMETA(DiplayName = "Cone Blast")
};

UCLASS(Blueprintable)
class AGP_API AAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
	EAbilityType AbilityType;

	virtual void ActivateAbility();
	virtual void DeactivateAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
