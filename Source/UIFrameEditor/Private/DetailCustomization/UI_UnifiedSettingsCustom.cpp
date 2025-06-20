#include "DetailCustomization/UI_UnifiedSettingsCustom.h"
#include <DetailLayoutBuilder.h>
#include <DetailWidgetRow.h>
#include <Widget/EditorTool/UI_UnifiedSettings.h>
#include <Misc/AssertionMacros.h>
#include "DetailCategoryBuilder.h"

#define LOCTEXT_NAMESPACE "UI_UnifiedSettingsCustom"

TSharedRef<IDetailCustomization> IUI_UnifiedSettingsCustom::MakeInstance()
{
	return MakeShareable(new IUI_UnifiedSettingsCustom);
}

void IUI_UnifiedSettingsCustom::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// 获取属性句柄
	TSharedPtr<IPropertyHandle> UI_UnifiedSettingsHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UUI_UnifiedSettings, UI_UnifiedSettings));

	UObject* OutValue;
	UI_UnifiedSettingsHandle->GetValue(OutValue);
	if (IsValid(OutValue))
	{
		UI_UnifiedSettings = Cast<UUI_UnifiedSettings>(OutValue);
	}

	check(UI_UnifiedSettingsHandle.IsValid());

	IDetailCategoryBuilder& GetAllWidgetBuilder = DetailBuilder.EditCategory("UI_UnifiedSettings");
	//搜索词条
	GetAllWidgetBuilder.AddCustomRow(FText::FromString("UI_UnifiedSettings"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("GetAllWidget", "获取全部该类型的控件"))
			
		]
		.ValueContent()
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Text(LOCTEXT("Get", "获取"))
			.OnClicked(FOnClicked::CreateRaw(this, &IUI_UnifiedSettingsCustom::Click_GetAllWidget_Button))//绑定按键事件
		];
}

FReply IUI_UnifiedSettingsCustom::Click_GetAllWidget_Button()
{
	if (IsValid(UI_UnifiedSettings))
	{
		UI_UnifiedSettings->AllWidget.Empty();
		UI_UnifiedSettings->GetAllWidgetFormClass(UI_UnifiedSettings->GetRootWidget(), UI_UnifiedSettings->WidgetClass);
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE