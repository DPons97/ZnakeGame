// Copyright SquirrelDevelopment - Game developed by DP

#include "Layer1Actor.h"
#include "TeleportTargetComponent.h"
#include "Character/SnakeCharacter.h"
#include "PaperSpriteComponent.h"

ALayer1Actor::ALayer1Actor()
{
	AbilitySlot1 = CreateDefaultSubobject<UPaperSpriteComponent>(FName("AbilitySlot1"));
	AbilitySlot1->SetupAttachment(RootComponent);

	AbilitySlot2 = CreateDefaultSubobject<UPaperSpriteComponent>(FName("AbilitySlot2"));
	AbilitySlot2->SetupAttachment(RootComponent);

	AbilitySlot3 = CreateDefaultSubobject<UPaperSpriteComponent>(FName("AbilitySlot3"));
	AbilitySlot3->SetupAttachment(RootComponent);

	AbilitySlot4 = CreateDefaultSubobject<UPaperSpriteComponent>(FName("AbilitySlot4"));
	AbilitySlot4->SetupAttachment(RootComponent);

	AbilitySlot5 = CreateDefaultSubobject<UPaperSpriteComponent>(FName("AbilitySlot5"));
	AbilitySlot5->SetupAttachment(RootComponent);

}

void ALayer1Actor::BeginPlay()
{
	Super::BeginPlay();

}

void ALayer1Actor::TeleportTo(UPrimitiveComponent * TriggerComponent, UTeleportTargetComponent * Target)
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

TArray<UPaperSpriteComponent*> ALayer1Actor::GetAbilitySpritePool()
{
	TArray<UPaperSpriteComponent*> TempPool;
	TempPool.Add(AbilitySlot1);
	TempPool.Add(AbilitySlot2);
	TempPool.Add(AbilitySlot3);
	TempPool.Add(AbilitySlot4);
	TempPool.Add(AbilitySlot5);

	return TempPool;
}


