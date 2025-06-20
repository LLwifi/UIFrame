// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIFrame.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FUIFrameModule"

void FUIFrameModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	//// 加载 PropertyEditor 模块
	//FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	////将UI_WidgetBase 的细节面板和 IUI_DetailCustomization 关联起来
	//PropertyEditorModule.RegisterCustomClassLayout(FName("UI_WidgetBase"), FOnGetDetailCustomizationInstance::CreateStatic(&IUI_DetailCustomization::MakeInstance));
	//PropertyEditorModule.RegisterCustomClassLayout(FName("UI_UnifiedSettings"), FOnGetDetailCustomizationInstance::CreateStatic(&IUI_UnifiedSettingsCustom::MakeInstance));
	////PropertyEditorModule.RegisterCustomClassLayout(FName("UI_CheckBoxCanvas"), FOnGetDetailCustomizationInstance::CreateStatic(&IUI_CheckBoxCanvasCustom::MakeInstance));
	////将结构体FUI_PrefabOperation 和 IUI_PrefabPropertyTypeCustom 关联起来
	//PropertyEditorModule.RegisterCustomPropertyTypeLayout(FName("UI_PrefabOperation"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&IUI_PrefabPropertyTypeCustom::MakeInstance));

	//PropertyEditorModule.NotifyCustomizationModuleChanged();


}

void FUIFrameModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	//PropertyEditorModule.UnregisterCustomClassLayout(FName("UI_WidgetBase"));
	//PropertyEditorModule.UnregisterCustomClassLayout(FName("UI_UnifiedSettings"));

	//PropertyEditorModule.NotifyCustomizationModuleChanged();
	////-----------------------------------------------------------------------------------------------------------------

	//if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	//{
	//	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	//	PropertyModule.UnregisterCustomPropertyTypeLayout("UI_PrefabOperation");

	//	PropertyModule.NotifyCustomizationModuleChanged();
	//}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUIFrameModule, UIFrame)