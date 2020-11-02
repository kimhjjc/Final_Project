// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Final_Project : ModuleRules
{
	public Final_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // ���� ���� �߰��� �κ� : UMG(UI ���� ���), AIModule, NavigationSystem(�׺���̼� �ý��� ���), GameplayTasks(�����̺�� Ʈ�� �½�ũ ���)
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "GameplayTasks", "NavigationSystem" });

        // �����ΰ� ���ִ� Private ���������� �Ʒ� ����� ����ϱ� ������ PrivateDependencyModuleNames���� �߰����ش�.
        PrivateDependencyModuleNames.AddRange(new string[] { "Final_ProjectSetting" });


        // �Ʒ� �� ���� ����Ƽ ���带 ����ϴ� ������ ���� �ð��� ���� ���ҽ���������, �ڵ庯���� ���� ��⿡�� �������� �ʴ�. �ٽ� �ٲٷ��� �ּ�ó���� �ٲٸ� �ȴ�.
        //bFasterWithoutUnity = true;
        //MinFilesUsingPrecompiledHeader = 1;

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
