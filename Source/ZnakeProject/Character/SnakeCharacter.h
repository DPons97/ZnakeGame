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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 DecelerationPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AbilityDeltaSpeed = -15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AbilityDuration = 5.f;

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
