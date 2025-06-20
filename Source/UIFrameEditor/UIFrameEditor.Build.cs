// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UIFrameEditor : ModuleRules
{
	public UIFrameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"DeveloperSettings",
				"DetailCustomizations",//�Զ���ϸ�����
				"UnrealEd",//�Զ������ļ�
				"UIFrame",
                "Projects",//�Զ��崴������Դ���ͼƬ
                "Kismet",
                "EditorStyle",//ͼ��ڵ㹤��
                "ToolMenus",//ͼ�����ʹ��
                "GameplayTags",
                "GraphEditor",//ͼ��ڵ㹤��

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
