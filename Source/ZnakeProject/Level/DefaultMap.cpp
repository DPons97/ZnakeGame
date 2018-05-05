// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMap.h"
#include "PaperTileMapComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"


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
void ADefaultMap::SpawnActorInMap(TSubclassOf<T> ToSpawn, FVector SpawnLocation)
{
	FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), SpawnLocation, FVector(1, 1, 1));

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ToSpawn) 
	{
		GetWorld()->SpawnActor<T>(ToSpawn, SpawnTransform, ActorSpawnParams);
	}
	else 
	{
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
