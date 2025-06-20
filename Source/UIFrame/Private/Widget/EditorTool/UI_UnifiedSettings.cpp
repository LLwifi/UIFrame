// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/EditorTool/UI_UnifiedSettings.h"
#include "Widgets/Layout/SSpacer.h"
#include "Components/PanelWidget.h"

UUI_UnifiedSettings::UUI_UnifiedSettings()
{
	UI_UnifiedSettings = this;
}

TSharedRef<SWidget> UUI_UnifiedSettings::RebuildWidget()
{
	return SNew(SSpacer);
}

#if WITH_EDITOR
const FText UUI_UnifiedSettings::GetPaletteCategory()
{
	return FText::FromString("UI_WidgetTool");
}
#endif

UPanelWidget* UUI_UnifiedSettings::GetRootWidget()
{
	UPanelWidget* LastPanel = nullptr;
	UPanelWidget* NextPanel = GetParent();
	while (true)
	{
		LastPanel = NextPanel;
		NextPanel = NextPanel->GetParent();

		if (!IsValid(NextPanel))
		{
			break;
		}
	}

	return LastPanel;
}

void UUI_UnifiedSettings::GetAllWidgetFormClass(UPanelWidget* PanelWidget, TSubclassOf<UWidget> Class)
{
	if (!IsValid(PanelWidget) || !IsValid(Class))
	{
		return;
	}

	TArray<UWidget*> AllChildren = PanelWidget->GetAllChildren();
	for (int32 i = 0; i < AllChildren.Num(); i++)
	{
		if (AllChildren[i]->IsA(Class))
		{
			AllWidget.Add(AllChildren[i]);
		}
		else
		{
			UPanelWidget* Panel = Cast<UPanelWidget>(AllChildren[i]);
			if (IsValid(Panel))
			{
				GetAllWidgetFormClass(Panel, Class);
			}
		}
	}
}
