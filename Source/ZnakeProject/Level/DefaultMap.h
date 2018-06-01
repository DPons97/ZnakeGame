// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultMap.generated.h"

USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	TSubclassOf<class AScoringActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float MinSpawnCooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float MaxSpawnCooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 MaxActorsInMap = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float NewPointCooldown = 0;

	int32 ActorsInMap = 0;

	float ElapsedTime = 0.f;
};


UCLASS()
class ZNAKEPROJECT_API ADefaultMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultMap();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
	TArray<FSpawnParams> SpawnParams = { 
		{
			FString("DefaultPoint"),
			nullptr,
			5.f,
			2.f,
			1,
			5.f,
			0
		}
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	float CheckSpawnSpeed = 15.f;
	FVector SpawnLocation;
	AActor * LastPointActorSpawned = nullptr;
	
	bool ChooseRandomLocation(FVector& OutLocation);

	bool IsLocationColliding(FVector Location);

	template<class T>
	T* SpawnActorInMap(TSubclassOf<T> ToSpawn, FVector Location);

	void SpawnPointActors(float DeltaTime);

	
};
