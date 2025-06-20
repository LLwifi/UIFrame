// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Component/UI_ShowAndHide.h"
#include "UI_SubSystem.h"
#include "EditorConfig/UI_CommonGameConfig.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "Widgets/Layout/SSpacer.h"

UUI_ShowAndHide::UUI_ShowAndHide()
{

}

TSharedRef<SWidget> UUI_ShowAndHide::RebuildWidget()
{
	return SNew(SSpacer);
}

#if WITH_EDITOR
void UUI_ShowAndHide::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UUI_ShowAndHide, PlaySoundConfig))
	{
		for (TPair<EUIInterface, USoundBase*> P : PlaySoundConfig)
		{
			GetSound(P.Key);
		}
	}
}
#endif

void UUI_ShowAndHide::Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type/* = EUIShowHideType::Normal*/)
{
	if (PlaySoundConfig.Contains(EUIInterface::Show))
	{
		UGameplayStatics::PlaySound2D(this, GetSound(EUIInterface::Show));
	}
	CallUIInterfaceOfClass(ShowUIClass);
	CallUIInterfaceOfUserWidget(ShowUIWidget);
	SetParentUIVisibility(EUIInterface::Show);
}

void UUI_ShowAndHide::Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type/* = EUIShowHideType::Normal*/)
{
	if (PlaySoundConfig.Contains(EUIInterface::Hide))
	{
		UGameplayStatics::PlaySound2D(this, GetSound(EUIInterface::Hide));
	}
	CallUIInterfaceOfClass(HideUIClass);
	CallUIInterfaceOfUserWidget(HideUIWidget);
	SetParentUIVisibility(EUIInterface::Hide);
}

bool UUI_ShowAndHide::Esc_Implementation(UWidget* TriggerUI)
{
	if (PlaySoundConfig.Contains(EUIInterface::Esc))
	{
		UGameplayStatics::PlaySound2D(this, GetSound(EUIInterface::Esc));
	}
	SetParentUIVisibility(EUIInterface::Esc);
	return true;
}

void UUI_ShowAndHide::CallUIInterfaceOfClass(TMap<TSubclassOf<UUserWidget>, EUIInterface> Class)
{
	for (TPair<TSubclassOf<UUserWidget>, EUIInterface> P : Class)
	{
		switch (P.Value) {
		case EUIInterface::None:
			break;
		case EUIInterface::RemoveFromParent:
			break;
		case EUIInterface::UIInit:
			break;
		case EUIInterface::Show:
			GetUISubSystem()->ShowUIForClass(P.Key);
			break;
		case EUIInterface::Hide:
			GetUISubSystem()->HideUIForClass(P.Key);
			break;
		case EUIInterface::Esc:
			GetUISubSystem()->Esc();
			break;
		}
	}
}

void UUI_ShowAndHide::CallUIInterfaceOfUserWidget(TMap<UUserWidget*, EUIInterface> UserWidget)
{
	for (TPair<UUserWidget*, EUIInterface> P : UserWidget)
	{
		switch (P.Value) {
		case EUIInterface::None:
			break;
		case EUIInterface::RemoveFromParent:
			break;
		case EUIInterface::UIInit:
			break;
		case EUIInterface::Show:
			GetUISubSystem()->ShowUI(P.Key);
			break;
		case EUIInterface::Hide:
			GetUISubSystem()->HideUI(P.Key);
			break;
		case EUIInterface::Esc:
			GetUISubSystem()->Esc();
			break;
		}
	}
}

UUI_SubSystem* UUI_ShowAndHide::GetUISubSystem()
{
	if (!IsValid(UISubSystem))
	{
		UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UUI_SubSystem::StaticClass()));
	}
	return UISubSystem;
}

USoundBase* UUI_ShowAndHide::GetSound(EUIInterface Type)
{
	if (!PlaySoundConfig[Type] && UUI_CommonGameConfig::GetInstance()->DefaultSound.Contains(Type))
	{
		PlaySoundConfig[Type] = UAssetManager::GetStreamableManager().LoadSynchronous(UUI_CommonGameConfig::GetInstance()->DefaultSound[Type]);
	}
	return PlaySoundConfig[Type];
}

void UUI_ShowAndHide::SetParentUIVisibility(EUIInterface Type)
{
	if (ParentUI && ParentUIVisibility.Contains(Type))
	{
		ParentUI->SetVisibility(ParentUIVisibility[Type]);
	}
}
