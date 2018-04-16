// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMap.h"
#include "PaperTileMapComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ADefaultMap::ADefaultMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/* SpawnActorInMap: Spawns actor in map, given a specific location relative to the map.
	* @param ToSpawn, Subclass of spawning actor
	* @param OutActorRef, OUT new actor reference
	* @param SpawnLocation, Spawning location
*/
template<class T>
void ADefaultMap::SpawnActorInMap(TSubclassOf<T> ToSpawn, FVector SpawnLocation)
{
	FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), SpawnLocation, FVector(1, 1, 1));

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (ToSpawn) {
		GetWorld()->SpawnActor<T>(ToSpawn, SpawnTransform, ActorSpawnParams);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[%s] No PointActorClass set!"), *GetName())
	}
}

// Called when the game starts or when spawned
void ADefaultMap::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADefaultMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= NewPointCooldown)
	{
		// Generate a new random point		
		// If spawn point found -> Spawn a point object at the location
		if (EnableSpawning && ChooseRandomLocation(SpawnLocation)) {
			// UE_LOG(LogTemp, Warning, TEXT("[%s] Spawning new point"), *GetName())
			SpawnActorInMap(PointActorClass, SpawnLocation);
		}

		NewPointCooldown = FMath::RandRange(MinSpawnCooldown, MaxSpawnCooldown);
		ElapsedTime = 0.f;
	}
}

bool ADefaultMap::ChooseRandomLocation(FVector& OutLocation)
{
	const int MAX_TRIES = 60;

	for (size_t i = 0; i < MAX_TRIES; i++)
	{
		FVector RandPoint = FMath::RandPointInBox(MapPlayableBounds);
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

	int DiffX = X % Grid;
	int DiffY = Y % Grid;
	
	if (DiffX <= (Grid - DiffX))
	{
		X = X - DiffX;
	}
	else
	{
		X = X + (Grid - DiffX);
	}

	if (DiffY <= (Grid - DiffY))
	{
		Y = Y - DiffY;
	}
	else
	{
		Y = Y + (Grid - DiffY);
	}

	return FVector(X+GridOffset, Y+GridOffset, Z);
}
