// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/FPTutorialNPC.h"
#include "Components/WidgetComponent.h"
#include "UI/FPNPCWidget.h"
#include "UI/FPQuestWidget.h"
#include "UI/FPConversationWidget.h"
#include "Characters/Player/FPCharacter.h"
#include "Characters/Player/FPPlayerController.h"
#include "Characters/NPC/FPTutorialNPC_NextQuestPosition.h"

// Sets default values
AFPTutorialNPC::AFPTutorialNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	NPCNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCNAMEWIDGET"));


	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	Trigger->SetupAttachment(GetMesh());
	NPCNameWidget->SetupAttachment(GetMesh());


	Trigger->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	NPCNameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	NPCNameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));

	NPCName = "Tutarial NPC";
	QuestNumber = 0;
	MonsterKill = 0;
	TargetKill = 0;
	IsQuestAccept = false;
	IsInteractive = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_NPCNAME(TEXT("/Game/UI/UI_NPCName.UI_NPCName_C"));	// 블루 프린터로 제작하였으므로 경로 뒤에 _C를 붙여주어야 한다.
	if (UI_NPCNAME.Succeeded())
	{
		NPCNameWidget->SetWidgetClass(UI_NPCNAME.Class);
		NPCNameWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
}

// Called when the game starts or when spawned
void AFPTutorialNPC::BeginPlay()
{
	Super::BeginPlay();

	auto NPCWidget = Cast<UFPNPCWidget>(NPCNameWidget->GetUserWidgetObject());
	FPCHECK(nullptr != NPCWidget);
	NPCWidget->BindNPCName(NPCName);

	QuestInfo = "";
	ContentInfo = "";

}

void AFPTutorialNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// BoxComponent에는  OnComponentBeginOverlap이라는 멀티캐스팅 델리게이트가 있다. 이를 이용하여 박스컴포넌트에 겹침 콜리전 반응 진입 시 델리게이트 발생.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFPTutorialNPC::OnCharacterOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AFPTutorialNPC::OnOverlapEnd);
}

// Called every frame
void AFPTutorialNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPTutorialNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPTutorialNPC::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	auto FPCharacter = Cast<AFPCharacter>(OtherActor);
	FPCHECK(nullptr != FPCharacter);

	auto PlayerController = FPCharacter->GetFPPlayerController();
	FPCHECK(nullptr != PlayerController);

	if (nullptr != FPCharacter)
	{
		FPLOG(Warning, TEXT("NPC Interactive!"));

		IsInteractive = true;
		//FPCharacter->SetNPCInteractive(true);

		if (IsQuestAccept == true && MonsterKill == TargetKill)
		{
			static TArray<AActor*> NextPlace;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPTutorialNPC_NextQuestPosition::StaticClass(), NextPlace);
			if (QuestNumber == 0)
			{
				PlayerController->NPCKill(100);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				IsQuestAccept = false;
				QuestInfo = "go to the gate.";
				ContentInfo = "I'll wait in front of the gate.";

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);

				for (auto Place : NextPlace)
				{
					auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
					FPCHECK(nullptr != TempPlace);
					if (TempPlace->GetQuestNumber() == QuestNumber)
					{
						SetActorRelativeLocation(TempPlace->GetActorLocation());
						SetActorRelativeRotation(TempPlace->GetActorRotation());
					}

				}

				CloseContentWidget();
			}
			else if (QuestNumber == 1)
			{

			}
		}
		else if (IsQuestAccept == false)
		{
			if (QuestNumber == 0)
			{
				MonsterKill = -1;
				TargetKill = 3;
				// 어떤 델리게이트를 받아올까?
				// 킬을 했을 때.

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					MonsterKill = FMath::Clamp<int32>(MonsterKill + 1, 0, TargetKill);
					QuestInfo = "Kill the monsters you see when you leave the gate.\n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / 3)";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					IsQuestAccept = true;
					});

				ContentInfo = "The weather is good too.The Goblin has been hunting?";
				PlayerController->OnQuestUpdate.Broadcast();
			}
			else if (QuestNumber == 1)
			{
				QuestInfo = "Next Quest";
				ContentInfo = "Next Quest Content";
				IsQuestAccept = true;
			}
			else
			{
				ContentInfo = "There are more monsters these days.";
			}
			FPLOG(Warning, TEXT("QuestAccept false"));
		}
		else if (IsQuestAccept == true && MonsterKill != TargetKill)
		{
			ContentInfo = "You haven't hunted them yet?";
		}

		PlayerController->GetConversationWidget()->BindNPCContent(NPCName, ContentInfo);

		PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Visible);

	}

}

void AFPTutorialNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsInteractive = false;

	CloseContentWidget();
}


void AFPTutorialNPC::CloseContentWidget()
{
	GetWorld()->GetTimerManager().SetTimer(CloseTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		if (IsInteractive) return;

		auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);
		}), 5, false);
}