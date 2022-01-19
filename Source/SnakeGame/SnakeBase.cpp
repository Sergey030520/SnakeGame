// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 10.f;
	LastMoveDirection = EMovementDirection::UP;
}


void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(5);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();

}

void ASnakeBase::AddSnakeElement(int ELementsNum)
{	
	for (int i = 0; i < ELementsNum; ++i) {
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		int32 ElementIndex = SnakeElements.Add(NewSnakeElem);
		if (ElementIndex == 0) {
			NewSnakeElem->SetFirstElementType();
		}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	float LocalMovementSpeed = ElementSize;
	switch (LastMoveDirection) {
	case EMovementDirection::UP:
		MovementVector.X += LocalMovementSpeed;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= LocalMovementSpeed;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += LocalMovementSpeed;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= LocalMovementSpeed;
		break;
	}
	//AddActorWorldOffset(MovementVector);
	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
}



