// Copyright SquirrelDevelopment - Game developed by DP

#include "SnakeBody.h"
#include "Engine/World.h"

ASnakeBody::ASnakeBody()
{
	Tags.Add(FName("Body"));
}

void ASnakeBody::MoveToNextTile(FTransform NextTransform)
{
	LastTransform = GetActorTransform();
	
	FHitResult HitResult;
	SetActorTransform(NextTransform, false, &HitResult, ETeleportType::TeleportPhysics);

	if (NextBodyReference)
	{
		NextBodyReference->MoveToNextTile(LastTransform);
	}

}

ASnakeBody* ASnakeBody::AppendBodyPart(TSubclassOf<ASnakeBody> ToSpawn)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ToSpawn)
	{
		ASnakeBody* NewBodyPart = GetWorld()->SpawnActor<ASnakeBody>(ToSpawn, GetActorTransform(), ActorSpawnParams);
		NewBodyPart->AddActorLocalOffset(FVector(-32,0,0), false, nullptr, ETeleportType::TeleportPhysics);
		NewBodyPart->MoveToNextTile(LastTransform);
		NextBodyReference = NewBodyPart;
		return NextBodyReference;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No body class set!"), *GetName())
		return nullptr;
	}

}
