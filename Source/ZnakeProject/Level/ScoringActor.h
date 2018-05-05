// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoringActor.generated.h"


UCLASS()
class ZNAKEPROJECT_API AScoringActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoringActor();

	UPROPERTY(VisibleDefaultsOnly, Category = "Defaults")
	USceneComponent * DefaultSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collisions")
	class UBoxComponent * HitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ScoreValue = 10;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ToBeDeleted = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
