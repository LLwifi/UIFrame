// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SubSystem.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <EditorConfig/UI_CommonGameConfig.h>
#include <RedPointTree/RedPointTree.h>
#include "Common/UI_FunctionLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/AssetManager.h"

void UUI_SubSystem::Deinitialize()
{
	Super::Deinitialize();
	PC = nullptr;
	AllUI.Empty();
	MainPanel = nullptr;
	ESCList.Empty();
	EscPanel = nullptr;

	UE_LOG(UIFrame, Warning, TEXT("UI_SubSystem Deinitialize"));
}

void UUI_SubSystem::AddUI(UWidget* UI, FName UITag /*= ""*/, bool IsESCCheck /*= true*/)
{
	//确保相同的UI不会被添加两次
	if (IsValid(UI))
	{
		if (IsESCCheck)
		{
			EscAddCheck(UI);
		}
		
		if (!AllUI.Contains(UI->GetClass()) || !AllUI[UI->GetClass()].Contains(UI))
		{
			if (UITag == "" && UI->Implements<UUI_PanelInteract>())
			{
				UITag = IUI_PanelInteract::Execute_GetUITag(UI);
			}
		
			if (AllUI.Contains(UI->GetClass()))
			{
				AllUI[UI->GetClass()].AddWidget(UI, UITag);
			}
			else
			{
				AllUI.Add(UI->GetClass(), FSameTypeUI(UI, UITag));
			}
		
			UUI_WidgetBase* WidgetBase = Cast<UUI_WidgetBase>(UI);
			if (WidgetBase)
			{
				WidgetBase->UITag = UITag;
			}

			RootPanelCheck();
			if (!UI->GetParent())
			{
				AddUIToCanvasPanelLayout(UI,GetCanvasPanelLayoutFormTag(Tag_UI_Layout_Root));
			}
		}
	}
}

void UUI_SubSystem::AddPanel(UUI_PanelBase* UI, FName UITag /*= ""*/)
{
	if (UITag == "")
	{
		AddUI(UI, IUI_PanelInteract::Execute_GetUITag(UI));
	}
	else
	{
		AddUI(UI, UITag);
	}
}

UUserWidget* UUI_SubSystem::CreateUI(TSubclassOf<UUserWidget> UIClass, UUI_WidgetBase* ParentUI /*= nullptr*/, bool IsAddToView /*= true*/, FName UITag /*= ""*/)
{
	if (!IsValid(UIClass))
	{
		return nullptr;
	}

	UUserWidget* Widget;
	if(UKismetSystemLibrary::IsDedicatedServer(this))
	{
		UE_LOG(UIFrame, Warning, TEXT("UI should not be created on the server!!"));
		return nullptr;
	}
	else
	{
		Widget = CreateWidget(GetWorld(), UIClass);
	}

	if (IsValid(Widget))
	{
		//实现了IUI_PanelInteract的Widget
		FName CurUITag;
		if (Widget->Implements<UUI_PanelInteract>())
		{
			CurUITag = IUI_PanelInteract::Execute_GetUITag(Widget);
			
			IUI_PanelInteract::Execute_UIInit(Widget);

			UUI_WidgetBase* WidgetBase = Cast<UUI_WidgetBase>(Widget);
			if (IsValid(WidgetBase))
			{
				WidgetBase->UITag = UITag;
				if (IsValid(ParentUI))
				{
					WidgetBase->ParentUI = ParentUI;
				}
			}
			
			if (IsAddToView)
			{
				ShowUI(Widget);
			}
		}
		else//普通Widget
		{
			CurUITag = UITag;
			if (IsAddToView && !Widget->IsInViewport() && Widget->GetParent() == nullptr)
			{ 
				Widget->AddToViewport();
			}
		}

		AddUI(Widget, CurUITag);

		//实现了红点接口的UI---初始判断是否开启
		if (Widget->Implements<UUI_RedPointInteract>() && CurRedPointTree)
		{
			CurRedPointTree->RedPointPanelShowCheck(UIClass, CurUITag);
		}
	}

	return Widget;
}

