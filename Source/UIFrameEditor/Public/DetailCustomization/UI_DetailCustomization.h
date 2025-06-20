#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

//UI的扩展细节面板
class UIFRAMEEDITOR_API IUI_DetailCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
