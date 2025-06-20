// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CanvasPanel/UI_CheckBoxCanvas.h"
#include "../../../Public/Common/UI_FunctionLibrary.h"
#include <Components/CheckBox.h>

#if WITH_EDITOR

void UUI_CheckBoxCanvas::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FProperty* Property = PropertyChangedEvent.Property;
	if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, AllCheckBox))
	{
		if (bIsMultiCheck)//多选
		{
			MultiUpdate();
		}
		else//单选
		{
			SingleUpdate();
		}
		IsNoneCheck();
	}
	else if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, bIsMultiCheck))
	{
		if (bIsMultiCheck)
		{
			MaxCheckNum = 2;
		}
		else
		{
			//多选回到单选
			int32 Index = 0;
			if (CurCheckIndex.IsValidIndex(0))
			{
				Index = CurCheckIndex[0];
			}
			int32 Count = 0;
			MaxCheckNum = 1;
			for (TPair<UCheckBox*, bool>& P : AllCheckBox)
			{
				ChangeCheckState(P, Count == Index);
				Count++;
			}
		}
	}
	else if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, MaxCheckNum))
	{
		if (MaxCheckNum < 1)
		{
			MaxCheckNum = 1;
		}
		else if (MaxCheckNum > AllCheckBox.Num())
		{
			MaxCheckNum = AllCheckBox.Num();
		}
		//当前初始打开的数量高于最大多选数量时，动态减少
		int32 CheckNum = 0;
		for (TPair<UCheckBox*, bool>& P : AllCheckBox)
		{
			if (P.Value)
			{
				CheckNum++;
				if (CheckNum > MaxCheckNum)
				{
					ChangeCheckState(P, false);
				}
			}
		}
	}
	else if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, bAutoFindCheckBox))
	{
		if (bAutoFindCheckBox)
		{
			Init();
		}
	}
	else if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, bIsNoneCheck))
	{
		if (bIsNoneCheck)
		{
			IsNoneCheck();
		}
	}
	CurCheckIndexUpdate();
}

const FText UUI_CheckBoxCanvas::GetPaletteCategory()
{
	return FText::FromString("UI_Widget");
}

#endif

void UUI_CheckBoxCanvas::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

TSharedRef<SWidget> UUI_CheckBoxCanvas::RebuildWidget()
{
	Init();
	return Super::RebuildWidget();
}

void UUI_CheckBoxCanvas::Init()
{
#if WITH_EDITOR
	if (bAutoFindCheckBox)
	{
		AllCheckBox.Empty();
		TArray<UWidget*> AllWidget = UUI_FunctionLibrary::GetAllWidgetFromClass(this, UCheckBox::StaticClass());
		for (int32 i = 0; i < AllWidget.Num(); i++)
		{
			UCheckBox* CheckBox = Cast<UCheckBox>(AllWidget[i]);
			if (IsValid(CheckBox))
			{
				AllCheckBox.Add(CheckBox, CheckBox->IsChecked());
			}
		}
		IsNoneCheck();
	}
# endif

#if UE_BUILD_SHIPPING
	for (TPair<UCheckBox*, bool>& P : AllCheckBox)
	{
		if (IsValid(P.Key))
		{
			FScriptDelegate ScriptDelegate;
			ScriptDelegate.BindUFunction(this, "CheckBoxChange");
			P.Key->OnCheckStateChanged.Add(ScriptDelegate);
		}
	}
#endif
	CurCheckIndexUpdate();
}

void UUI_CheckBoxCanvas::ChangeCheckState(TPair<UCheckBox*, bool>& OnceCheckBox, bool IsCheck)
{
	if (IsValid(OnceCheckBox.Key) && OnceCheckBox.Key->IsChecked() != IsCheck)
	{
		OnceCheckBox.Value = IsCheck;
		OnceCheckBox.Key->SetIsChecked(IsCheck);
	}
}

void UUI_CheckBoxCanvas::SingleUpdate()
{
	int32 Count = 0;
	if (CurCheckIndex.IsValidIndex(0))
	{
		//移除上次选择的
		for (TPair<UCheckBox*, bool>& P : AllCheckBox)
		{
			if (Count == CurCheckIndex[0])
			{
				ChangeCheckState(P, false);
				break;
			}
			Count++;
		}
		//寻找新的下标
		Count = 0;
		for (TPair<UCheckBox*, bool>& P : AllCheckBox)
		{
			if (P.Value)
			{
				ChangeCheckState(P, true);
				break;
			}
			Count++;
		}
	}
}

void UUI_CheckBoxCanvas::MultiUpdate()
{
	int32 Count = 0;
	for (TPair<UCheckBox*, bool>& P : AllCheckBox)
	{
		ChangeCheckState(P, P.Value);
		if (P.Value)
		{
			Count++;
		}
	}
	//是否超过了最大选择
	if (Count > MaxCheckNum)
	{
		Count = 0;
		for (TPair<UCheckBox*, bool>& P : AllCheckBox)
		{
			ChangeCheckState(P, CurCheckIndex.Contains(Count));
			Count++;
		}
	}
}

void UUI_CheckBoxCanvas::IsNoneCheck()
{
	if (!bIsNoneCheck)
	{
		//是否一个都没选
		int Count = 0;
		bool IsCheck = false;
		for (TPair<UCheckBox*, bool>& P : AllCheckBox)
		{
			if (P.Value)
			{
				IsCheck = true;
				break;
			}
		}
		if (!IsCheck)
		{
			int32 Index = 0;
			if (CurCheckIndex.IsValidIndex(0))
			{
				Index = CurCheckIndex[0];
			}

			for (TPair<UCheckBox*, bool>& P : AllCheckBox)
			{
				if (Count == Index)
				{
					ChangeCheckState(P, true);
					break;
				}
				else
				{
					ChangeCheckState(P, false);
				}
				Count++;
			}
		}
	}
}

void UUI_CheckBoxCanvas::CheckBoxChange(bool bIsChecked)
{
	TArray<UCheckBox*> CheckBoxs;
	AllCheckBox.GenerateKeyArray(CheckBoxs);

	if (bIsMultiCheck)
	{
		MultiUpdate();
	}
	else
	{
		SingleUpdate();
	}
	IsNoneCheck();
	CurCheckIndexUpdate();

	//触发事件
	CheckBoxsStateChanged.Broadcast(CurCheckIndex);
}

void UUI_CheckBoxCanvas::CurCheckIndexUpdate()
{
	CurCheckIndex.Empty();
	int32 Count = 0;
	for (TPair<UCheckBox*, bool>& P : AllCheckBox)
	{
		if (P.Value)
		{
			CurCheckIndex.Add(Count);
		}
		Count++;
	}
}

