// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Component/UI_ComponentBase.h"
#include "Widgets/Layout/SSpacer.h"

UUI_ComponentBase::UUI_ComponentBase()
{

}

TSharedRef<SWidget> UUI_ComponentBase::RebuildWidget()
{
	return SNew(SSpacer);
}

#if WITH_EDITOR
const FText UUI_ComponentBase::GetPaletteCategory()
{
	return FText::FromString("UI_Component");
}
#endif

void UUI_ComponentBase::Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type)
{
	CurUIShowHideType = Type;
}

void UUI_ComponentBase::Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type)
{
	CurUIShowHideType = Type;
}

UUserWidget* UUI_ComponentBase::GetParentUI_Implementation()
{
	return ParentUI;
}

void UUI_ComponentBase::SetParentUI_Implementation(UUserWidget* Parent)
{
	ParentUI = Parent;
}

bool UUI_ComponentBase::IsShow_Implementation()
{
	return true;
}

EUIShowHideType UUI_ComponentBase::GetCurUIShowHideType_Implementation()
{
	return CurUIShowHideType;
}

TArray<EUIShowHideType> UUI_ComponentBase::GetFollowParentShowHideType_Implementation()
{
	return FollowParentShowHideType;
}

bool UUI_ComponentBase::IsFollowParentInteract_Implementation()
{
	return bIsFollowParentInteract;
}


