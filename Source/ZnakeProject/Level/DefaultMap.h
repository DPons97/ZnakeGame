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
	TSubclassOf<AActor> PointActorClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Score")
	USceneComponent * DefaultSceneComponent;

	UPROPERTY(EditDefaultsOnly, Category="Score")
	float MinSpawnCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	float MaxSpawnCooldown = 5.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	class UBoxComponent * PlayableBounds;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float GridOffset = 16.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score")
	bool EnableSpawning = true;

	UFUNCTION(BlueprintCallable, Category="Map")
	FVector ApproximateVectorComponents(FVector Vector, int Grid);

private:
	float ElapsedTime;
	float NewPointCooldown = 5.f;
	FVector SpawnLocation;
	AActor * LastPointActorSpawned = nullptr;
	
	bool ChooseRandomLocation(FVector& OutLocation);

	bool IsLocationColliding(FVector Location);

	template<class T>
	void SpawnActorInMap(TSubclassOf<T> ToSpawn, FVector Location);
	
};
