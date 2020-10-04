// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Asteroid.generated.h"


UCLASS()
class LD_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AsteroidConfig)
	float radius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AsteroidConfig)
	float forceAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AsteroidConfig)
	class USphereComponent* collisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AsteroidConfig)
	class UParticleSystemComponent* particleSystem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void OnExploding();

};
