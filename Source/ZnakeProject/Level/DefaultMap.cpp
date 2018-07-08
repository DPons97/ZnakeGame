// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMap.h"
#include "PaperTileMapComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ZnakeGameMode.h"
#include "ScoringActor.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <functional>


// Sets default values
ADefaultMap::ADefaultMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultScene"));
	DefaultSceneComponent->SetupAttachment(RootComponent);

	RootComponent = DefaultSceneComponent;

	SpawnBounds = CreateDefaultSubobject<UBoxComponent>(FName("SpawnBox"));
	SpawnBounds->SetupAttachment(DefaultSceneComponent);
	SpawnBounds->SetCollisionProfileName(FName("NoCollision"));
}

/* SpawnActorInMap: Spawns actor in map, given a specific location relative to the map.
	* @param ToSpawn, Subclass of spawning actor
	* @param SpawnLocation, Spawning location
*/
template<class T>
T* ADefaultMap::SpawnActorInMap(TSubclassOf<T> ToSpawn, FVector SpawnLocation)
{
	FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), SpawnLocation, FVector(1, 1, 1));

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ToSpawn) 
	{
		T* Spawned = GetWorld()->SpawnActor<T>(ToSpawn, SpawnTransform, ActorSpawnParams);
		return Spawned;
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No PointActorClass set!"), *GetName())
		return nullptr;
	}
}

// Reload distribution weights if changed
void ADefaultMap::ReloadSpawnDistributionWeights()
{
	for (size_t i = 0; i < SpawnParams.Num(); i++)
	{
		ParamList.push_back(SpawnParams[i].SpawningWeight);
	}
}

/* GetParamIndexByName: Check if spawn parameter with a given name exists in the map's database
	* @param Name, Name of the parameter to search
	* @param OutIndex, reference to index, if found
	* @returns bool if parameter is found
*/
bool ADefaultMap::GetParamIndexByName(FString Name, int32 & OutIndex)
{
	for (size_t i = 0; i < SpawnParams.Num(); i++)
	{
		if (SpawnParams[i].Name == Name)
		{
			OutIndex = i;
			return true;
		}
	}
	return false;
}

// Returns true if map has no points
bool ADefaultMap::IsMapEmpty()
{
	for (size_t i = 0; i < SpawnParams.Num(); i++)
	{
		if (SpawnParams[i].InMap > 0)
		{
			return false;
		}
	}

	return true;
}

// Returns index of actor forcing his spawn and the number of actors to spawn
int32 ADefaultMap::GetForcingSpawn(int32 & OutNumberToSpawn)
{
	for (size_t i = 0; i < SpawnParams.Num(); i++)
	{
		if (SpawnParams[i].ForceSpawn > 0)
		{
			OutNumberToSpawn = SpawnParams[i].ForceSpawn;
			return i;
		}
	}
	OutNumberToSpawn = 0;
	return -1;
}

// Called when the game starts or when spawned
void ADefaultMap::BeginPlay()
{
	Super::BeginPlay();	

	if (UGameplayStatics::GetGameMode(this))
	{
		AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));
		GameMode->DefaultMaxSpawnCooldown = MaxSpawnCooldown;
		GameMode->DefaultMinSpawnCooldown = MinSpawnCooldown;
	}

	NewPointCooldown = FMath::RandRange(MinSpawnCooldown, MaxSpawnCooldown);
	ReloadSpawnDistributionWeights();

	// Generate a new random point following custom distribution
	Distribution = std::discrete_distribution<int>(ParamList.begin(), ParamList.end());
	RandomGenerator = std::mt19937(std::random_device{}());

}

// Called every frame
void ADefaultMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnPointActor(DeltaTime);	
}

void ADefaultMap::SpawnPointActor(float DeltaTime)
{	
	ElapsedTime += DeltaTime;

	// Check if points are present in map
	PreventAutoSpawn = !IsMapEmpty();

	if (ElapsedTime >= NewPointCooldown)
	{
		UpdateForcePreventParams();

		if (!PreventAutoSpawn) 
		{
			FVector SpawnLocation;
			int32 ForcedToSpawn;
			int32 ForcingIndex = GetForcingSpawn(ForcedToSpawn);
			if (ForcingIndex != -1)
			{
				if (!SpawnParams[ForcingIndex].PreventSpawn && ChooseRandomLocation(SpawnLocation))
				{
					SpawnPointActorByIndex(ForcingIndex, SpawnLocation);
					SpawnParams[ForcingIndex].ForceSpawn--;
				}
			}
			else
			{
				int SpawnIndex = Distribution(RandomGenerator);

				//UE_LOG(LogTemp, Warning, TEXT("[%s] Spawning: %s"), *GetName(), *SpawnParams[SpawnIndex].Name);

				if (!SpawnParams[SpawnIndex].PreventSpawn && ChooseRandomLocation(SpawnLocation))
				{
					SpawnPointActorByIndex(SpawnIndex, SpawnLocation);
				}
				else if (SpawnParams[SpawnIndex].PreventSpawn && ChooseRandomLocation(SpawnLocation))
				{
					SpawnPointActorByIndex(0, SpawnLocation);
				}
			}			
		}

		NewPointCooldown = FMath::RandRange(MinSpawnCooldown, MaxSpawnCooldown);
		ElapsedTime = 0.f;
	}
}

