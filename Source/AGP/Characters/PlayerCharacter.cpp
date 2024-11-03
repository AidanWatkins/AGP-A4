#include "PlayerCharacter.h"
#include "ChargeAbility.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Set up input mapping for the local player
    if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    // Ability setup
    CurrentAbility = GetWorld()->SpawnActor<AChargeAbility>(AChargeAbility::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
    if (AChargeAbility* ChargeAbility = Cast<AChargeAbility>(CurrentAbility))
    {
        ChargeAbility->InitializeAbility(this);
    }
}

void APlayerCharacter::ActivateCurrentAbility()
{
    if (CurrentAbility)
    {
        CurrentAbility->ActivateAbility();
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        Input->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireWeapon);
        Input->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABaseCharacter::Reload);
        Input->BindAction(AbilityAction, ETriggerEvent::Started, this, &APlayerCharacter::ActivateCurrentAbility);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FVector ForwardVector = GetActorForwardVector();
    AddMovementInput(ForwardVector, MovementVector.X);
    const FVector RightVector = GetActorRightVector();
    AddMovementInput(RightVector, MovementVector.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookVector = Value.Get<FVector2D>() * LookSensitivity;
    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::FireWeapon(const FInputActionValue& Value)
{
    FVector CameraPosition;
    FRotator CameraRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraPosition, CameraRotation);
    const FVector CameraForward = UKismetMathLibrary::GetForwardVector(CameraRotation);
    if (BulletStartPosition)
    {
        Fire(BulletStartPosition->GetComponentLocation() + 10000.0f * CameraForward);
    }
}
