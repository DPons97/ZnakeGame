// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "SnakeBody.generated.h"

/**
 * 
 */
UCLASS()
class ZNAKEPROJECT_API ASnakeBody : public APaperCharacter
{
	GENERATED_BODY()
	
public:
	ASnakeBody();	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movements")
	FTransform LastTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
	ASnakeBody* NextBodyReference;

	UFUNCTION(BlueprintCallable)
	void MoveToNextTile(FTransform NextTransform);

	UFUNCTION(BlueprintCallable)
	ASnakeBody* AppendBodyPart(TSubclassOf<ASnakeBody> ToSpawn);


protected:


private:
	
	
};
