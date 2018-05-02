// Copyright SquirrelDevelopment - Game developed by DP

#include "ZnakeGameMode.h"
#include "Engine/World.h"
#include "../Character/SnakeCharacter.h"
#include "../Level/DefaultMap.h"
#include "../Saves/LeaderboardSaves.h"
#include "Kismet/GameplayStatics.h"


AZnakeGameMode::AZnakeGameMode()
{
	
}

void AZnakeGameMode::BeginPlay()
{
	Super::BeginPlay();

	LoadLeaderboard();
}

void AZnakeGameMode::GenerateMap(TSubclassOf<ADefaultMap> Map)
{
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector(16, 16, 0), FVector(1, 1, 1));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Map) {
		CurrentMap = GetWorld()->SpawnActor<ADefaultMap>(Map, SpawnTransform, ActorSpawnParams);
	}	
}

void AZnakeGameMode::UpdateSnakeSpeed()
{	
	if (Score % StepSize == 0) {
		ASnakeCharacter* PlayerPawn = Cast<ASnakeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		// Speed increment calculation:
		// Difference between default speed (at Score = 0) and max speed to reach (at MaxSpeedPoints score)
		float SpeedIncrement = (MaxSpeed - DefaultSpeed) / ((MaxSpeedPoints - StepSize) / StepSize);

		// Applying the increment and updating to character
		PlayerPawn->Speed = FMath::Clamp(SpeedIncrement + PlayerPawn->Speed, DefaultSpeed, MaxSpeed);
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
