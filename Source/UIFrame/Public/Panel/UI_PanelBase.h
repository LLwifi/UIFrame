// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI_WidgetBase.h"
#include "UI_PanelBase.generated.h"

class UUI_ComponentBase;

/*
* 同类型Widget_一定要使用Widget是因为某些纯功能组件是继承的Widget
* 可以将该结构体当作Map容器使用
*/
USTRUCT(BlueprintType)
struct FSameTypeUI
{
	GENERATED_BODY()

public:
	FSameTypeUI() {};
	FSameTypeUI(UWidget* Widget, FName UIName = "") { AddWidget(Widget, UIName); };
	UWidget* operator[](FName Key) { return Contains(Key)? AllWidget[Key]:nullptr; };
	int32 WidgetNum() { return AllWidget.Num(); };
	void AddWidget(UWidget* Widget, FName UIName = "") { AllWidget.Add(UIName, Widget); };
	void RemoveWidget(FName UIName = "")
	{
		if (AllWidget.Num() == 0)
		{
			return;
		}
		else if (AllWidget.Num() == 1)
		{
			AllWidget.Empty();
		}
		else
		{
			AllWidget.Remove(UIName);
		}
	}
	bool Contains(FName UIName) { return AllWidget.Contains(UIName); };
	bool Contains(UWidget* UI)
	{
		TArray<UWidget*> Widgets;
		AllWidget.GenerateValueArray(Widgets);
		return Widgets.Contains(UI);
	};
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TMap<FName, UWidget*> AllWidget;
};

/**
* 面板的基类
* 当一个UI包含其他多个控件（或面板）同时又由这些控件（或面板）组成时，视为是一个面板
* 面板功能：
* UITag：在Panel层面通常为玩家创建时自定义，用于区分多个同类型Panel同时存在的情况
* bIsShow：面板的显示/隐藏状态（某些特殊情况下如：透明度为0时视为该面板隐藏）
* 设置输入模式UIInputMode
* 子级UI结构ChildUIs以及获取子UI的一系列方法，UI_Panel可以标记自己拥有哪些子UI（子UI包括UI_Widget和其他Panel）
* 渲染层级UILevel
* 
 */
UCLASS()
class UIFRAME_API UUI_PanelBase : public UUI_WidgetBase
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual bool Initialize() override;

	//在编辑器设置控件属性可以在该函数中操作
	virtual void SynchronizeProperties() override;
	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void RemoveFromParent() override;

	virtual void BeginDestroy() override;


	
public:
	//初始化函数 不同于Construct在添加到屏幕才会被调用，init在创建时就会被调用
	virtual void UIInit_Implementation() override;
	//显示隐藏
	virtual void Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
	virtual void Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
	virtual bool Esc_Implementation(UWidget* TriggerUI) override;

public:
	//增加一个子UI
	UFUNCTION(BlueprintCallable)
		void AddChildrenUI(UWidget* Widget);

	//获取全部UI组件
	UFUNCTION(BlueprintCallable)
		void AddChildrenUIsFromPanelWidget(UPanelWidget* PanelWidget);

	UFUNCTION(BlueprintCallable)
		void AddChildrenUIsFromRootPanel();

	/*
	* 调用子UI的接口
	* FunctionName根据UI_PanelInteract中的函数名传
	*/
	UFUNCTION()
		void CallChildUIFunction(EUIInterface Function,UWidget* TriggerUI = nullptr);

	/*
	* 通过Class和ChildrenName获取子UI
	* ChildrenName 为 GetIndex0UI 时获取该类型的第一个UI
	*/
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "UIClass"))
		UWidget* GetChildUIForClass(TSubclassOf<UWidget> UIClass, FName ChildrenName = "GetIndex0UI");
	//通过Class获取子UI
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "UIClass"))
		TArray<UWidget*> GetAllChildUIForClass(TSubclassOf<UWidget> UIClass, TArray<FName>& AllUIName);

public:

	//子面板
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Panel")
		TMap<TSubclassOf<UWidget>, FSameTypeUI> ChildUIs;
};
