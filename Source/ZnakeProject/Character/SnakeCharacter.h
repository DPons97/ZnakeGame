// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "SnakeCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZNAKEPROJECT_API ASnakeCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 10.5f;

	
	
};
