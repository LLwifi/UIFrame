// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Button/UI_EscButton.h"
#include <Subsystems/SubsystemBlueprintLibrary.h>
#include "../../../Public/UI_SubSystem.h"

UUI_EscButton::UUI_EscButton()
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "ClickEscButton");
	OnClicked.Add(Delegate);
}

void UUI_EscButton::Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type)
{
	CurUIShowHideType = Type;
}

void UUI_EscButton::Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type)
{
	CurUIShowHideType = Type;
}

bool UUI_EscButton::IsShow_Implementation()
{
	return true;
}

EUIShowHideType UUI_EscButton::GetCurUIShowHideType_Implementation()
{
	return CurUIShowHideType;
}

TArray<EUIShowHideType> UUI_EscButton::GetFollowParentShowHideType_Implementation()
{
	return FollowParentShowHideType;
}

void UUI_EscButton::ClickEscButton()
{
	if (IsValid(GetUISubSystem()))
	{
		GetUISubSystem()->Esc();
	}
}

UUI_SubSystem* UUI_EscButton::GetUISubSystem()
{
	if (!IsValid(UISubSystem))
	{
		UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UUI_SubSystem::StaticClass()));
	}
	return UISubSystem;
}
