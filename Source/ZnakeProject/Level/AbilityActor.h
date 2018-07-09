// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/PickableActor.h"
#include "AbilityActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityActorDelegate);


UCLASS()
class ZNAKEPROJECT_API AAbilityActor : public APickableActor
{
	GENERATED_BODY()
	
public:
	AAbilityActor();

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAbilityActorDelegate OnAbilityTaken;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAbilityActorDelegate OnAbilityActivated;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UPaperSprite * Icon;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void DestroyActor();

	UFUNCTION(BlueprintCallable)
	void ActivateAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:



};
