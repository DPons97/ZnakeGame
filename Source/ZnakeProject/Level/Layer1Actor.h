// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "Layer1Actor.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class ZNAKEPROJECT_API ALayer1Actor : public APaperTileMapActor
{
	GENERATED_BODY()
	
public:
	ALayer1Actor();

	UFUNCTION(BlueprintCallable)
	void TeleportTo(UPrimitiveComponent * TriggerComponent, class UTeleportTargetComponent * Target);

	UPROPERTY(VisibleDefaultsOnly, Category = "Defaults")
	USceneComponent * DefaultSceneComponent;

	UFUNCTION(BlueprintCallable)
	TArray<UPaperSpriteComponent*> GetAbilitySpritePool();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UPaperSpriteComponent * AbilitySlot1;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UPaperSpriteComponent * AbilitySlot2;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UPaperSpriteComponent * AbilitySlot3;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UPaperSpriteComponent * AbilitySlot4;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UPaperSpriteComponent * AbilitySlot5;

	void BeginPlay();

private:


	
	
};
