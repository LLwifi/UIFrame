#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include <Engine/DataTable.h>
#include "Common/UI_StructAndEnum.h"

//FPrefab结构体的自定义细节面板
class UIFRAMEEDITOR_API IUI_PrefabPropertyTypeCustom : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(
	TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	bool IsAutoSaveDataTable();

	FReply CoverButtonClick();

	FReply NewButtonClick();

public:
	TSharedPtr<class SEditableTextBox> NewRowNameEditableTextBox;

	FUI_PrefabOperation PrefabOperation;

	UDataTable* Datatable;

	UObject* Widget;
};
