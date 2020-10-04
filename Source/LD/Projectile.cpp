// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <LD\Asteroid.h>
#include <LD\LDCharacter.h>
#include "Components\StaticMeshComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(mesh);

	mesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10.0f);

	mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnCollision);
}

void AProjectile::OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetLifeSpan(0.0f);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AAAAAH!"));

	if (OtherActor->IsA<AAsteroid>())
	{
		auto asteroid = Cast<AAsteroid>(OtherActor);

		auto character = Cast<ALDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		FVector direction = character->GetActorLocation() - asteroid->GetActorLocation();
		direction.Normalize();

		character->GetMesh()->AddImpulse(FVector(1, 1, 0) * direction * 1000.0f);

		asteroid->OnExploding();
	}
}

