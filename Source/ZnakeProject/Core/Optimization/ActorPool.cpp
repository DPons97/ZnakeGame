// Copyright SquirrelDevelopment - Game developed by DP

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AActor * UActorPool::Checkout()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Checkout."), *GetName())

	if (Pool.Num() == 0)
	{
		return nullptr;
	}

	return Pool.Pop();
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if (ActorToAdd == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Added null pointer"), *GetName())
		return;
	}
	Pool.EmplaceAt(0, ActorToAdd);
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Being added."), *ActorToAdd->GetName())
}
