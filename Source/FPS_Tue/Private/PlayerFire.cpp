// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "FPSPlayer.h"
#include <Components/ArrowComponent.h>
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include <Camera/CameraComponent.h>

// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}


// Called when the game starts
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UPlayerFire::InitializeComponent()
{
	Super::InitializeComponent();

	//begine ���ִ� me �̵�
	me = Cast<AFPSPlayer>(GetOwner());
	if (me)
	{
		//��������Ʈ�� �� �Լ��� ����ϰ� �ʹ�.
		me->OnInputDelegate.AddUObject(this, &UPlayerFire::SetupPlayerInputComponent);
		// �ѱ� ã�Ƽ� ���
		firePosition =  Cast<UArrowComponent>(me->GetDefaultSubobjectByName(TEXT("FirePosition")));
	}
}

// Called every frame
void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerFire::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::Fire);
}

void UPlayerFire::Fire()
{
	// GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), firePosition->GetComponentRotation());

	// Line ���� �ε��� ������ ȿ���� ����ϰ� �ʹ�.
	// �ʿ�Ӽ� : ȿ��
	// �ε��� ������ ���� ������ �ʿ�
	
	FHitResult hitinfo;

	// �ѱ��� �ƴ� ī�޶� �������� ����
	auto fpsCam = Cast<UCameraComponent>(me->GetDefaultSubobjectByName(TEXT("FPSCamera")));
	
	FVector start = firePosition->GetComponentLocation();
	FVector end = firePosition->GetComponentLocation() + firePosition->GetForwardVector() * fireDistance;
	
	FCollisionQueryParams param;
	param.AddIgnoredActor(me);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitinfo, start, end, ECC_Pawn, param);

	if (bHit)
	{
		FTransform trans;
		trans.SetLocation(hitinfo.ImpactPoint);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, trans);

		auto enemy = Cast<AEnemy>(hitinfo.GetActor());

		if (enemy)
		{
			// Enemy�� ���¸� Damage�� ��ȯ�Ѵ�.
			enemy->enemyFSM->OnDamageProcess(fpsCam->GetForwardVector());
		}
	}
}

