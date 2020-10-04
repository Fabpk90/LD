// Copyright Epic Games, Inc. All Rights Reserved.

#include "LDCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <LD\Asteroid.h>
#include "Kismet/KismetMathLibrary.h"
#include <Containers\UnrealString.h>

//////////////////////////////////////////////////////////////////////////
// ALDCharacter

ALDCharacter::ALDCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALDCharacter::OnCollision);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALDCharacter::OnFirePressed_Implementation);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALDCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALDCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALDCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALDCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALDCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALDCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALDCharacter::OnResetVR);
}

void ALDCharacter::ComputeAngle()
{
	FVector position = GetActorLocation() - asteroidInRange->GetActorLocation();
	position.Normalize();

	asteroidAngle = FMath::RadiansToDegrees(FMath::Acos(position.X));

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Angle  %f"), asteroidAngle));
}


void ALDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ALDCharacter::OnFirePressed_Implementation()
{
	auto forward = GetCameraBoom()->GetForwardVector();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Eh %f %f %f!"), forward.X, forward.Y, forward.Z));

	auto proj = GetWorld()->SpawnActor<AProjectile>(projectile, GetActorLocation() + forward * 200.0f, FRotator::ZeroRotator);
	auto pos = proj->GetActorLocation();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Eh %f %f %f!"), pos.X, pos.Y, pos.Z));


	//TODO: fix me, crashing on null
	UStaticMeshComponent* comp = (UStaticMeshComponent*)proj->GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (comp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("HO !"));
		comp->AddImpulse(forward * 10000.0f);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("nope !"));
}

void ALDCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALDCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALDCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALDCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALDCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALDCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALDCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALDCharacter::Tick(float delta)
{
	Super::Tick(delta);

    if(asteroidInRange)
    {
		asteroidAngle = (asteroidAngle + delta);
		FVector pos;

		pos.X = FMath::Cos(asteroidAngle + 180);
		pos.Y = FMath::Sin(asteroidAngle + 180);

		pos *= 100.0f;

		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Pos %f, %f"), pos.X, pos.Y));

		SetActorLocation(asteroidInRange->GetActorLocation() + pos, false, nullptr, ETeleportType::ResetPhysics);
    }
}

void ALDCharacter::OnCollision(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	asteroidInRange = Cast<AAsteroid>(OtherActor);

	if (asteroidInRange)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Asteroid in range !"));
		ComputeAngle();
		GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	}
}
