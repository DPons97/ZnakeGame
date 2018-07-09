// Copyright SquirrelDevelopment - Game developed by DP

#include "ZnakeGameMode.h"
#include "Engine/World.h"
#include "../Character/SnakeCharacter.h"
#include "../Level/DefaultMap.h"
#include "../Saves/LeaderboardSaves.h"
#include "Kismet/GameplayStatics.h"
#include "Optimization/ActorPool.h"
#include "Level/PickableActor.h"
#include "Level/AbilityActor.h"
#include "EngineUtils.h"


AZnakeGameMode::AZnakeGameMode()
{
	DeletedPickableActorPool = CreateDefaultSubobject<UActorPool>(TEXT("Pool"));
}

void AZnakeGameMode::BeginPlay()
{
	Super::BeginPlay();

	LoadLeaderboard();
}

ADefaultMap * AZnakeGameMode::GenerateMap(TSubclassOf<ADefaultMap> Map)
{
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector(0, 0, 0), FVector(1, 1, 1));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Map) {
		CurrentMap = GetWorld()->SpawnActor<ADefaultMap>(Map, SpawnTransform, ActorSpawnParams);
		return CurrentMap;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Map class set!"))
		return nullptr;
	}
}

void AZnakeGameMode::UpdateSnakeSpeed()
{	
	if (PrimaryScore % StepSize == 0) {
		ASnakeCharacter* PlayerPawn = Cast<ASnakeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		
		// Speed increment calculation:
		SpeedIncrement = ((MaxSpeed - StartingSpeed) * StepSize) / MaxSpeedPoints;

		// Applying the increment and updating to character
		PlayerPawn->Speed = FMath::Clamp(SpeedIncrement + PlayerPawn->Speed, MinSpeed, MaxSpeed);
	}
}

void AZnakeGameMode::UpdatePointSpawnSpeed()
{
	if (Score % StepSize == 0) {
		ASnakeCharacter* PlayerPawn = Cast<ASnakeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		float AvgCooldown = (DefaultMinSpawnCooldown - DefaultMaxSpawnCooldown) / 2;
		float Decrement =  AvgCooldown / (MaxSpeedPoints - StepSize);
		CurrentMap->MaxSpawnCooldown = FMath::Clamp(CurrentMap->MaxSpawnCooldown - Decrement, 0.f, DefaultMaxSpawnCooldown);
		CurrentMap->MinSpawnCooldown = FMath::Clamp(CurrentMap->MinSpawnCooldown - Decrement, 0.f, DefaultMinSpawnCooldown);

	}
}

void AZnakeGameMode::IncreaseScore(int32 Increment, bool IsSecondary)
{
	if (IsSecondary)
	{
		SecondaryScore += Increment;
		Score += Increment;
		OnScoreIncreased.Broadcast();
	}
	else
	{
		PrimaryScore += Increment;
		Score += Increment;
		OnScoreIncreased.Broadcast();
	}
}

void AZnakeGameMode::SaveScoreToLeaderboard()
{
	if (LeaderboardSave)
	{
		LeaderboardSave->AddScoreToLeaderboard(PlayerName, Score);
		LeaderboardSave->SaveLeaderboard();
		UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSave->SaveSlotName, LeaderboardSave->UserIndex);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Load/Create a savegame before saving!"))
	}
	
}

void AZnakeGameMode::LoadLeaderboard()
{
	LeaderboardSave = Cast<ULeaderboardSaves>(UGameplayStatics::CreateSaveGameObject(ULeaderboardSaves::StaticClass()));
	LeaderboardSave = Cast<ULeaderboardSaves>(UGameplayStatics::LoadGameFromSlot(LeaderboardSave->SaveSlotName, LeaderboardSave->UserIndex));

	if (!LeaderboardSave)
	{
		LeaderboardSave = Cast<ULeaderboardSaves>(UGameplayStatics::CreateSaveGameObject(ULeaderboardSaves::StaticClass()));
		UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSave->SaveSlotName, LeaderboardSave->UserIndex);
		LeaderboardSave = Cast<ULeaderboardSaves>(UGameplayStatics::LoadGameFromSlot(LeaderboardSave->SaveSlotName, LeaderboardSave->UserIndex));
		UE_LOG(LogTemp, Warning, TEXT("No savegame found: creating new one..."))
	}

	LeaderboardSave->LoadLeaderboard();
}

void AZnakeGameMode::AddToPool(APickableActor * Actor)
{
	DeletedPickableActorPool->Add(Actor);
	UpdateDeletedPool();
}

void AZnakeGameMode::EnqueueAbilityToPool(AAbilityActor * Ability)
{
	if (AbilityPool.Num() < MaxAbilityStack)
	{
		int32 index = AbilityPool.Add(Ability);
		CurrentMap->EnqueueAbilitySprite(Ability, index);
	}
	else
	{
		Ability->DestroyActor();
	}
}

void AZnakeGameMode::DequeueAbilityFromPool()
{
	if (AbilityPool[0])
	{
		AbilityPool.Pop();
		CurrentMap->DequeueAbilitySprite();
	}
}

void AZnakeGameMode::UpdateDeletedPool()
{
	int NumberOfActors = 0;
	auto ScoreIterator = TActorIterator<APickableActor>(GetWorld());
	while (ScoreIterator)
	{
		NumberOfActors++;
		++ScoreIterator;
	}

	if (NumberOfActors >= DeletedPickableActorPool->MaxActorsInPool)
	{
		auto DeletingActor = DeletedPickableActorPool->Checkout();
		DeletingActor->Destroy();
	}
}

int32 AZnakeGameMode::GetMaxSpeedPoints() const
{
	return MaxSpeedPoints;
}

float AZnakeGameMode::GetMaxSpeed() const
{
	return MaxSpeed;
}

int32 AZnakeGameMode::GetStepSize() const
{
	return StepSize;
}
