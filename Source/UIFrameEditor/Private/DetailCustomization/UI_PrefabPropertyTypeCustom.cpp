#include "DetailCustomization/UI_PrefabPropertyTypeCustom.h"
#include <IDetailChildrenBuilder.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Misc/MessageDialog.h>
#include <Misc/PackageName.h>
#include <FileHelpers.h>
#include <Common/UI_PrefabInteract.h>
#include <DetailWidgetRow.h>

#define LOCTEXT_NAMESPACE "UI_PrefabPropertyTypeCustom"

TSharedRef<IPropertyTypeCustomization> IUI_PrefabPropertyTypeCustom::MakeInstance()
{
	return MakeShareable(new IUI_PrefabPropertyTypeCustom());
}

void IUI_PrefabPropertyTypeCustom::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//TSharedPtr<IPropertyHandle> NamePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FUI_PrefabOperation, Info));

	//check(NamePropertyHandle.IsValid());

	// 每一个默认Content都分为NameContent和ValueContent
	// NameContent和ValueContent之间由一道分隔线区分：
	// [ Struct Value Category      ]
	// [ NameContent | ValueContent ] ( HeaderRow )
	// [      ...    |      ...     ] ( ChildrenRow )
	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		];
}

void IUI_PrefabPropertyTypeCustom::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TSharedPtr<IPropertyHandle> DatatableHandle = PropertyHandle->GetParentHandle()->GetChildHandle("UseRow")->GetChildHandle("Datatable");
	TSharedPtr<IPropertyHandle> WidgetHandle = PropertyHandle->GetParentHandle()->GetChildHandle("Widget");

	check(DatatableHandle.IsValid() && WidgetHandle.IsValid());

	//临时使用Widget接取Datatable变量
	DatatableHandle->GetValue(Widget);
	if (Widget)
	{
		Datatable = Cast<UDataTable>(Widget);
	}

	//拿到Widget
	WidgetHandle->GetValue(Widget);

	void* ValuePtr;
	PropertyHandle->GetValueData(ValuePtr);
	if (ValuePtr != nullptr)
	{
		PrefabOperation = *(FUI_PrefabOperation*)ValuePtr;
	}

	ChildBuilder.AddCustomRow(FText())
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.VAlign(VAlign_Center)//垂直对齐防式
				.HAlign(HAlign_Center)//水平对齐方式
				.Text(LOCTEXT("CoverButton", "覆盖当前行"))
				.OnClicked(FOnClicked::CreateRaw(this, &IUI_PrefabPropertyTypeCustom::CoverButtonClick))//绑定按键事件
			]
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SAssignNew(NewRowNameEditableTextBox, SEditableTextBox)
					.HintText(LOCTEXT("NewRowName", "新增行名称"))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
					.VAlign(VAlign_Center)//垂直对齐防式
					.HAlign(HAlign_Center)//水平对齐方式
					.Text(LOCTEXT("NewButton", "新增预制行"))
					.OnClicked(FOnClicked::CreateRaw(this, &IUI_PrefabPropertyTypeCustom::NewButtonClick))//绑定按键事件
				]
			]
		];
}

bool IUI_PrefabPropertyTypeCustom::IsAutoSaveDataTable()
{
	UPackage* Package = FindPackage(nullptr, *FPackageName::FilenameToLongPackageName(Datatable->GetPathName()));
	if (Package)
	{
		Package->SetDirtyFlag(true);
		FEditorFileUtils::SaveDirtyPackages(false, false, true);
		return true;
	}
	return false;
}

FReply IUI_PrefabPropertyTypeCustom::CoverButtonClick()
{
	if (IsValid(Widget) && Widget->Implements<UUI_PrefabInteract>())
	{
		IUI_PrefabInteract* PrefabInst = Cast<IUI_PrefabInteract>(Widget);
		PrefabInst->OperationEvent_Cover();

		if (IsAutoSaveDataTable())
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("CoverBackSuccess1", "覆盖成功"));
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("CoverBackSuccess2", "覆盖成功，但仍需手动保存文件"));
		}
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("CoverBackFail", "覆盖失败：Widget未找到 或者 该控件没有实现预制相关接口（IUI_PrefabInteract）"));
	}
	
	return FReply::Handled();
}

FReply IUI_PrefabPropertyTypeCustom::NewButtonClick()
{
	FString S = NewRowNameEditableTextBox->GetText().ToString();
	if (S.Len() > 0)
	{
		if (IsValid(Widget) && Widget->Implements<UUI_PrefabInteract>())
		{
			IUI_PrefabInteract* PrefabInst = Cast<IUI_PrefabInteract>(Widget);
			PrefabInst->OperationEvent_New(FName(*S));

			if (IsAutoSaveDataTable())
			{
				FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NewBackSuccess1", "添加成功"));
			}
			else
			{
				FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NewBackSuccess2", "添加成功，但仍需手动保存文件"));
			}
			
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NewBackFail2", "添加失败：没有绑定事件"));
		}
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NewBackFail1", "添加失败：没有输入行名称"));
	}


	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE