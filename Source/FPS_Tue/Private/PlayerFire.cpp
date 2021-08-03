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

	//begine 에있는 me 이동
	me = Cast<AFPSPlayer>(GetOwner());
	if (me)
	{
		//델리게이트에 내 함수를 등록하고 싶다.
		me->OnInputDelegate.AddUObject(this, &UPlayerFire::SetupPlayerInputComponent);
		// 총구 찾아서 등록
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

	// Line 쏴서 부딪힌 지점에 효과를 재생하고 싶다.
	// 필요속성 : 효과
	// 부딪힌 정보를 담을 변수가 필요
	
	FHitResult hitinfo;

	// 총구가 아닌 카메라 기준으로 수정
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
			// Enemy의 상태를 Damage로 전환한다.
			enemy->enemyFSM->OnDamageProcess(fpsCam->GetForwardVector());
		}
	}
}

