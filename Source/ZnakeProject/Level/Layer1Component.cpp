// Copyright SquirrelDevelopment - Game developed by DP

#include "Layer1Component.h"
#include "TeleportTargetComponent.h"
#include "Character/SnakeCharacter.h"


void ULayer1Component::TeleportTo(UPrimitiveComponent * TriggerComponent, UTeleportTargetComponent * Target)
{
	if (Target)
	{
		FTransform TargetTransform = Target->GetComponentTransform();
		ASnakeCharacter* PlayerPawn = Cast<ASnakeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		PlayerPawn->MoveToTile(TargetTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No target set!"), *TriggerComponent->GetName())
	}
	
}

