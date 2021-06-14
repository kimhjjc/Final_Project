// Fill out your copyright notice in the Description page of Project Settings.


#include "FPQuestProgressBox.h"
#include "Characters/Player/FPCharacter.h"
#include "Characters/Player/FPPlayerController.h"
#include "FPPlayerState.h"
#include "UI/FPHUDWidget.h"
#include "UI/FPNPCWidget.h"
#include "UI/FPQuestWidget.h"
#include "UI/FPConversationWidget.h"

// Sets default values
AFPQuestProgressBox::AFPQuestProgressBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	RootComponent = TriggerBox;

	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HOWTOMOVE_C(TEXT("/Game/UI/TutorialUI/UI_HowToMove.UI_HowToMove_C"));
	if (UI_HOWTOMOVE_C.Succeeded())
	{
		HowToMoveWindowWidgetClass = UI_HOWTOMOVE_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_NPCTUTORIAL_C(TEXT("/Game/UI/TutorialUI/UI_TutorialNPC.UI_TutorialNPC_C"));
	if (UI_NPCTUTORIAL_C.Succeeded())
	{
		TutorialWindowWidgetClass = UI_NPCTUTORIAL_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HOWTOATTACK_C(TEXT("/Game/UI/TutorialUI/UI_HowToAttack.UI_HowToAttack_C"));
	if (UI_HOWTOATTACK_C.Succeeded())
	{
		HowToAttackWindowWidgetClass = UI_HOWTOATTACK_C.Class;
	}

	QuestNumber = 0;

}

void AFPQuestProgressBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// BoxComponent에는  OnComponentBeginOverlap이라는 멀티캐스팅 델리게이트가 있다. 이를 이용하여 박스컴포넌트에 겹침 콜리전 반응 진입 시 델리게이트 발생.
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFPQuestProgressBox::OnCharacterOverlap);
}

// Called when the game starts or when spawned
void AFPQuestProgressBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPQuestProgressBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPQuestProgressBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto FPCharacter = Cast<AFPCharacter>(OtherActor);
	FPCHECK(nullptr != FPCharacter);
	auto PlayerController = FPCharacter->GetFPPlayerController();
	FPCHECK(nullptr != PlayerController);

	QuestUpdate(FPCharacter);

}

void AFPQuestProgressBox::QuestUpdate(class AFPCharacter* FPCharacter, bool bIsGameLoad)
{
	auto PlayerController = FPCharacter->GetFPPlayerController();
	FPCHECK(nullptr != PlayerController);

	if (nullptr != FPCharacter)
	{
		FPLOG(Warning, TEXT("Quest Progress!"));

		//if (QuestNumber == PlayerController->GetFPPlayerState()->GetQuestNumber())
		{
			TArray<FString> ContentInfos;

			if (QuestNumber == 0)
			{
				QuestNumber++;

				ContentInfos.Add("I'm finally ready for the trip.");
				ContentInfos.Add("What's waiting for me?");
				ContentInfos.Add("If I make a lot of money, I'll do whatever I want.");
				ContentInfos.Add("To do that, I need to be strong and move hard.");
				ContentInfos.Add("Now Going on the adventure?");
				ContentInfos.Add("hmm, Maybe moving was done with WASD and mouse move.");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					FPPlayerCharacter->SetandOnTutorialUI(HowToMoveWindowWidgetClass);
					CloseContentWidget();
					});

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					QuestInfo = "Let's go out the door.\n";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					});
				PlayerController->OnQuestUpdate.Broadcast();
				PlayerController->OnQuestUpdate.Clear();

			}
			else if (QuestNumber == 1)
			{
				QuestNumber++;

				ContentInfos.Add("Oh, I see a man over there.");
				ContentInfos.Add("I'm gonna go talk to him.");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {

					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					FPPlayerCharacter->SetandOnTutorialUI(TutorialWindowWidgetClass);
					CloseContentWidget();
					});

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					QuestInfo = "Let's go talk to him.\n";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					});
				PlayerController->OnQuestUpdate.Broadcast();
				PlayerController->OnQuestUpdate.Clear();

			}

			FPCharacter->NPCConversation("Destiny", ContentInfos);
		}

		PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Visible);

	}

}

void AFPQuestProgressBox::CloseContentWidget()
{
		auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);

}