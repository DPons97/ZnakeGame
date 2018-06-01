// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMap.h"
#include "PaperTileMapComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Core/ZnakeGameMode.h"
#include "ScoringActor.h"
#include <iostream>
#include <string>
#include <random>


// Sets default values
ADefaultMap::ADefaultMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultScene"));
	DefaultSceneComponent->SetupAttachment(RootComponent);

	RootComponent = DefaultSceneComponent;

	PlayableBounds = CreateDefaultSubobject<UBoxComponent>(FName("PlaygroundBox"));
	PlayableBounds->SetupAttachment(DefaultSceneComponent);
	PlayableBounds->SetCollisionProfileName(FName("NoCollision"));

}

/* SpawnActorInMap: Spawns actor in map, given a specific location relative to the map.
	* @param ToSpawn, Subclass of spawning actor
	* @param OutActorRef, OUT new actor reference
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

// Called when the game starts or when spawned
void ADefaultMap::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::GetGameMode(this))
	{
		AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));
		GameMode->DefaultMaxSpawnCooldown = SpawnParams[0].MaxSpawnCooldown;
		GameMode->DefaultMinSpawnCooldown = SpawnParams[0].MinSpawnCooldown;
	}

}

// Called every frame
void ADefaultMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnPointActors(DeltaTime);	
}

void ADefaultMap::SpawnPointActors(float DeltaTime)
{	
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= (1 / CheckSpawnSpeed))
	{
		for (size_t i = 0; i < SpawnParams.Num(); i++)
		{
			SpawnParams[i].ElapsedTime += ElapsedTime;
			//UE_LOG(LogTemp, Warning, TEXT("[%s] ElapsedTime for %s = %f"), *GetName(), *SpawnParams[i].Name, SpawnParams[i].ElapsedTime)

			if (SpawnParams[i].ElapsedTime >= SpawnParams[i].NewPointCooldown)
			{
				// Generate a new random point		
				// If spawn point found -> Spawn a point object at the location
				if (SpawnParams[i].ActorsInMap < SpawnParams[i].MaxActorsInMap && ChooseRandomLocation(SpawnLocation))
				{
					// UE_LOG(LogTemp, Warning, TEXT("[%s] Spawning new point"), *GetName())
					AScoringActor* SpawnedActor = SpawnActorInMap(SpawnParams[i].ActorClass, SpawnLocation);
					SpawnedActor->ID = i;
					SpawnParams[i].ActorsInMap++;
				}

				// Generates new spawn cooldown using Gaussian Distribution
				float Mean = (SpawnParams[i].MinSpawnCooldown + SpawnParams[i].MaxSpawnCooldown) / 2;
				float StdDev = FMath::Sqrt((FMath::Pow(SpawnParams[i].MinSpawnCooldown - Mean, 2) + (FMath::Pow(SpawnParams[i].MaxSpawnCooldown - Mean, 2))) / 2);

				std::default_random_engine generator;
				std::normal_distribution<double> distribution(Mean, StdDev);

				SpawnParams[i].NewPointCooldown = FMath::Abs(distribution(generator));
				SpawnParams[i].ElapsedTime = 0.f;
			}
		}

		ElapsedTime = 0.f;
	}
}

bool ADefaultMap::ChooseRandomLocation(FVector& OutLocation)
{
	const int MAX_TRIES = 60;
	FBox SpawnableArea;

	FVector BoundsLocation = PlayableBounds->GetComponentLocation();
	FVector BoundsExtent = PlayableBounds->GetUnscaledBoxExtent();
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
	UE_LOG(LogTemp, Warning, TEXT("Point to approximate: %s"), *FVector(X,Y,Z).ToString())

	int ModX = X % Grid;
	int ModY = Y % Grid;
	UE_LOG(LogTemp, Warning, TEXT("Resto della divisione X = %d ; Y = %d"), ModX, ModY)
	
	if (ModX <= (Grid - ModX))
	{
		X = X - ModX;
		UE_LOG(LogTemp, Warning, TEXT("Resto X minore (o uguale) della metà della griglia. Nuova X = %d"), X)
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Resto X maggiore della metà della griglia. Nuova X = %d"), X)
		X = X + (Grid - ModX);
	}

	if (ModY <= (Grid - ModY))
	{
		Y = Y - ModY;
		UE_LOG(LogTemp, Warning, TEXT("Resto Y minore (o uguale) della metà della griglia. Nuova Y = %d"), Y)
	}
	else
	{
		Y = Y + (Grid - ModY);
		UE_LOG(LogTemp, Warning, TEXT("Resto Y maggiore della metà della griglia. Nuova Y = %d"), Y)
	}

	UE_LOG(LogTemp, Warning, TEXT("Punto approssimato: %s"), *FVector(X + GridOffset, Y + GridOffset, Z).ToString())
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------------------"))

	return FVector(X+GridOffset, Y+GridOffset, Z);
}
