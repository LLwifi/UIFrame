// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Text/UI_Text.h"
#include <EditorConfig/UI_CommonGameConfig.h>

UUI_Text::UUI_Text()
{
#if WITH_EDITOR
	Prefab.UseRow.DataTable = UUI_CommonGameConfig::GetInstance()->PrefabTextDatatable.Get();
	if (!IsValid(Prefab.UseRow.DataTable))
	{
		Prefab.UseRow.DataTable = UUI_CommonGameConfig::GetInstance()->PrefabTextDatatable.LoadSynchronous();
	}

	Prefab.Widget = this;
#endif
}

#if WITH_EDITOR
void UUI_Text::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	auto Property = PropertyChangedEvent.Property;

	if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_Text, Prefab) && Prefab.UseRow.DataTable)
	{
		FPrefabTextInfo* TextInfo = Prefab.UseRow.DataTable->FindRow<FPrefabTextInfo>(Prefab.UseRow.RowName, "ContextString");
		if (TextInfo != nullptr)
		{
			SetTextInfo(*TextInfo);
		}
	}
}

const FText UUI_Text::GetPaletteCategory()
{
	return FText::FromString("UI_Widget");
}

void UUI_Text::OperationEvent_Cover()
{
	//UDataTable* DT = const_cast<UDataTable*>(Prefab.UseRow.DataTable);//UE4
	UDataTable* DT = const_cast<UDataTable*>(Prefab.UseRow.DataTable.Get());
	if (DT)
	{
		DT->AddRow(Prefab.UseRow.RowName, GetTextInfo());
	}
}

void UUI_Text::OperationEvent_New(FName Name)
{
	//UDataTable* DT = const_cast<UDataTable*>(Prefab.UseRow.DataTable);//UE4
	UDataTable* DT = const_cast<UDataTable*>(Prefab.UseRow.DataTable.Get());
	if (DT)
	{
		DT->AddRow(Name, GetTextInfo());
	}
}

#endif

void UUI_Text::SetTextInfo(FPrefabTextInfo TextInfo)
{
	SetColorAndOpacity(TextInfo.ColorAndOpacity);
	SetFont(TextInfo.Font);
	SetStrikeBrush(TextInfo.StrikeBrush);
	SetShadowOffset(TextInfo.ShadowOffset);
	SetShadowColorAndOpacity(TextInfo.ShadowColorAndOpacity);
	Margin = TextInfo.Margin;
	LineHeightPercentage = TextInfo.LineHeightPercentage;
}

FPrefabTextInfo UUI_Text::GetTextInfo()
{
	FPrefabTextInfo MyTextInfo;
	//UE4
	//MyTextInfo.ColorAndOpacity = ColorAndOpacity;
	//MyTextInfo.Font = Font;
	//MyTextInfo.StrikeBrush = StrikeBrush;
	//MyTextInfo.ShadowOffset = ShadowOffset;
	//MyTextInfo.ShadowColorAndOpacity = ShadowColorAndOpacity;
	//UE5
	MyTextInfo.ColorAndOpacity = GetColorAndOpacity();
	MyTextInfo.Font = GetFont();
	MyTextInfo.StrikeBrush = GetStrikeBrush();
	MyTextInfo.ShadowOffset = GetShadowOffset();
	MyTextInfo.ShadowColorAndOpacity = GetShadowColorAndOpacity();

	MyTextInfo.Margin = Margin;
	MyTextInfo.LineHeightPercentage = LineHeightPercentage;

	return MyTextInfo;
}

void UUI_Text::SetWrapTextAt(float TextWrapTextAt)
{
	WrapTextAt = TextWrapTextAt;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetWrapTextAt(TextWrapTextAt);
	}
}