// Copyright SquirrelDevelopment - Game developed by DP

#include "LeaderboardSaves.h"
#include "Archive.h"
#include "FileManager.h"
#include "BufferArchive.h"
#include "FileHelper.h"
#include "MemoryReader.h"
#include "Paths.h"


ULeaderboardSaves::ULeaderboardSaves()
{
	SaveSlotName = TEXT("Leaderboard");
	UserIndex = 0;
}

void ULeaderboardSaves::LoadLeaderboard()
{
	FString RelativePath = FPaths::ProjectSavedDir();
	FString FilePath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*RelativePath).Append(FString("SaveGames/Binary/LeaderboardArray.save"));

	LoadGameDataFromFile(FilePath, Leaderboard);
	SortLeaderboard();
}

FString ULeaderboardSaves::GetBestPlayerName() const
{
	FPlayerStats BestPlayer;
	if (Leaderboard.IsValidIndex(0)) {
		BestPlayer = Leaderboard[0];
		return BestPlayer.PlayerName;
	}
	else
	{
		// UE_LOG(LogTemp, Error, TEXT("Error: couldn't get best player name"))
		return FString("None");
	}
}

int32 ULeaderboardSaves::GetBestScore() const
{
	FPlayerStats BestPlayer;
	if (Leaderboard.IsValidIndex(0)) {
		BestPlayer = Leaderboard[0];
		return BestPlayer.Score;
	}
	else
	{
		// UE_LOG(LogTemp, Error, TEXT("Error: couldn't get best score"))
		return 0;
	}
}

FString ULeaderboardSaves::GetPlayerName(int Index) const
{
	if (Leaderboard.IsValidIndex(Index))
	{
		return Leaderboard[Index].PlayerName;
	}
	else return FString("None");
	
}

int32 ULeaderboardSaves::GetScore(int Index) const
{
	if (Leaderboard.IsValidIndex(Index))
	{
		return Leaderboard[Index].Score;
	}
	else return 0;
}

int32 ULeaderboardSaves::GetLeaderboardLength() const
{
	return Leaderboard.Num();
}

void ULeaderboardSaves::SaveLeaderboard()
{
	FString RelativePath = FPaths::ProjectSavedDir();
	FString FilePath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*RelativePath).Append(FString("SaveGames/Binary/LeaderboardArray.save"));

	UE_LOG(LogTemp, Log, TEXT("Saving data in: %s"), *FilePath)
	
	if (SaveGameDataToFile(FilePath, Leaderboard))
	{
		UE_LOG(LogTemp, Log, TEXT("Data saved successfully!"))
	}
}

void ULeaderboardSaves::SaveLoadData(FArchive& Ar, TArray<FPlayerStats>& SaveDataArray)
{
	Ar << SaveDataArray;
}

bool ULeaderboardSaves::SaveGameDataToFile(const FString & FullFilePath, TArray<FPlayerStats>& SaveDataArray)
{
	FBufferArchive ToBinary;
	SaveLoadData(ToBinary, SaveDataArray);

	if (ToBinary.Num() <= 0) return false;

	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		ToBinary.FlushCache();
		ToBinary.Empty();

		UE_LOG(LogTemp, Log, TEXT("Save Success!"))

		return true;
	}

	ToBinary.FlushCache();
	ToBinary.Empty();

	UE_LOG(LogTemp, Warning, TEXT("File could not be saved!"))

	return false;
}

bool ULeaderboardSaves::LoadGameDataFromFile(const FString & FullFilePath, TArray<FPlayerStats>& SaveDataArray)
{
	TArray<uint8> BinaryArray;

	if (!FFileHelper::LoadFileToArray(BinaryArray, *FullFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("FFILEHELPER:>> Invalid File"))
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Loaded File Size: %d"), BinaryArray.Num())

	if (BinaryArray.Num() <= 0) return false;

	FMemoryReader FromBinary = FMemoryReader(BinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	SaveLoadData(FromBinary, SaveDataArray);

	FromBinary.FlushCache();

	BinaryArray.Empty();
	FromBinary.Close();

	return true;
}

void ULeaderboardSaves::AddScoreToLeaderboard(FString PlayerName, int32 Score)
{
	FPlayerStats NewPlayer;
	NewPlayer.PlayerName = PlayerName;
	NewPlayer.Score = Score;
	
	Leaderboard.Add(NewPlayer);
	SortLeaderboard();
}

void ULeaderboardSaves::SortLeaderboard()
{
	Leaderboard.Sort([](const FPlayerStats& A, const FPlayerStats& B) {
		return A.Score > B.Score;
	});

	if (Leaderboard.Num() > MaxLeaderboardLength)
	{
		Leaderboard.RemoveAt(Leaderboard.Num()-1);
	}
}
