// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor.generated.h"

UCLASS()
class ZNAKEPROJECT_API APickableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickableActor();

	UPROPERTY(VisibleDefaultsOnly, Category = "Class")
	int32 SpawnParamID;

	UPROPERTY(VisibleDefaultsOnly, Category = "Defaults")
	USceneComponent * DefaultSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collisions")
	class UBoxComponent * HitBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ToBeDeleted = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