void ADefaultMap::SpawnPointActorByIndex(int SpawnIndex, FVector SpawnLocation)
{
	// If spawn point found -> Spawn a point object at the location
	AScoringActor* SpawnedActor = SpawnActorInMap(SpawnParams[SpawnIndex].ActorClass, SpawnLocation);
	if (SpawnedActor)
	{
		LastPointActorSpawned = SpawnedActor;
		SpawnedActor->SpawnParamID = SpawnIndex;
		SpawnParams[SpawnIndex].InMap++;
		SpawnParams[SpawnIndex].Spawned++;
		PreventAutoSpawn = true;
	}
}

bool ADefaultMap::ChooseRandomLocation(FVector& OutLocation)
{
	const int MAX_TRIES = 60;
	FBox SpawnableArea;

	FVector BoundsLocation = SpawnBounds->GetComponentLocation();
	FVector BoundsExtent = SpawnBounds->GetUnscaledBoxExtent();
	FVector SpawnableAreaMin = FVector(BoundsLocation.X - BoundsExtent.X, BoundsLocation.Y - BoundsExtent.Y, BoundsLocation.Z - BoundsExtent.Z);
	FVector SpawnableAreaMax = FVector(BoundsLocation.X + BoundsExtent.X, BoundsLocation.Y + BoundsExtent.Y, BoundsLocation.Z + BoundsExtent.Z);
	SpawnableArea = FBox(SpawnableAreaMin, SpawnableAreaMax);

	for (size_t i = 0; i < MAX_TRIES; i++)
	{
		FVector RandPoint = FMath::RandPointInBox(SpawnableArea);

		RandPoint = ApproximateVectorComponents(RandPoint, 32);

		if (!IsLocationColliding(RandPoint))
		{
			OutLocation = RandPoint;
			return true;
		}	
	}
	return false;
}

bool ADefaultMap::IsLocationColliding(FVector Location)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	FVector BoxParams = FVector(14, 14, 1);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(BoxParams)
	);
	// FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	// DrawDebugBox(GetWorld(), GlobalLocation, BoxParams, ResultColor, true, 100);
	// UE_LOG(LogTemp, Warning, TEXT("[%s] Location is colliding: %s"), *GetName(), (HasHit ? TEXT("True") : TEXT("False")))
	return HasHit;
}

FVector ADefaultMap::ApproximateVectorComponents(FVector Vector, int Grid)
{
	int X = (int)Vector.X;
	int Y = (int)Vector.Y;
	int Z = (int)Vector.Z;

	int ModX = X % Grid;
	int ModY = Y % Grid;
	
	if (ModX <= (Grid - ModX))
	{
		X = X - ModX;
	}
	else
	{
		X = X + (Grid - ModX);
	}

	if (ModY <= (Grid - ModY))
	{
		Y = Y - ModY;
	}
	else
	{
		Y = Y + (Grid - ModY);
	}

	return FVector(X, Y, Z);
}

// This function contains all additional spawn conditions to let specific scoring actors to spawn
void ADefaultMap::UpdateForcePreventParams()
{
	// Every additional scoring actors can spawn only AFTER 'AllowCustomsAfter' default points
	if (SpawnParams[0].Spawned >= AllowCustomsAfter)
	{
		int32 ToUpdateIndex;

		// Boost Score
		if (GetParamIndexByName(FString("BoostPoint"), ToUpdateIndex) && SpawnParams[ToUpdateIndex].PreventSpawn)
		{
			SpawnParams[ToUpdateIndex].ForceSpawn = 1;
			SpawnParams[ToUpdateIndex].PreventSpawn = false;
		} 

		// Speed Boost
		if (GetParamIndexByName(FString("SpeedBoostPoint"), ToUpdateIndex) && SpawnParams[ToUpdateIndex].PreventSpawn)
		{
			SpawnParams[ToUpdateIndex].PreventSpawn = false;
		}

		// Point rain
		if (GetParamIndexByName(FString("PointRain"), ToUpdateIndex) && SpawnParams[ToUpdateIndex].PreventSpawn)
		{
			SpawnParams[ToUpdateIndex].PreventSpawn = false;
		}

		// Deceleration ability
		if (GetParamIndexByName(FString("DecelerationAbility"), ToUpdateIndex) && SpawnParams[ToUpdateIndex].PreventSpawn)
		{
			// Calculate how many base points the player has to take in order to activate spawn of this point type
			AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));
			float SpeedThreshold = 12.f;
			int32 PointThresold = AllowCustomsAfter + ( (SpeedThreshold - GameMode->StartingSpeed) / GameMode->SpeedIncrement );

			// If player took enough primary points to have [SpeedThresold] speed
			if ( SpawnParams[0].Spawned >= PointThresold )
			{
				// Enable spawning
				SpawnParams[ToUpdateIndex].ForceSpawn = 1;
				SpawnParams[ToUpdateIndex].PreventSpawn = false;
			}
		}
	}

	return;
}
