// Fill out your copyright notice in the Description page of Project Settings.


#include "Panel/UI_PanelBase.h"
#include <Blueprint/WidgetTree.h>
#include <Common/UI_PanelInteract.h>

#if WITH_EDITOR
void UUI_PanelBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

bool UUI_PanelBase::Initialize()
{
	//1
	return Super::Initialize();
}

void UUI_PanelBase::SynchronizeProperties()
{
	//2  当属性改变时触发，同步数据给Slate控件
	Super::SynchronizeProperties();

#if WITH_EDITOR
	AddChildrenUIsFromRootPanel();//为了在编辑器下看到子类数量
#endif
}

void UUI_PanelBase::NativePreConstruct()
{
	//3
	Super::NativePreConstruct();
}

void UUI_PanelBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUI_PanelBase::NativeConstruct()
{
	Super::NativeConstruct();
	//AddChildrenUIsFromRootPanel();
}

void UUI_PanelBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_PanelBase::RemoveFromParent()
{
	// if (GWorld && GWorld->IsGameWorld())
	// {
	// 	CallChildUIFunction(EUIInterface::RemoveFromParent);
	// 	ChildUIs.Empty();
	// }
	Super::RemoveFromParent();
}

void UUI_PanelBase::BeginDestroy()
{
	ChildUIs.Empty();
	Super::BeginDestroy();
}

void UUI_PanelBase::UIInit_Implementation()
{
	Super::UIInit_Implementation();
	AddChildrenUIsFromRootPanel();
	CallChildUIFunction(EUIInterface::UIInit);
}

void UUI_PanelBase::Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type)
{
	Super::Show_Implementation(TriggerUI, Type);
	CallChildUIFunction(EUIInterface::Show, TriggerUI);
}

void UUI_PanelBase::Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type)
{
	Super::Hide_Implementation(TriggerUI, Type);
	CallChildUIFunction(EUIInterface::Hide, TriggerUI);
}

bool UUI_PanelBase::Esc_Implementation(UWidget* TriggerUI)
{
	Super::Esc_Implementation(TriggerUI);
	CallChildUIFunction(EUIInterface::Esc, TriggerUI);
	return true;
}

void UUI_PanelBase::AddChildrenUI(UWidget* Widget)
{
	if (IsValid(Widget) && Widget->Implements<UUI_PanelInteract>())
	{
		//相同Class的UI放进一个数组
		if (ChildUIs.Contains(Widget->GetClass()))
		{
			ChildUIs[Widget->GetClass()].AddWidget(Widget, FName(*Widget->GetName()));
		}
		else
		{
			ChildUIs.Add(Widget->GetClass(), FSameTypeUI(Widget, FName(*Widget->GetName())));
		}
		//设置父UI
		IUI_PanelInteract::Execute_SetParentUI(Widget,this);
	}
}

void UUI_PanelBase::AddChildrenUIsFromPanelWidget(UPanelWidget* PanelWidget)
{
	if (!IsValid(PanelWidget))
	{
		return;
	}

	//获取这个Panel下的全部UWidget
	TArray<UWidget*> AllChildren = PanelWidget->GetAllChildren();
	for (int32 i = 0; i < AllChildren.Num(); i++)
	{
		if (!AllChildren[i])
		{
			continue;
		}
		//实现了接口的UI均视为子UI
		if (AllChildren[i]->Implements<UUI_PanelInteract>())
		{
			AddChildrenUI(AllChildren[i]);
		}
		else
		{
			UPanelWidget* Panel = Cast<UPanelWidget>(AllChildren[i]);
			if (IsValid(Panel))
			{
				AddChildrenUIsFromPanelWidget(Panel);
			}
		}
	}
}

void UUI_PanelBase::AddChildrenUIsFromRootPanel()
{
	UPanelWidget* Panel = Cast<UPanelWidget>(GetRootWidget());
	if (IsValid(Panel))
	{
		ChildUIs.Empty();
		AddChildrenUIsFromPanelWidget(Panel);
	}
}

void UUI_PanelBase::CallChildUIFunction(EUIInterface Function,UWidget* TriggerUI /*= nullptr*/)
{
	if (ChildUIs.Num() <= 0 || Function == EUIInterface::None)
	{
		return;
	}
	//这部分的逻辑可以使用反射调用函数进行重复代码的优化
	TMap<TSubclassOf<UWidget>, FSameTypeUI> AllChildUI = ChildUIs;
	for (TPair<TSubclassOf<UUserWidget>, FSameTypeUI> Pair : AllChildUI)
	{
		for (TPair<FName, UWidget*> Pair2 : Pair.Value.AllWidget)
		{
			//通过判断父类的有效性，在判断该Widget是否被删除了——被删除的Widget不需要调用UI生命周期函数
			if (IsValid(Pair2.Value) && Pair2.Value->GetParent() 
			&& IUI_PanelInteract::Execute_IsFollowParentInteract(Pair2.Value))
			{
				switch (Function) {
				case EUIInterface::None:
					{
						break;
					}
				case EUIInterface::RemoveFromParent:
					{
						Pair2.Value->RemoveFromParent();
						break;
					}
				case EUIInterface::UIInit:
					{
						IUI_PanelInteract::Execute_UIInit(Pair2.Value);
						break;
					}
				case EUIInterface::Show:
					{
						if (IUI_PanelInteract::Execute_GetFollowParentShowHideType(Pair2.Value).Contains(CurUIShowHideType))
						{
							IUI_PanelInteract::Execute_Show(Pair2.Value, TriggerUI, EUIShowHideType::Parent);
						}
						break;
					}
				case EUIInterface::Hide:
					{
						if (IUI_PanelInteract::Execute_GetFollowParentShowHideType(Pair2.Value).Contains(CurUIShowHideType))
						{
							IUI_PanelInteract::Execute_Hide(Pair2.Value, TriggerUI, EUIShowHideType::Parent);
						}
						break;
					}
				case EUIInterface::Esc:
					{
						IUI_PanelInteract::Execute_Esc(Pair2.Value, TriggerUI);
						break;
					}
				}
			}
		}
	}
}

UWidget* UUI_PanelBase::GetChildUIForClass(TSubclassOf<UWidget> UIClass, FName ChildrenName /*= "GetIndex0UI"*/)
{
	if (IsValid(UIClass))
	{
		if (ChildUIs.Contains(UIClass))
		{
			if (ChildrenName == "GetIndex0UI")
			{
				TArray<FName> AllKey;
				ChildUIs[UIClass].AllWidget.GetKeys(AllKey);
				return ChildUIs[UIClass][AllKey[0]];
			}
			else if (ChildUIs[UIClass].Contains(ChildrenName))
			{
				return ChildUIs[UIClass][ChildrenName];
			}
			
		}
	}
	return nullptr;
}

TArray<UWidget*> UUI_PanelBase::GetAllChildUIForClass(TSubclassOf<UWidget> UIClass, TArray<FName>& AllUIName)
{
	TArray<UWidget*> ReturnWidgets;
	if (IsValid(UIClass))
	{
		if (ChildUIs.Contains(UIClass))
		{
			ChildUIs[UIClass].AllWidget.GenerateValueArray(ReturnWidgets);
			ChildUIs[UIClass].AllWidget.GetKeys(AllUIName);
		}
	}
	return ReturnWidgets;
}
