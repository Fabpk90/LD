// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <LD\Asteroid.h>
#include <LD\LDCharacter.h>

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10.0f);

	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnCollision);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetLifeSpan(0.0f);

	if (OtherActor->IsA<AAsteroid>())
	{
		auto asteroid = Cast<AAsteroid>(OtherActor);

		auto character = Cast<ALDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		FVector direction = character->GetActorLocation() - asteroid->GetActorLocation();
		direction.Normalize();

		character->GetMesh()->SetPhysicsLinearVelocity(FVector(1, 1, 0) * direction * 10.0f);
	}
}

