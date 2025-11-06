// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Button/UI_Button.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Components/CanvasPanelSlot.h>
#include <EditorConfig/UI_CommonGameConfig.h>

UUI_Button::UUI_Button()
{
#if WITH_EDITOR
	Prefab.UseRow.DataTable = UUI_CommonGameConfig::GetInstance()->PrefabButtonDatatable.Get();
	if (!IsValid(Prefab.UseRow.DataTable))
	{
		Prefab.UseRow.DataTable = UUI_CommonGameConfig::GetInstance()->PrefabButtonDatatable.LoadSynchronous();
	}

	Prefab.Widget = this;
#endif
}

#if WITH_EDITOR
void UUI_Button::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	auto Property = PropertyChangedEvent.Property;

	if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_Button, Prefab) && Prefab.UseRow.DataTable)
	{
		FPrefabButtonInfo* ButtonInfo = Prefab.UseRow.DataTable->FindRow<FPrefabButtonInfo>(Prefab.UseRow.RowName, "ContextString");
		if (ButtonInfo != nullptr)
		{
			SetButtonInfo(*ButtonInfo);
		}
	}
}

const FText UUI_Button::GetPaletteCategory()
{
	return FText::FromString("UI_Widget");
}

void UUI_Button::OperationEvent_Cover()
{
	//UDataTable* DT =  const_cast<UDataTable*>(Prefab.UseRow.DataTable);//UE4
	UDataTable* DT = const_cast<UDataTable*>(Prefab.UseRow.DataTable.Get());
	if (DT)
	{
		DT->AddRow(Prefab.UseRow.RowName, GetButtonInfo());
	}
	
}

void UUI_Button::OperationEvent_New(FName RowName)
{
	//UDataTable* DT =  const_cast<UDataTable*>(Prefab.UseRow.DataTable);//UE4
	UDataTable* DT = const_cast<UDataTable*>(Prefab.UseRow.DataTable.Get());
	if (DT)
	{
		DT->AddRow(RowName, GetButtonInfo());
		Prefab.UseRow.RowName = RowName;
	}
}

#endif

void UUI_Button::SetButtonInfo(FPrefabButtonInfo ButtonInfo)
{
	SetStyle(ButtonInfo.WidgetStyle);
	SetClickMethod(ButtonInfo.ClickMethod);
	SetTouchMethod(ButtonInfo.TouchMethod);
	SetPressMethod(ButtonInfo.PressMethod);
}

FPrefabButtonInfo UUI_Button::GetButtonInfo()
{
	FPrefabButtonInfo MyButtonInfo;
	//UE4
	//MyButtonInfo.WidgetStyle = WidgetStyle;
	//MyButtonInfo.ClickMethod = ClickMethod;
	//MyButtonInfo.TouchMethod = TouchMethod;
	//MyButtonInfo.PressMethod = PressMethod;
	//UE5
	MyButtonInfo.WidgetStyle = GetStyle();
	MyButtonInfo.ClickMethod = GetClickMethod();
	MyButtonInfo.TouchMethod = GetTouchMethod();
	MyButtonInfo.PressMethod = GetPressMethod();

	return MyButtonInfo;
}

void UUI_Button::SetButtonImage(UObject* Image, TEnumAsByte<enum ESlateBrushDrawType::Type> DrawType, FMargin Box_Border_Margin, TEnumAsByte<enum ESlateBrushTileType::Type> Image_Tiling, FSlateBrushOutlineSettings RoundedBox_OutlineSettings)
{
	WidgetStyle.Normal.SetResourceObject(Image);
	WidgetStyle.Normal.DrawAs = DrawType;
	WidgetStyle.Normal.Margin = Box_Border_Margin;
	WidgetStyle.Normal.Tiling = Image_Tiling;
	WidgetStyle.Normal.OutlineSettings = RoundedBox_OutlineSettings;

	WidgetStyle.Hovered.SetResourceObject(Image);
	WidgetStyle.Hovered.DrawAs = DrawType;
	WidgetStyle.Hovered.Margin = Box_Border_Margin;
	WidgetStyle.Hovered.Tiling = Image_Tiling;
	WidgetStyle.Hovered.OutlineSettings = RoundedBox_OutlineSettings;

	WidgetStyle.Pressed.SetResourceObject(Image);
	WidgetStyle.Pressed.DrawAs = DrawType;
	WidgetStyle.Pressed.Margin = Box_Border_Margin;
	WidgetStyle.Pressed.Tiling = Image_Tiling;
	WidgetStyle.Pressed.OutlineSettings = RoundedBox_OutlineSettings;

	WidgetStyle.Disabled.SetResourceObject(Image);
	WidgetStyle.Disabled.DrawAs = DrawType;
	WidgetStyle.Disabled.Margin = Box_Border_Margin;
	WidgetStyle.Disabled.Tiling = Image_Tiling;
	WidgetStyle.Disabled.OutlineSettings = RoundedBox_OutlineSettings;

	SetStyle(WidgetStyle);
}
