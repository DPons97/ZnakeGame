// Copyright SquirrelDevelopment - Game developed by DP

#include "PickableActor.h"
#include "Components/BoxComponent.h"


// Sets default values
APickableActor::APickableActor()
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
}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

