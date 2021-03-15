// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

/*
1. 애니메이션의 사용은 아래와 같이 코드로 애니메이션을 지정하는 방법이 있지만, 게임의 규모가 커지면 이런 방식은 관리적인 한계에 부딫힌다. 따라서 언리얼엔진은 체계적으로 애니메이션 시스템을 설계하기 위해 블루프린트를 사용한다.
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Animation/WarriorRun.WarriorRun"));
	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}
2. 애니메이션 블루프린트를 불러올 때는 경로에 "_C"를 붙여줘야한다. (클래스임을 알려주기 위해)
3. ACharater 모델과 APawn 모델의 차이점은 캐릭터 무브먼트 컴포넌트를 사용한다는 것이다.
	캐릭터 무브먼트 컴포넌트의 장점
	1) 점프같은 중력 반영 움직임 제공
	2) 다양한 움직임 설정 가능(걷기 외 기어가기, 날아가기, 수영 등등), 현재 움직임에 대한 더 많은 정보 전달
	3) 멀티 플레이 시 네트워크 환경에서 캐릭터들의 움직임을 자동으로 동기화
4. 플레이 중 '~'키를 누르면 콘솔 명령창이 나온다. 거기서 예를들어 DisplayAll <classname> 이런식으로 검색하면 플레이 화면에 해당 정보가 나온다. ※ 이때, 클래스 접두사는 제외하고 입력한다.
5. 마우스 움직임 중, Z축(Yaw)는 ACharactor에서 useControllerRotationYaw가 기본 사용으로 설정되어 있지만, 나머지 Pitch, Roll은 기본 사용 안함으로 설정되어 있다.
6. ACharacter 클래스에서는 미리 Jump라는 멤버 함수가 바인딩할수 있게 선언 되어있어 특별한 정의 없이 바로 바인딩만 하여 Jump함수를 사용이 가능하다. GetCharacterMovement()->JumpZVelocity의 값은 기본적으로 420으로 설정되어 있다.
7. UFPAnimInstance 클래스같이 자주 사용하는 클래스는 멤버변수로 선언하여 전방 선언으로 설계하는 것이 바람직하다.
*/

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class FINAL_PROJECT_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

public:
	// Sets default values for this character's properties
	AFPCharacter();
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	int32 GetExp() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetWeapon();
	void Setweapon(class AFPWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	class AFPWeapon* CurrentWeapon;

	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UFPCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	// Attack 함수는 원래 private에 선언하였으나, NPC의 공격을 구현하기 위해 public의 위치로 바꾸어 주었다.
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);

private:
	// Axis 매핑 활용해서 이동키 만들기
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	// Action 매핑
	void Skill_Rest();
	void Attack_RMB();
	
	void ActFinish();
	void ViewChange();

	// 애니메이션 몽타주에 델리게이트를 사용해보기
	UFUNCTION()
	void OnAttackMontageStarted(UAnimMontage* Montage);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
	void MovementAnimMotionState();

	void OnAssetLoadCompleted();

	bool IsActing();

	void FinishResting();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsRestEntered;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsResting;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsRMBComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	// AttackRange와 AttackRadius는 공격범위를 체크하기 위해 사용한다.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAnimMotionMoveing;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAnimFinish;
	// 자주 사용하는 클래스를 전방선언 시켜준다.
	UPROPERTY()
	class UFPAnimInstance* FPAnim;
	
	int32 AssetIndex = 0;

	// FSoftObjectPath 클래스는 애셋의 경로 정보를 보관하기 위해 사용한다.
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class AFPAIController* FPAIController;

	UPROPERTY()
	class AFPPlayerController* FPPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = { };
};
