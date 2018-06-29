// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoringActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScoringActorDelegate);

UCLASS()
class ZNAKEPROJECT_API AScoringActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoringActor();

	UPROPERTY(VisibleDefaultsOnly, Category = "Class")
	int32 SpawnParamID;

	UPROPERTY(VisibleDefaultsOnly, Category = "Defaults")
	USceneComponent * DefaultSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collisions")
	class UBoxComponent * HitBox;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FScoringActorDelegate OnScoreTaken;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ScoreValue = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsSecondaryScoreType = false;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ToBeDeleted = false;

	void DestroyActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
