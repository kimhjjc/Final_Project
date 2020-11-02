// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Final_Project : ModuleRules
{
	public Final_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // 내가 직접 추가한 부분 : UMG(UI 관련 모듈), AIModule, NavigationSystem(네비게이션 시스템 모듈), GameplayTasks(비헤이비어 트리 태스크 모듈)
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "GameplayTasks", "NavigationSystem" });

        // 구현부가 모여있는 Private 폴더에서만 아래 모듈을 사용하기 때문에 PrivateDependencyModuleNames에서 추가해준다.
        PrivateDependencyModuleNames.AddRange(new string[] { "Final_ProjectSetting" });


        // 아래 두 줄은 유니티 빌드를 사용하는 것으로 빌드 시간을 대폭 감소시켜주지만, 코드변경이 잦은 모듈에는 적합하지 않다. 다시 바꾸려면 주석처리로 바꾸면 된다.
        //bFasterWithoutUnity = true;
        //MinFilesUsingPrecompiledHeader = 1;

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