void UUI_SubSystem::ShowUI(UWidget* Widget, bool CheckUIState /*= true*/)
{
	//实现了IUI_PanelInteract的Widget
	if (IsValid(Widget))
	{
		//实现了IUI_PanelInteract的Widget
		if (Widget->Implements<UUI_PanelInteract>())
		{
			SetUIInputMode(IUI_PanelInteract::Execute_GetUIInputMode(Widget), Widget);
			AddUI(Widget,IUI_PanelInteract::Execute_GetUITag(Widget));
			
			//并且没有显示
			if (CheckUIState ? !IUI_PanelInteract::Execute_IsShow(Widget) : true)
			{
				SeparatelyUICheck(Widget);
				IUI_PanelInteract::Execute_Show(Widget, Widget, EUIShowHideType::Normal);
			}
		}
		else
		{
			AddUI(Widget);
		}
		
	}
}

void UUI_SubSystem::HideUI(UWidget* Widget, bool CheckUIState /*= true*/, bool CheckESC/* = true*/)
{
	//当前UI正在显示 实现了IUI_PanelInteract的Widget 是否需要检测当前UI的显示隐藏状态
	if (IsValid(Widget) && Widget->Implements<UUI_PanelInteract>() && (CheckUIState?IUI_PanelInteract::Execute_IsShow(Widget):true))
	{
		if (CheckESC)
		{
			// 自己是Esc列表最上层的UI
			UUserWidget* EscTopUI;
			if (FindTopValidEscUI(ESCList.Num() - 1, EscTopUI) && EscTopUI == Widget)
			{
				HideTopEscUI();
			}
		}
		IUI_PanelInteract::Execute_Hide(Widget, Widget, EUIShowHideType::Normal);
	}
}

UWidget* UUI_SubSystem::ShowUIForClass(TSubclassOf<UUserWidget> UIClass, bool CheckUIState /*= true*/, FName UITag /*= ""*/)
{
	UWidget* Widget = GetUI(UIClass,UITag);

	if (IsValid(Widget))
	{
		ShowUI(Widget, CheckUIState);
	}
	else
	{
		Widget = CreateUI(UIClass, nullptr, true, UITag);
	}

	return Widget;
}

void UUI_SubSystem::HideUIForClass(TSubclassOf<UUserWidget> UIClass, FName UITag /*= ""*/, bool CheckUIState/* = true*/, bool CheckESC/* = true*/)
{
	HideUI(GetUI(UIClass, UITag), CheckUIState, CheckESC);
}

void UUI_SubSystem::DeleteUIForClass(TSubclassOf<UUserWidget> UIClass, bool DeleteAllSameType /*= false*/, FName UITag /*= ""*/)
{
	if (!IsValid(UIClass))
	{
		return;
	}

	if (DeleteAllSameType)
	{
		TArray<FName> AllUIName;
		TArray<UWidget*> AllWidget = GetAllUIForClass(UIClass, AllUIName);

		for (int32 i = 0; i < AllWidget.Num(); i++)
		{
			DeleteUI(Cast<UUserWidget>(AllWidget[i]), AllUIName[i]);
		}
	}
	else
	{
		DeleteUI(Cast<UUserWidget>(GetUI(UIClass, UITag)), UITag);
	}
}

void UUI_SubSystem::DeleteUI(UUserWidget* UI, FName UITag /*= ""*/)
{
	if (UI)
	{
		TryRemoveUIFromEsc(UI);
		if (AllUI.Contains(UI->GetClass()) && AllUI[UI->GetClass()].Contains(UITag))
		{
			UI->RemoveFromParent();
			AllUI[UI->GetClass()].AllWidget.Remove(UITag);
			int32 WidgetNum = AllUI[UI->GetClass()].WidgetNum();
			UE_LOG(UIFrame, Log, TEXT("DeleteUI UI-[%s] AllUI[%s].WidgetNum() = %d"),*UI->GetName(),*UI->GetClass()->GetName(),WidgetNum);
			//该类型已经没有UI了
			if (WidgetNum == 0)
			{
				AllUI.Remove(UI->GetClass());
			}

		}
	}
}

