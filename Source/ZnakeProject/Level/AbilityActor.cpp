// Copyright SquirrelDevelopment - Game developed by DP

#include "AbilityActor.h"
#include "Level/PickableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ZnakeGameMode.h"
#include "Level/DefaultMap.h"


// Sets default values
AAbilityActor::AAbilityActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AAbilityActor::OnBeginOverlap);
}

void AAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityActor::ActivateAbility()
{
	OnAbilityActivated.Broadcast();
	DestroyActor();
}

void AAbilityActor::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")) && !ToBeDeleted)
	{
		AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));

		OnAbilityTaken.Broadcast();

		SetActorHiddenInGame(true);
		GameMode->EnqueueAbilityToPool(this);

		ToBeDeleted = true;
		GameMode->CurrentMap->SpawnParams[SpawnParamID].InMap--;	
	}
}

void AAbilityActor::DestroyActor()
{
	AZnakeGameMode * GameMode = Cast<AZnakeGameMode>(UGameplayStatics::GetGameMode(this));
	GameMode->AddToPool(this);
	GameMode->DequeueAbilityFromPool();
}
