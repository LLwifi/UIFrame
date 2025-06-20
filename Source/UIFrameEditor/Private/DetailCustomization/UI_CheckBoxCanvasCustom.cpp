#include "DetailCustomization/UI_CheckBoxCanvasCustom.h"
#include <DetailLayoutBuilder.h>

#define LOCTEXT_NAMESPACE "UI_PrefabPropertyTypeCustom"

TSharedRef<IDetailCustomization> IUI_CheckBoxCanvasCustom::MakeInstance()
{
	return MakeShareable(new IUI_CheckBoxCanvasCustom);
}


void IUI_CheckBoxCanvasCustom::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// 获取属性句柄
	TSharedPtr<IPropertyHandle> bIsMultiCheckHandle = DetailBuilder.GetProperty("bIsMultiCheck");
	TSharedPtr<IPropertyHandle> MaxCheckNumHandle = DetailBuilder.GetProperty("MaxCheckNum");

	bool bIsMultiCheck = false;
	bIsMultiCheckHandle->GetValue(bIsMultiCheck);

	if (!bIsMultiCheck)
	{
		DetailBuilder.HideProperty(MaxCheckNumHandle);
	}
}

#undef LOCTEXT_NAMESPACE


