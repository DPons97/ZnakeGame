// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LeaderboardSaves.generated.h"

USTRUCT()
struct FPlayerStats 
{
	GENERATED_BODY()

	FString PlayerName;
	int32 Score;
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FPlayerStats& SaveGameData)
{
	//if (!SaveGameData) return Ar;
	//~

	Ar << SaveGameData.PlayerName;
	Ar << SaveGameData.Score;

	return Ar;
}

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class ZNAKEPROJECT_API ULeaderboardSaves : public USaveGame
{
	GENERATED_BODY()
	
public:
	TArray<FPlayerStats> Leaderboard;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	int32 UserIndex;

	ULeaderboardSaves();
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	FString GetBestPlayerName() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetBestScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	FString GetPlayerName(int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore(int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScoreToLeaderboard(FString PlayerName, int32 Score);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetLeaderboardLength() const;

	void SaveLeaderboard();

	void LoadLeaderboard();

private:
	int32 MaxLeaderboardLength = 15;

	void SortLeaderboard();

	void SaveLoadData(FArchive& Ar, TArray<FPlayerStats>& SaveDataArray);

	bool SaveGameDataToFile(const FString& FullFilePath, TArray<FPlayerStats>& SaveDataArray);

	bool LoadGameDataFromFile(const FString& FullFilePath, TArray<FPlayerStats>& SaveDataArray);

};
