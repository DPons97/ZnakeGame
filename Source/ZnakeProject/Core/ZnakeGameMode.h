// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Saves/LeaderboardSaves.h"
#include "ZnakeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZNAKEPROJECT_API AZnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZnakeGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString PlayerName = FString("Player0");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float DefaultMinSpawnCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float DefaultMaxSpawnCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class ADefaultMap * CurrentMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scoreboard")
	ULeaderboardSaves * LeaderboardSave;

	UFUNCTION(BlueprintCallable, Category = "Params")
	void GenerateMap(TSubclassOf<ADefaultMap> Map);

	UFUNCTION(BlueprintCallable, Category = "Params")
	void UpdateSnakeSpeed();

	UFUNCTION(BlueprintCallable, Category = "Params")
	void UpdatePointSpawnSpeed();

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Params")
	float MaxSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Params")
	int32 MaxSpeedPoints = 400;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Params")
	int32 StepSize = 2;

	UFUNCTION(BlueprintCallable, Category = "Params")
	void SaveScoreToLeaderboard();

	UFUNCTION(BlueprintCallable, Category = "Params")
	void LoadLeaderboard();

private:


};
