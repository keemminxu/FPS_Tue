// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "FPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>
#include "Enemy.h"
#include <DrawDebugHelpers.h>
#include "FPS_Tue.h"
#include <TimerManager.h>
#include <AIController.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	enemy=Cast<AEnemy>(GetOwner());

	target = Cast<AFPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass()));

	ai = Cast<AAIController>(enemy->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*dtdt = DeltaTime;*/
	// 목차(뼈대)를 구성

	switch (m_state)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void UEnemyFSM::IdleState()
{
	// 1. 상태를 Move로 전환한다.
		// 2. 대기시간이 되었으니까.
		// 3. 시간이 경과했으니까.

		// 1. 시간이 흘러서
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 2. 대기시간이 됐으니까
		// -> 만약 경과시간이 대기시간을 초과하였다면
	if (currentTime > idleDelayTime)
	{
		// 3. 상태를 Move로 전환하고 싶다.
		m_state = EEnemyState::Move;

		// 4. 다른 곳에서 사용할 수 있으니 경과시간을 초기화 시켜주자.
		currentTime = 0;
	}
}

// 필요속성 : 타겟, 이동속도
void UEnemyFSM::MoveState()
{
	// 타겟 방향으로 이동하고 싶다.
	
	
	//// 타겟 찾기
	//enemy = Cast<AEnemy>(GetOwner());

	//target = UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass());

	//dir = FVector(1, 1, 0) * moveSpeed;

	//if (target)
	//{
	//	dir = target->GetActorLocation() - enemy->GetActorLocation();
	//	dir.Normalize();
	//	dir *= moveSpeed;
	//}


	/*TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSPlayer::StaticClass(), actors);*/
	
	/*for (auto t : actors)
	{
		target = Cast<AFPSPlayer>(t);
		break;
	}*/

	/*for (TActorIterator<AFPSPlayer> it(GetWorld()); it; ++it)
	{
		target = *it;
	}*/

	// 1. 타겟이 있어야 한다.
   // 2. 방향이 필요 direction = target - me
	// 3. 이동하고 싶다.
	FVector direction = target->GetActorLocation() - enemy->GetActorLocation();
	float distance = direction.Size();
	direction.Normalize();

	if (ai)
	{
		ai->MoveToActor(target);
	}

	// enemy->AddMovementInput(direction, 1);

	/*FRotator targetRot = direction.ToOrientationRotator();
	FRotator myRot = enemy->GetActorRotation();

	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);

	enemy->SetActorRotation(myRot);*/

	DrawDebugSphere(GetWorld(), enemy->GetActorLocation(), attackRange, 10, FColor::Red);

	// 상태를 공격으로 전환하고 싶다.
	// 타겟과의 거리가 공격범위 안에 들어왔으니까.
	// 타겟과의 거리는??

	if (distance < attackRange)
	{
		m_state = EEnemyState::Attack;
		currentTime = attackDelayTime;

		ai->StopMovement();
	}



	/*FVector P0 = GetOwner()->GetActorLocation();
	FVector P = P0 + direction * 500 * GetWorld()->DeltaTimeSeconds;
	GetOwner()->SetActorLocation(P, true);*/

	/*enemy = Cast<AEnemy>(GetOwner());

	target = UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass());

	dir = FVector(1, 1, 0) * moveSpeed;

	if (target)
	{
		dir = target->GetActorLocation() - enemy->GetActorLocation();
		dir.Normalize();
		dir *= moveSpeed;
	}

	FVector P = enemy->GetActorLocation() + dir * dtdt;
	enemy->SetActorLocation(P, true);*/

}


// 일정시간에 한번씩
// 공격하고싶다.

void UEnemyFSM::AttackState()
{
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 2. 공격시간이 됐으니까,
	if (currentTime > attackDelayTime)
	{
	// 3, 콘솔에 공격 출력하고싶다
	PRINTLOG(TEXT("ATTACK"));
	currentTime = 0;
	}


	//// 거리가 필요하다
	//FVector direction = target->GetActorLocation() - enemy->GetActorLocation();
	//
	//// 거리는 디렉션의 사이즈
	//float distance = direction.Size();
	//direction.Normalize();

	// 같은 것
	float distance = FVector::Dist(target->GetActorLocation(), enemy->GetActorLocation());

	// 거리가 공격범위보다 크다면
	if (distance > attackRange)
	{
	// 상태를 무브로 전환해주고싶다
		m_state = EEnemyState::Move;
	}
}


// lerp를 이용해서 뒤로 밀리는 넉백 효과 만들어보기
void UEnemyFSM::DamageState()
{
	// LERP (From, to, 속도)
	FVector myPos = enemy->GetActorLocation();

	myPos = FMath::Lerp(myPos, knockbackPos, knockbackSpeed * GetWorld()->DeltaTimeSeconds);

	float distance = FVector::Dist(myPos, knockbackPos);

	if (distance < knockbackRange)
	{

	myPos = knockbackPos;
	m_state = EEnemyState::Idle;
    	//Idle 일때 경과시간이 0부터 되도록 처리하겠다.
   	 currentTime = 0;
		
	}

	enemy->SetActorLocation(myPos);



}

void UEnemyFSM::DieState()
{

}

void UEnemyFSM::OnDamageProcess(FVector shootDirection)
{

	ai->StopMovement();

	//상태를 Damage로 전환하고 싶다.
	
	// 피격 받았을 때 hp를 감소시키고 0 이하면 die로 바꾸고 없애버리자.
	// 1. HP가 감소했으니까
	hp --;

	// 2. 만약 hp가 0 이하면
	if (hp <= 0)
	{
		m_state = EEnemyState::Die;
		
		return;
	}

	// 넉백 (KnockBack) 처리
	// 밀릴 방향
	knockbackPos = enemy->GetActorLocation() + shootDirection * knockback;
	
	/*enemy->SetActorLocation(enemy->GetActorLocation() + shootDirection * knockback);
	*/
	m_state = EEnemyState::Damage;
	currentTime = 0;

	// 타이머 사용
	FTimerHandle damagTimer;
	GetWorld()->GetTimerManager().SetTimer(damagTimer, this, &UEnemyFSM::DamageState, damageDelayTime, false);
	}



