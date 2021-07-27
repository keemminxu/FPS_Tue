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
	
	// �浹ü �� ��Ʈ ���
	collision=CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = collision;
	collision->SetCollisionProfileName(TEXT("BlockAll"));

	// �ܰ�
	bodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(collision);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �����Ʈ
	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	movement->SetUpdatedComponent(collision);

	// ƨ���
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
	// ���� speed �� ���������
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		// movement �� �����ϰ� �ʹ�.
		// �ӵ�
		movement->MaxSpeed = speed;
		movement->InitialSpeed = speed;
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