UWidget* UUI_SubSystem::GetUI(TSubclassOf<UUserWidget> UIClass, FName UITag /*= ""*/)
{
	if (IsValid(UIClass) && AllUI.Contains(UIClass) && AllUI[UIClass].Contains(UITag))
	{
		return AllUI[UIClass][UITag];
	}

	return nullptr;
}

TArray<UWidget*> UUI_SubSystem::GetAllUIForClass(TSubclassOf<UUserWidget> UIClass, TArray<FName>& AllUITag)
{
	TArray<UWidget*> ReturnWidgets;

	if (IsValid(UIClass) && AllUI.Contains(UIClass))
	{
		AllUI[UIClass].AllWidget.GenerateValueArray(ReturnWidgets);
		AllUI[UIClass].AllWidget.GetKeys(AllUITag);
	}
	return ReturnWidgets;
}

void UUI_SubSystem::SetUIInputMode(EUIInputMode InputMode, UWidget* UserWidget /*= nullptr*/)
{
	if (CurInputMode == InputMode || InputMode == EUIInputMode::None)
	{
		return;
	}
	else
	{
		if (!IsValid(PC))
		{
			PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!IsValid(PC))
			{
				return;
			}
		}

		switch (InputMode)
		{
		case EUIInputMode::None:
			break;
		case EUIInputMode::Game:
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
			PC->bShowMouseCursor = false;
			break;
		}
		case EUIInputMode::UI:
		{
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, UserWidget, EMouseLockMode::LockAlways);
			PC->bShowMouseCursor = true;
			break;
		}
		case EUIInputMode::GameAndUI:
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, UserWidget, EMouseLockMode::LockAlways, false);
			PC->bShowMouseCursor = true;
			break;
		}
		default:
			break;
		}
		CurInputMode = InputMode;
		UIInputModeChange.Broadcast(CurInputMode);
	}
}

void UUI_SubSystem::SetMainPanel(UWidget* NewMainPanel)
{
	FString LastMainPanelName = IsValid(MainPanel)?MainPanel->GetName():"NULL";
	MainPanel = NewMainPanel;
	UE_LOG(UIFrame, Log, TEXT("UpdateMainPanel [%s]——>[%s]"),*LastMainPanelName,*MainPanel->GetName());
	MainUIChange.Broadcast(MainPanel);
}

UWidget* UUI_SubSystem::DebugPanelOperate(bool IsShow /*= true*/)
{
	if (IsShow)
	{
		if (!DebugPanel)//要显示时查看有没有创建
		{
			TSubclassOf<UUserWidget> DebugPanelClass = UAssetManager::GetStreamableManager().LoadSynchronous(UUI_CommonGameConfig::GetInstance()->DebugPanelClass);
			if (DebugPanelClass)
			{
				DebugPanel = ShowUIForClass(DebugPanelClass);
			}
			else
			{
				UE_LOG(UIFrame, Log, TEXT("ShowDebugPanel DebugPanelClass Is Null!!!"));
			}
		}
		else
		{
			ShowUI(DebugPanel);
		}
	}
	else
	{
		if (DebugPanel)
		{
			HideUI(DebugPanel);
		}
	}

	return DebugPanel;
}

