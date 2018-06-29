// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <functional>
#include "DefaultMap.generated.h"

USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_BODY()

public:
	// Name of the score param
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Parameters")
	FString Name;

	// Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Parameters")
	TSubclassOf<class AScoringActor> ActorClass;

	// Spawning Weight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Parameters")
	int SpawningWeight = 0;

	// Prevent spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Parameters")
	bool PreventSpawn = true;	

	// Force Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Parameters")
	int32 ForceSpawn = 0;

	int32 Spawned = 0;

	UPROPERTY(EditAnywhere)
	int32 InMap = 0;
};


UCLASS()
class ZNAKEPROJECT_API ADefaultMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultMap();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
	float MinSpawnCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
	float MaxSpawnCooldown = 7.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
	int32 AllowCustomsAfter = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	AActor * LastPointActorSpawned = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
	TArray<FSpawnParams> SpawnParams = { 
		{
			FString("DefaultPoint"),
			nullptr,
			30,
			false,
			0,
			0,
			0
		},
		{
			FString("BoostPoint"),
			nullptr,
			10,
			true,
			0,
			0,
			0
		},
		{
			FString("SpeedBoostPoint"),
			nullptr,
			5,
			true,
			0,
			0,
			0
		},
		{
			FString("PointRain"),
			nullptr,
			5,
			true,
			0,
			0,
			0
		}
	};
	
	UFUNCTION(BlueprintCallable)
	bool GetParamIndexByName(FString Name, int32 & OutIndex);

	UFUNCTION(BlueprintCallable)
	void SpawnPointActorByIndex(int SpawnIndex, FVector SpawnLocation);

	UFUNCTION(BlueprintCallable)
	bool ChooseRandomLocation(FVector& OutLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReloadSpawnDistributionWeights();

	UPROPERTY(VisibleDefaultsOnly, Category = "Score")
	USceneComponent * DefaultSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	class UBoxComponent * PlayableBounds;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float GridOffset = 16.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Map")
	FVector ApproximateVectorComponents(FVector Vector, int Grid);

private:
	float ElapsedTime = 0.f;

	float NewPointCooldown = 0.f;

	bool PreventAutoSpawn = false;

	// *** Random generator supports ***
	std::mt19937 RandomGenerator;

	std::discrete_distribution<int> Distribution;

	std::vector<int> ParamList;

	bool IsLocationColliding(FVector Location);

	template<class T>
	T* SpawnActorInMap(TSubclassOf<T> ToSpawn, FVector Location);

	void SpawnPointActor(float DeltaTime);

	bool IsMapEmpty();

	void UpdateForcePreventParams();

	int32 GetForcingSpawn(int32 & OutNumberToSpawn);
};
