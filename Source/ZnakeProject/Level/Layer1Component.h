// Copyright SquirrelDevelopment - Game developed by DP

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapComponent.h"
#include "Layer1Component.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class ZNAKEPROJECT_API ULayer1Component : public UPaperTileMapComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void TeleportTo(UPrimitiveComponent * TriggerComponent, class UTeleportTargetComponent * Target);

private:
	
	
};
