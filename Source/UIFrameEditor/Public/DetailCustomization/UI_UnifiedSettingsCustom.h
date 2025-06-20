#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class UUI_UnifiedSettings;

//统一设置控件的扩展细节面板
class UIFRAMEEDITOR_API IUI_UnifiedSettingsCustom : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:
	FReply Click_GetAllWidget_Button();

public:
	UUI_UnifiedSettings* UI_UnifiedSettings;
};
