// Copyright SquirrelDevelopment - Game developed by DP

#include "ScoringActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "../Core/ZnakeGameMode.h"
#include "../Character/SnakeCharacter.h"
#include "DefaultMap.h"


// Sets default values
AScoringActor::AScoringActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultScene"));
	DefaultSceneComponent->SetupAttachment(RootComponent);

	RootComponent = DefaultSceneComponent;

	HitBox = CreateDefaultSubobject<UBoxComponent>(FName("HitBox"));
	HitBox->SetupAttachment(DefaultSceneComponent);
	HitBox->SetCollisionProfileName(FName("Trigger"));
	HitBox->SetRelativeLocation(FVector(0, 0, 50));
	HitBox->SetBoxExtent(FVector(10, 10, 50));
	// Generates Hit Event?

}

// Called when the game starts or when spawned
void AScoringActor::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AScoringActor::OnBeginOverlap);

}

// Called every frame
void AScoringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoringActor::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")) && !ToBeDeleted)
	{
		AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));
		GameMode->IncreaseScore(ScoreValue, IsSecondaryScoreType);

		// Primary score increases speed
		if (!IsSecondaryScoreType)
		{
			Cast<ASnakeCharacter>(OtherActor)->AddBodyPart();
			GameMode->UpdateSnakeSpeed();
		}
		// Both primary and secondary score increases spawn
		GameMode->UpdatePointSpawnSpeed();
		
		DestroyActor();
	}
}

void AScoringActor::DestroyActor()
{
	AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));

	ToBeDeleted = true;
	GameMode->CurrentMap->SpawnParams[SpawnParamID].InMap--;

	OnScoreTaken.Broadcast();

	SetActorHiddenInGame(true);

	GameMode->AddToPool(this);
}
