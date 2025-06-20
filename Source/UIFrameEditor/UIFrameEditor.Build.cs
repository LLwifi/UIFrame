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
				"DetailCustomizations",//自定义细节面板
				"UnrealEd",//自动保存文件
				"UIFrame",
                "Projects",//自定义创建的资源外观图片
                "Kismet",
                "EditorStyle",//图表节点工厂
                "ToolMenus",//图表规则使用
                "GameplayTags",
                "GraphEditor",//图表节点工厂

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
