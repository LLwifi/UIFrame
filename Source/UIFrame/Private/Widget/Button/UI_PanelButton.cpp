// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Button/UI_PanelButton.h"
#include <UI_SubSystem.h>
#include "Subsystems/SubsystemBlueprintLibrary.h"

UUI_PanelButton::UUI_PanelButton()
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "ClickPanelButton");
	OnClicked.Add(Delegate);
}

void UUI_PanelButton::ClickPanelButton()
{
	for (TPair<TSubclassOf<UUserWidget>, bool> P : ClickChangeUI_Class)
	{
		if (P.Value)
		{
			GetUISubSystem()->ShowUIForClass(P.Key);
		}
		else
		{
			GetUISubSystem()->HideUIForClass(P.Key);
		}
	}

	for (TPair<UUserWidget*, bool> P : ClickChangeUI_Widget)
	{
		if (P.Value)
		{
			GetUISubSystem()->ShowUI(P.Key);
		}
		else
		{
			GetUISubSystem()->HideUI(P.Key);
		}
	}
}

UUI_SubSystem* UUI_PanelButton::GetUISubSystem()
{
	if (!IsValid(UISubSystem))
	{
		UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UUI_SubSystem::StaticClass()));
	}
	return UISubSystem;
}
