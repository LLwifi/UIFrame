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
		//将Map的状态刷新给CheckBox
		for (TPair<UCheckBox*, bool>& CheckBox : AllCheckBox)
		{
			ChangeCheckState(CheckBox, CheckBox.Value);
		}
		CheckBoxRangeCheck(CurSequenceCheckBox.Num());
	}
	else if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, MaxCheckNum))
	{
		SetMaxCheckNum(MaxCheckNum);
	}
	else if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_CheckBoxCanvas, bIsNoneCheck))
	{
		SetIsNoneCheck(bIsNoneCheck);
	}
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
	AutoFindCheckBox();
	return Super::RebuildWidget();
}

void UUI_CheckBoxCanvas::Init()
{
	for (TPair<UCheckBox*, bool>& P : AllCheckBox)
	{
		if (IsValid(P.Key))
		{
			FScriptDelegate ScriptDelegate;
			ScriptDelegate.BindUFunction(this, "CheckBoxChange");
			P.Key->OnCheckStateChanged.Add(ScriptDelegate);
		}
	}
}

void UUI_CheckBoxCanvas::AutoFindCheckBox()
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
}

void UUI_CheckBoxCanvas::ChangeCheckState(TPair<UCheckBox*, bool>& OnceCheckBox, bool IsCheck)
{
	if (IsValid(OnceCheckBox.Key) && OnceCheckBox.Key->IsChecked() != IsCheck)//UI有效，且值不同
	{
		OnceCheckBox.Value = IsCheck;
		OnceCheckBox.Key->SetIsChecked(IsCheck);
		if (IsCheck)
		{
			CurSequenceCheckBox.Add(OnceCheckBox.Key);
		}
		else
		{
			CurSequenceCheckBox.Remove(OnceCheckBox.Key);
		}
	}
}

bool UUI_CheckBoxCanvas::CheckBoxRangeCheck(int32 CurCheckNum)
{
	//判断最大数量
	TArray<UCheckBox*> CheckBoxs;
	AllCheckBox.GenerateKeyArray(CheckBoxs);
	if (CurCheckNum > MaxCheckNum)
	{
		//超过了就进行还原
		TArray<UCheckBox*> SequenceCheckBox = CurSequenceCheckBox;//For循环的过程中需要移除，所以这里用另外一个进行循环
		for (int32 i = SequenceCheckBox.Num() - 1; i >= 0; i--)
		{
			if (SequenceCheckBox.Num() - i <= MaxCheckNum)
			{
				ChangeCheckState(AllCheckBox.Get(FSetElementId::FromInteger(CheckBoxs.Find(SequenceCheckBox[i]))), true);
			}
			else
			{
				ChangeCheckState(AllCheckBox.Get(FSetElementId::FromInteger(CheckBoxs.Find(SequenceCheckBox[i]))), false);
			}
		}
		return false;
	}
	else if (!bIsNoneCheck && CurCheckNum < 1)//判断最小数量 不允许为空时 && 为空了
	{
		//进行还原，这种情况通常是由 1 ——》 0
		ChangeCheckState(AllCheckBox.Get(FSetElementId::FromInteger(CheckBoxs.Find(CurSequenceCheckBox[0]))), true);
		return false;
	}
	return true;
}

void UUI_CheckBoxCanvas::SomeCheckBoxChange(bool bIsChecked)
{
	TArray<int32> NewCheckIndex;
	int32 Index = 0;
	//将CheckBox的状态刷新给Map 
	for (TPair<UCheckBox*, bool>& CheckBox : AllCheckBox)
	{
		ChangeCheckState(CheckBox, CheckBox.Key->IsChecked());
		if (CheckBox.Key->IsChecked())
		{
			NewCheckIndex.Add(Index);
		}
		Index++;
	}
	if (CheckBoxRangeCheck(NewCheckIndex.Num()))
	{
		//修改通过，刷新下标记录
		CurCheckIndex = NewCheckIndex;
	}
	//触发事件
	CheckBoxsStateChanged.Broadcast(CurCheckIndex);
}

bool UUI_CheckBoxCanvas::IsMultiCheck()
{
	return MaxCheckNum > 1;
}

bool UUI_CheckBoxCanvas::SetIsNoneCheck(bool IsNoneCheck)
{
	bIsNoneCheck = IsNoneCheck;
	if (!bIsNoneCheck && CurCheckIndex.Num() < 1)//只有当突然不允许为空时，才会需要去进行自动指定
	{
		ChangeCheckState(AllCheckBox.Get(FSetElementId::FromInteger(0)), true);//将首个勾选
	}
	return bIsNoneCheck;
}

void UUI_CheckBoxCanvas::SetMaxCheckNum(int32 Num)
{
	//夹值
	if (Num < 1)
	{
		Num = 1;
	}
	else if (Num > AllCheckBox.Num())
	{
		Num = AllCheckBox.Num();
	}
	MaxCheckNum = Num;
	CheckBoxRangeCheck(CurSequenceCheckBox.Num());
}

