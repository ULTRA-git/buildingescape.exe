// Fill out your copyright notice in the Description page of Project Settings.

#include "GRABBER.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT 

// Sets default values for this component's properties
UGRABBER::UGRABBER()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGRABBER::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();

	SetupInputComponent();
	
}

void UGRABBER::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGRABBER::GRAB);
		InputComponent->BindAction("Release", IE_Released, this, &UGRABBER::RELEASE);
	}
}

void UGRABBER::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("NO PHYSICS HANDLE COMPONENT FOUND ON %s!"), * GetOwner()->GetName());
	}
}

void UGRABBER::GRAB()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent * ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor()) 
	{
		PhysicsHandle-> GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}

}

void UGRABBER::RELEASE()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));

	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGRABBER::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGRABBER::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult Hit;

	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner()
	);

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor * ActorHit = Hit.GetActor();
	 
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Has Hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}