// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/Characters/AbilityBase.h"
#include "Components/CapsuleComponent.h"
#include "ChargeAbility.generated.h"


/**
 * 
 */

class APlayerCharacter;

UCLASS()
class AGP_API AChargeAbility : public AAbilityBase
{
	GENERATED_BODY()

public:
	AChargeAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Info")
	float ChargeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Info")
	float ChargeRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CollisionComponent;

	virtual void ActivateAbility() override;

	void InitializeAbility(APlayerCharacter* Character);

	virtual void DeactivateAbility() override;

protected:
	float DistanceTraveled;

	void MoveForward();

	virtual void BeginPlay() override;

	FTimerHandle ChargeTimerHandle;

	APlayerCharacter* PlayerCharacter;

	UFUNCTION()
	void OnChargeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	
};
