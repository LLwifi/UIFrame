// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/UI_WidgetBase.h"
#include <EditorConfig/UI_CommonGameConfig.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>
#include "Blueprint/WidgetTree.h"

#if WITH_EDITOR
const FText UUI_WidgetBase::GetPaletteCategory()
{
	return FText::FromString("UI_Panel");
}

void UUI_WidgetBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UUI_WidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}
#endif

void UUI_WidgetBase::RemoveFromParent()
{
	Super::RemoveFromParent();
}

void UUI_WidgetBase::BeginDestroy()
{
	Super::BeginDestroy();
	ParentUI = nullptr;
}

void UUI_WidgetBase::UIInit_Implementation()
{

}

FName UUI_WidgetBase::GetUITag_Implementation()
{
	return UITag;
}

UUserWidget* UUI_WidgetBase::GetParentUI_Implementation()
{
	return ParentUI;
}

void UUI_WidgetBase::SetParentUI_Implementation(UUserWidget* Parent)
{
	ParentUI = Parent;
}

void UUI_WidgetBase::Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type/* = EUIShowHideType::Normal*/)
{
	//没有被添加到屏幕上，并且自己没有依靠父类显示时，将当前UI显示到屏幕上
	if (!IsInViewport() && GetParent() == nullptr)
	{
		AddToViewport((int32)UILevel);
	}
	if (IsFocusable() && GetOwningPlayer())//SetFocus()内部会使用GetOwningPlayers()若为空编辑器会报错，这里判断有效性避免报错 //bIsFocusable//UE4
	{
		SetFocus();
	}
	UEnum* const CompileModeEnum = StaticEnum<EUIShowHideType>();
	if (CompileModeEnum)
	{
		UE_LOG(UIFrame, Log, TEXT("Event-Show(%s) UI-[%s]"),
			*CompileModeEnum->GetDisplayNameTextByValue(static_cast<uint8>(Type)).ToString(), *GetName());
	}

	ShowDelegate.Broadcast(this, Type);
	
	CurUIShowHideType = Type;
	if (!bIsShow && Type != EUIShowHideType::Esc_Top)//Esc_Top是一个纯时机的枚举，它表示进入/立刻EscList的顶层
	{
		bIsShow = true;
		IUI_PanelInteract::Execute_ShowUI(this,TriggerUI);
		UE_LOG(UIFrame, Log, TEXT("Display-ShowUI UI-[%s]"),*GetName());
		SwitchUIDelegate.Broadcast(this, true);
	}
}

void UUI_WidgetBase::Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type/* = EUIShowHideType::Normal*/)
{
	UEnum* const CompileModeEnum = StaticEnum<EUIShowHideType>();
	if (CompileModeEnum)
	{
		UE_LOG(UIFrame, Log, TEXT("Event-Hide(%s) UI-[%s]"),
			*CompileModeEnum->GetDisplayNameTextByValue(static_cast<uint8>(Type)).ToString(), *GetName());
	}

	HideDelegate.Broadcast(this, Type);
	
	CurUIShowHideType = Type;
	if (bIsShow && Type != EUIShowHideType::Esc_Top)//Esc_Top是一个纯时机的枚举，它表示进入/立刻EscList的顶层
	{
		bIsShow = false;
		IUI_PanelInteract::Execute_HideUI(this,TriggerUI);
		UE_LOG(UIFrame, Log, TEXT("Display-HideUI UI-[%s]"),*GetName());
		SwitchUIDelegate.Broadcast(this, false);
	}
}

void UUI_WidgetBase::ShowUI_Implementation(UWidget* TriggerUI)
{
}

void UUI_WidgetBase::HideUI_Implementation(UWidget* TriggerUI)
{
}

bool UUI_WidgetBase::IsFollowParentInteract_Implementation()
{
	return bFollowParentInteract;
}

bool UUI_WidgetBase::IsControlledByEsc_Implementation()
{
	return bIsControlledByEsc;
}

bool UUI_WidgetBase::IsClearHistoricalRecords_Implementation()
{
	return bIsClearHistoricalRecords;
}

bool UUI_WidgetBase::Esc_Implementation(UWidget* TriggerUI)
{
	IUI_PanelInteract::Execute_Hide(this, TriggerUI, EUIShowHideType::Esc);
	return true;
}

bool UUI_WidgetBase::IsSeparatelyShow_Implementation()
{
	return bIsSeparatelyShow;
}

EUIInputMode UUI_WidgetBase::GetUIInputMode_Implementation()
{
	return UIInputMode;
}

EUILevel UUI_WidgetBase::GetUILevel_Implementation()
{
	return UILevel;
}

FGameplayTag UUI_WidgetBase::GetUILayoutTag_Implementation()
{
	return LayoutTag;
}

bool UUI_WidgetBase::IsShow_Implementation()
{
	return bIsShow;
}

EUIShowHideType UUI_WidgetBase::GetCurUIShowHideType_Implementation()
{
	return  CurUIShowHideType;
}

TArray<EUIShowHideType> UUI_WidgetBase::GetFollowParentShowHideType_Implementation()
{
	return FollowParentShowHideType;
}

UWidget* UUI_WidgetBase::GetWidgetFormTag(FGameplayTag Tag)
{
	FName WidgetName = FName(Tag.ToString().Replace(TEXT(String_UI_Layout),TEXT("")).Replace(TEXT("."),TEXT("_")));
	return WidgetTree->FindWidget(WidgetName);
}


