#include "Common/UI_PanelInteract.h"
#include "UI_SubSystem.h"

DEFINE_LOG_CATEGORY(UIFrame);

void IUI_PanelInteract::UIInit_Implementation()
{

}

FName IUI_PanelInteract::GetUITag_Implementation()
{
	return "";
}

UUserWidget* IUI_PanelInteract::GetParentUI_Implementation()
{
	return nullptr;
}

void IUI_PanelInteract::SetParentUI_Implementation(UUserWidget* Parent)
{
	
}

EUIShowHideType IUI_PanelInteract::GetCurUIShowHideType_Implementation()
{
	return EUIShowHideType::Normal;
}

void IUI_PanelInteract::Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type/* = EUIShowHideType::Normal*/)
{
	
}

void IUI_PanelInteract::Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type/* = EUIShowHideType::Normal*/)
{
	
}

void IUI_PanelInteract::ShowUI_Implementation(UWidget* TriggerUI)
{
}

void IUI_PanelInteract::HideUI_Implementation(UWidget* TriggerUI)
{
}

bool IUI_PanelInteract::IsFollowParentInteract_Implementation()
{
	return true;
}

TArray<EUIShowHideType> IUI_PanelInteract::GetFollowParentShowHideType_Implementation()
{
	TArray<EUIShowHideType> UIShowHideType;
	return UIShowHideType;
}

bool IUI_PanelInteract::IsControlledByEsc_Implementation()
{
	return false;
}

bool IUI_PanelInteract::IsClearHistoricalRecords_Implementation()
{
	return false;
}

bool IUI_PanelInteract::Esc_Implementation(UWidget* TriggerUI)
{
	return true;
}

bool IUI_PanelInteract::IsSeparatelyShow_Implementation()
{
	return false;
}

bool IUI_PanelInteract::IsShow_Implementation()
{
	return true;
}

EUIInputMode IUI_PanelInteract::GetUIInputMode_Implementation()
{
	return EUIInputMode::None;
}

EUILevel IUI_PanelInteract::GetUILevel_Implementation()
{
	return EUILevel::Bot;
}

FGameplayTag IUI_PanelInteract::GetUILayoutTag_Implementation()
{
	return Tag_UI_Layout_Mid;
}

void IUI_Tip::AddTipText_Implementation(FUI_TipInfo)
{
}
