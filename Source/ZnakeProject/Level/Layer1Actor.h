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
	UFUNCTION(BlueprintCallable)
		void TeleportTo(UPrimitiveComponent * TriggerComponent, class UTeleportTargetComponent * Target);

private:


	
	
};