void UUI_SubSystem::SeparatelyUICheck(UWidget* UI, bool IsShow /*= true*/)
{
	//继承了接口，并且需要被Esc管控
	if (UI && UI->Implements<UUI_PanelInteract>() && IUI_PanelInteract::Execute_IsControlledByEsc(UI))
	{
		int32 NextSeparatelyUIIndex = -1;
		if (IsShow)//显示
		{
			UpdateEscListTopPanel(UI,UI);
			if (ESCList.Num() == 1)//该面板是从主界面打开的面板
			{
				IUI_PanelInteract::Execute_Show(UI, UI, EUIShowHideType::Esc_FirstUI);
			}
			
			if (IUI_PanelInteract::Execute_IsSeparatelyShow(UI))
			{
				NextSeparatelyUIIndex = ESCList.Num() - 1;//全部UI关闭
			}
			else//“我”本身不是一个需要单独显示的UI
			{
				//查找下一个单独显示的UI，它之上的UI全部显示，它之下的UI全部隐藏
				NextSeparatelyUIIndex = SeparatelyUIIndex.IsValidIndex(SeparatelyUIIndex.Num() - 1)?SeparatelyUIIndex[SeparatelyUIIndex.Num() - 1]:-1;
			}
		}
		else//隐藏
		{
			//查找下一个单独显示的UI，它之上的UI(包括他自己)全部显示，它之下的UI全部隐藏
			NextSeparatelyUIIndex = SeparatelyUIIndex.IsValidIndex(SeparatelyUIIndex.Num() - 1)?SeparatelyUIIndex[SeparatelyUIIndex.Num() - 1]:-1;
			if (ESCList.IsValidIndex(ESCList.Num() - 1))
			{
				UpdateEscListTopPanel(ESCList[ESCList.Num() - 1],UI);
			}
		}
		//处理顶层UI下的全部UI
		UE_LOG(UIFrame, Log, TEXT("Event-Esc----------------------------------------------------------Start"));
		for (int32 i = 0; i < ESCList.Num(); i++)
		{
			EscChangeUIDisplayState(ESCList[i],UI, i >= NextSeparatelyUIIndex);
		}
		EscChangeUIDisplayState(MainPanel,UI, NextSeparatelyUIIndex == -1);
		UE_LOG(UIFrame, Log, TEXT("Event-Esc----------------------------------------------------------End"));
	}
}

void UUI_SubSystem::EscChangeUIDisplayState(UWidget* UI, UWidget* TriggerUI, bool IsShow)
{
	if (UI)
	{
		if (IsShow)
		{
			IUI_PanelInteract::Execute_Show(UI,TriggerUI, EUIShowHideType::Esc);
		}
		else
		{
			IUI_PanelInteract::Execute_Hide(UI,TriggerUI, EUIShowHideType::Esc);
		}
	}
}

void UUI_SubSystem::SetEscPanelClass(TSubclassOf<UUserWidget> PanelClass)
{
	EscPanelClass = PanelClass;
}

void UUI_SubSystem::SetDisableEsc(bool IsDisable)
{
	bIsDisableEsc = IsDisable;
}

void UUI_SubSystem::SetDisableShowEscPanel(bool IsDisable)
{
	bIsDisableShowEscPanel = IsDisable;
}

void UUI_SubSystem::HideTopEscUI()
{
	// 自己是Esc列表最上层的UI
	UUserWidget* EscTopUI;
	if (FindTopValidEscUI(ESCList.Num() - 1,EscTopUI))
	{
		//移除单独显示UI记录的下标
		if (IUI_PanelInteract::Execute_IsSeparatelyShow(EscTopUI))
		{
			SeparatelyUIIndex.Remove(ESCList.Find(EscTopUI));
		}
		
		if (IUI_PanelInteract::Execute_Esc(EscTopUI, ESCList.Last()))
		{
			ESCList.Remove(EscTopUI);
			ESCListChange.Broadcast(ESCList);

			SeparatelyUICheck(EscTopUI, false);

			if (ESCList.Num() > 0)
			{
				SetUIInputMode(IUI_PanelInteract::Execute_GetUIInputMode(ESCList.Last()), ESCList.Last());
			}
			else
			{
				if (MainPanel)
				{
					SetUIInputMode(IUI_PanelInteract::Execute_GetUIInputMode(MainPanel), MainPanel);
				}
				else
				{
					SetUIInputMode(EUIInputMode::Game);
				}
			}

			if (ESCList.Num() == 0)
			{
				UpdateEscListTopPanel(MainPanel, EscTopUI);
			}
		}
	}
}

void UUI_SubSystem::Esc()
{
	if (bIsDisableEsc)//是否禁用了Esc
	{
		return;
	}
	
	if (ESCList.Num() > 0)
	{
		HideTopEscUI();
	}
	else
	{
		ShowEscPanel();
	}
}

