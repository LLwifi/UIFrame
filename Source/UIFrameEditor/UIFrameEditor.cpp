// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIFrameEditor.h"


#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UI_SubSystem.h"
#include "DetailCustomization/UI_DetailCustomization.h"
#include "DetailCustomization/UI_PrefabPropertyTypeCustom.h"
#include "DetailCustomization/UI_UnifiedSettingsCustom.h"
#include "Public/RedPointEditor/RedPointFactory.h"
#include <Interfaces/IPluginManager.h>
#include <Styling/SlateStyleRegistry.h>
#include <EdGraphUtilities.h>
#include "Public/RedPointEditor/RedPointTreeNodeFactory.h"


#define LOCTEXT_NAMESPACE "FUIFrameEditorModule"

void FUIFrameEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// 加载 PropertyEditor 模块
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	//将UI_WidgetBase 的细节面板和 IUI_DetailCustomization 关联起来
	PropertyEditorModule.RegisterCustomClassLayout(FName("UI_WidgetBase"), FOnGetDetailCustomizationInstance::CreateStatic(&IUI_DetailCustomization::MakeInstance));
	PropertyEditorModule.RegisterCustomClassLayout(FName("UI_UnifiedSettings"), FOnGetDetailCustomizationInstance::CreateStatic(&IUI_UnifiedSettingsCustom::MakeInstance));
	//PropertyEditorModule.RegisterCustomClassLayout(FName("UI_CheckBoxCanvas"), FOnGetDetailCustomizationInstance::CreateStatic(&IUI_CheckBoxCanvasCustom::MakeInstance));
	//将结构体FUI_PrefabOperation 和 IUI_PrefabPropertyTypeCustom 关联起来
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FName("UI_PrefabOperation"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&IUI_PrefabPropertyTypeCustom::MakeInstance));

	PropertyEditorModule.NotifyCustomizationModuleChanged();

	//自定义资源注册
	IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	CreateRedPointTreeAction = MakeShareable(new FAssetTypeAction_RedPointTree);
	AssetToolsModule.RegisterAssetTypeActions(CreateRedPointTreeAction.ToSharedRef());
	//此代码将在您的模块加载到内存后执行；确切的时间在.upl中指定

	StyleSet = MakeShareable(new FSlateStyleSet("OrfeasStyle"));
	//此插件的内容路径
	FString ContentDir = IPluginManager::Get().FindPlugin("UIFrame")->GetBaseDir();
	//我们要加载的映像位于基本目录中的Resources文件夹中
	//因此，让我们将content-dir设置为基本目录，然后手动切换到Resources文件夹：
	StyleSet->SetContentRoot(ContentDir);
	//从图标创建笔刷
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/RedPointAsset128"), TEXT(".png")), FVector2D(128.f, 128.f));
	if (ThumbnailBrush)
	{
		//为了将缩略图绑定到我们的类，我们需要键入ClassThumbnam.X，其中X是资产的C++类的名称
		StyleSet->Set("ClassThumbnail.RedPointTree", ThumbnailBrush);
		//Reguster the created style
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}

	//注册图表节点工厂
	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShareable(new FRedPointTreeNodeFactory()));
}

void FUIFrameEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout(FName("UI_WidgetBase"));
	PropertyEditorModule.UnregisterCustomClassLayout(FName("UI_UnifiedSettings"));

	PropertyEditorModule.NotifyCustomizationModuleChanged();
	//-----------------------------------------------------------------------------------------------------------------

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout("UI_PrefabOperation");

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
	if (AssetToolsModule)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();
		AssetTools.UnregisterAssetTypeActions(CreateRedPointTreeAction.ToSharedRef());
	}
	//取消注册样式
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUIFrameEditorModule, UIFrameEditor)