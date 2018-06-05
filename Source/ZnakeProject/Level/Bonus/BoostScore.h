// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "Level/ScoringActor.h"
#include "BoostScore.generated.h"

/**
 * 
 */
UCLASS()
class ZNAKEPROJECT_API ABoostScore : public AScoringActor
{
	GENERATED_BODY()

	// int32 MinScoreToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
};