bool UUI_SubSystem::FindTopValidEscUI(int32 TopIndex,UUserWidget*& TopUI)
{
	if (ESCList.IsValidIndex(TopIndex))
	{
		//在Esc回退的过程中需要判断要回退开启的那个UI是否有效，无效时略过它的开启逻辑 并一直往后寻找到一个有效的UI
		if (ESCList[TopIndex])
		{
			TopUI = ESCList[TopIndex];
			return true;
		}
		else
		{
			if (ESCList.IsValidIndex(TopIndex))
			{
				ESCList.RemoveAt(TopIndex);//移除这个无效的UI
				ESCListChange.Broadcast(ESCList);
			}
			return FindTopValidEscUI(TopIndex-1,TopUI);
		}
	}
	return false;
}

void UUI_SubSystem::HideAllEscUI(bool IsClearEscList/* = false*/)
{
	TArray<UUserWidget*> ESCListUI = ESCList;
	for (UUserWidget* UserWidget : ESCListUI)
	{
		if (UserWidget)
		{
			IUI_PanelInteract::Execute_Esc(UserWidget,nullptr);
		}
	}
	if(IsClearEscList)
	{
		ESCList.Empty();
	}
	if (MainPanel)
	{
		SetUIInputMode(IUI_PanelInteract::Execute_GetUIInputMode(MainPanel), MainPanel);
	}
	else
	{
		SetUIInputMode(EUIInputMode::Game);
	}
}

void UUI_SubSystem::EscAddCheck(UWidget* Widget)
{
	if (IsValid(Widget) && !ESCList.Contains(Widget))
	{
		//实现了IUI_PanelInteract的UserWidget
		UUserWidget* UserWidget = Cast<UUserWidget>(Widget);
		if (UserWidget && UserWidget->Implements<UUI_PanelInteract>() && IUI_PanelInteract::Execute_IsControlledByEsc(UserWidget))
		{
			ESCList.Add(UserWidget);
			ESCListChange.Broadcast(ESCList);
			if (IUI_PanelInteract::Execute_IsSeparatelyShow(UserWidget))
			{
				SeparatelyUIIndex.Add(ESCList.Find(UserWidget));
			}
		}
	}
}

void UUI_SubSystem::ShowEscPanel()
{
	if (bIsDisableShowEscPanel)
	{
		UE_LOG(UIFrame, Log, TEXT("UUI_SubSystem::ShowEscPanel() bIsDisableShowEscPanel = True"));
		return;
	}

	if (!IsValid(EscPanel))
	{
		if (!EscPanelClass)
		{
			EscPanelClass = UAssetManager::GetStreamableManager().LoadSynchronous(UUI_CommonGameConfig::GetInstance()->EscPanelClass);
			UE_LOG(UIFrame, Log, TEXT("UUI_SubSystem::ShowEscPanel() GetEscPanelClass From UUI_CommonGameConfig"));
		}
		//创建ESC面板
		EscPanel = ShowUIForClass(EscPanelClass);
		UE_LOG(UIFrame, Log, TEXT("UUI_SubSystem::ShowEscPanel() ShowUIForClass(EscPanelClass)"));
	}
	else
	{
		ShowUI(EscPanel);
		UE_LOG(UIFrame, Log, TEXT("UUI_SubSystem::ShowEscPanel() ShowUI(EscPanel)"));
	}
}

bool UUI_SubSystem::TryRemoveUIFromEsc(UUserWidget* UI)
{
	if (UI)
	{
		for (UUserWidget* UserWidget : ESCList)
		{
			if (UI == UserWidget)
			{
				ESCList.Remove(UserWidget);
				ESCListChange.Broadcast(ESCList);
				return true;
			}
		}
	}
	return false;
}

int32 UUI_SubSystem::FindUIIndexFormEscList(UWidget* UI)
{
	return ESCList.Find(Cast<UUserWidget>(UI));
}

int32 UUI_SubSystem::GetEscUINum()
{
	return ESCList.Num();
}

