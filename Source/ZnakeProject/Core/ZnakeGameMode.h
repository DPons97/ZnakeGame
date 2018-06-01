// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Saves/LeaderboardSaves.h"
#include "ZnakeGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameModeDelegate);

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
	int32 PrimaryScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 SecondaryScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 UIScore;

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
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FGameModeDelegate OnScoreIncreased;

	UFUNCTION(BlueprintCallable, Category = "Params")
	void GenerateMap(TSubclassOf<ADefaultMap> Map);

	UFUNCTION(BlueprintCallable, Category = "Params")
	void UpdateSnakeSpeed();

	UFUNCTION(BlueprintCallable, Category = "Params")
	void UpdatePointSpawnSpeed();

	UFUNCTION()
	void IncreaseScore(int32 Increment, bool IsSecondary);

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Params")
	float MaxSpeed = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Params")
	int32 MaxSpeedPoints = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Params")
	int32 StepSize = 20;

	UFUNCTION(BlueprintCallable, Category = "Params")
	void SaveScoreToLeaderboard();

	UFUNCTION(BlueprintCallable, Category = "Params")
	void LoadLeaderboard();

private:
	

};
