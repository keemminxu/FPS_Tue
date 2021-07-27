// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 충돌체 및 루트 등록
	collision=CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = collision;
	collision->SetCollisionProfileName(TEXT("BlockAll"));

	// 외관
	bodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(collision);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 무브먼트
	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	movement->SetUpdatedComponent(collision);

	// 튕기기
	movement->bShouldBounce = true;
	movement->Bounciness = 0.3f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3);
	
}

void ABullet::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
}

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// 만약 speed 가 변경됐으면
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		// movement 에 적용하고 싶다.
		// 속도
		movement->MaxSpeed = speed;
		movement->InitialSpeed = speed;
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

