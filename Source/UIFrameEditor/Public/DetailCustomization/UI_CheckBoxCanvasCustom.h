#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

//统一设置控件的扩展细节面板
class UIFRAMEEDITOR_API IUI_CheckBoxCanvasCustom : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
