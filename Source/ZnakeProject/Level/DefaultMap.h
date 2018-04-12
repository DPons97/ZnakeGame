// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultMap.generated.h"

UCLASS()
class ZNAKEPROJECT_API ADefaultMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultMap();

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	TSubclassOf<AActor> PointActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Score")
	float NewPointCooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	FBox MapPlayableBounds;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	float ElapsedTime;

	FVector SpawnLocation;
	
	bool ChooseRandomLocation(FVector& OutLocation);

	bool IsLocationColliding(FVector Location);

	FVector ApproximateVectorComponents(FVector Vector, int Grid);

	void SpawnNewPoint(FVector Location);
	
};
