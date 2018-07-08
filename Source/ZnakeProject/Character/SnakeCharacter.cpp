// Copyright SquirrelDevelopment - Game developed by DP

#include "SnakeCharacter.h"
#include "SnakeBody.h"

ASnakeCharacter::ASnakeCharacter()
{
	
}

void ASnakeCharacter::AddBodyPart()
{
	if (SnakeBodyClass)
	{
		ASnakeBody * NewBodyPart = BodyReferences.Last()->AppendBodyPart(SnakeBodyClass);
		BodyReferences.Add(NewBodyPart);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No body class set!"), *GetName())
	}

}

void ASnakeCharacter::MoveToTile(FTransform NextTransform)
{
	LastTransform = GetActorTransform();

	FHitResult HitResult;
	SetActorTransform(NextTransform, false, &HitResult, ETeleportType::TeleportPhysics);

	if (NextBodyRef)
	{
		NextBodyRef->MoveToNextTile(LastTransform);
	}

}