void UUI_SubSystem::UpdateEscListTopPanel(UWidget* NewTopPanel, UWidget* TriggerUI)
{
	if (NewTopPanel)
	{
		IUI_PanelInteract::Execute_Show(NewTopPanel,TriggerUI, EUIShowHideType::Esc_Top);
		if(EscListTopPanel)
		{
			IUI_PanelInteract::Execute_Hide(EscListTopPanel,TriggerUI, EUIShowHideType::Esc_Top);
		}
		FString LastTopPanelName = IsValid(EscListTopPanel)?EscListTopPanel->GetName():"NULL";
		UE_LOG(UIFrame, Log, TEXT("UpdateEscListTopPanel [%s]——>[%s]"),*LastTopPanelName,*NewTopPanel->GetName());
		EscListTopPanel = NewTopPanel;
	}
}

URedPointTree* UUI_SubSystem::SetRedPointTree(URedPointTree* RedPointTree)
{
	if (UKismetSystemLibrary::IsDedicatedServer(this))
	{
		UE_LOG(UIFrame, Warning, TEXT("RedPointTree should not be created on the server!!"));
		return nullptr;
	}
	else if (RedPointTree)
	{
		CurRedPointTree = RedPointTree;
		RedPointTree->Init(UGameplayStatics::GetPlayerPawn(this,0));
	}
	return CurRedPointTree;
}

void UUI_SubSystem::RootPanelCheck()
{
	if (!RootPanel && !RootPanelClass.Get())
	{
		RootPanelClass = UAssetManager::GetStreamableManager().LoadSynchronous(UUI_CommonGameConfig::GetInstance()->RootPanelClass);
		RootPanel = CreateWidget(GetWorld(), RootPanelClass);

		if (RootPanel)
		{
			ShowUI(RootPanel);
			//RootPanel->AddToViewport(0);
			//获取Layout
			TArray<UPanelWidget*> Layouts = UUI_FunctionLibrary::GetAllWidgetFromClass<UPanelWidget>(Cast<UPanelWidget>(RootPanel->GetRootWidget()), UPanelWidget::StaticClass(), true);
			for (UPanelWidget* Panel : Layouts)
			{
				FGameplayTag LayoutTag = FGameplayTag::RequestGameplayTag(FName(String_UI_Layout + Panel->GetName()));
				if (LayoutTag.IsValid())
				{
					AddLayout(LayoutTag, Panel);
				}
			}
		}
	}
}

UPanelWidget* UUI_SubSystem::GetLayoutFormTag(FGameplayTag Tag)
{
	return AllLayout.Contains(Tag)? AllLayout[Tag]: nullptr;
}

UCanvasPanel* UUI_SubSystem::GetCanvasPanelLayoutFormTag(FGameplayTag Tag)
{
	return Cast<UCanvasPanel>(GetLayoutFormTag(Tag));
}

void UUI_SubSystem::AddLayout(FGameplayTag Tag, UPanelWidget* Layout)
{
	AllLayout.Add(Tag,Layout);
}

bool UUI_SubSystem::AddUIToCanvasPanelLayout(UWidget* UI, UCanvasPanel* CanvasLayout)
{
	bool ReturnBool = false;
	if (UI && CanvasLayout)
	{
		UCanvasPanelSlot* CanvasPanelSlot = CanvasLayout->AddChildToCanvas(UI);
		if (CanvasPanelSlot)
		{
			CanvasPanelSlot->SetAnchors(FAnchors(0.0f,0.0f,1.0f,1.0f));
			CanvasPanelSlot->SetOffsets(FMargin(0.0f,0.0f,0.0f,0.0f));
			int32 ZOrder = 0;
			if (UI->Implements<UUI_PanelInteract>())
			{
				ZOrder = (int32)IUI_PanelInteract::Execute_GetUILevel(UI);
			}
			CanvasPanelSlot->SetZOrder(ZOrder);
			ReturnBool = true;
		}
	}
	return ReturnBool;
}

UUserWidget* UUI_SubSystem::GetRootPanel()
{
	return RootPanel;
}

