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
	ASnakeCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float AbilityCooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FTransform LastTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<class ASnakeBody> SnakeBodyClass;	

	UFUNCTION(BlueprintCallable)
	void AddBodyPart();

	UFUNCTION(BlueprintCallable)
	void MoveToTile(FTransform NextTransform);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	TArray<ASnakeBody *> BodyReferences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	ASnakeBody * NextBodyRef;

};
