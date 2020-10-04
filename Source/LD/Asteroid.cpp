// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"
#include <Particles\ParticleSystemComponent.h>

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collisionSphere->SetSphereRadius(radius);

	SetRootComponent(collisionSphere);

	particleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAsteroid::OnExploding()
{
	particleSystem->SetVisibility(true);
	particleSystem->Activate(true);

	SetLifeSpan(.5f);
}

