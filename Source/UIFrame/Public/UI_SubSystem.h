// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include <Panel/UI_PanelBase.h>
#include "UI_SubSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUISubSystem_ESCChange, const TArray<UUserWidget*>&, ESCList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUISubSystem_UIInputModeChange, EUIInputMode, UIInputMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUISubSystem_MainUIChange, UWidget*, MainUI);

/**
 UI子系统
 通过该系统创建的UI可以被有效的管理起来
 该系统同时可以创建UUserWidget类型的UI
 如果创建的是一个继承自UUI_PanelBase的UI，那么它还具体其他的一些管理功能
 */
UCLASS()
class UIFRAME_API UUI_SubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

public:

	/*往UI管理器中添加UI
	* IsESCCheck:是否需要进行ESC检测，对于一个已经显示的UI来说在添加时检测ESC可以方便管理
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "UI"))
		void AddUI(UWidget* UI,FName UITag = "", bool IsESCCheck = true);
	
	/* 往UI管理器中添加Panel，如果UITag有值会使用UITag的值，如果UITag无值使用Panel本身的UITag
	 * 没用UI_WidgetBase是因为理论上UI框架只会保存各种面板不会管理更细的UI_WidgetBase
	 */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "UI"))
		void AddPanel(UUI_PanelBase* UI, FName UITag = "");

	/*
	* 创建UI（UI类型，父UI，UI标记，是否添加到屏幕）
	* 该函数允许可重复创建UI
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "3", DeterminesOutputType = "UIClass"))
		UUserWidget* CreateUI(TSubclassOf<UUserWidget> UIClass, UUI_WidgetBase* ParentUI = nullptr, bool IsAddToView = true, FName UITag = "");

	/*
	* 通过UI直接显示
	* UUserWidget接口认为用户已经拿到了具体UI而调用
	* CheckUIState：是否要检测UI当前的显示状态来判断是否调用真正的显示函数
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1", DefaultToSelf = "Widget"))
		void ShowUI(UWidget* Widget, bool CheckUIState = true);

	/*通过UI直接隐藏
	* UUserWidget接口认为用户已经拿到了具体UI而调用
	* CheckUIState：是否要检测UI当前的显示状态来判断是否调用真正的显示函数
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1", DefaultToSelf = "Widget"))
		void HideUI(UWidget* Widget, bool CheckUIState = true, bool CheckESC = true);

	/*
	* 显示UI（UI类型，UI标记）
	* 该UI未创建时，通过CreateUI创建-》显示-》返回
	* 该UI创建时，查找-》显示-》返回
	* Class版本的接口的使用情况会认为是没有拿到具体UI的情况下调用
	* CheckUIState：是否要检测UI当前的显示状态来判断是否调用真正的显示函数
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1", DeterminesOutputType = "UIClass"))
		UWidget* ShowUIForClass(TSubclassOf<UUserWidget> UIClass, bool CheckUIState = true, FName UITag = "");

	/*
	* 隐藏UI（UI类型，UI标记）
	* Class版本的接口的使用情况会认为是没有拿到具体UI的情况下调用
	* CheckUIState：是否检测UI状态，若已经Hide了则会return
	* CheckESC:是否检测ESC 若该值为false则仅仅调用UI上的Hide函数而不会进行ESC列表相关检测（如弹出最上层）
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
		void HideUIForClass(TSubclassOf<UUserWidget> UIClass, FName UITag = "", bool CheckUIState = true, bool CheckESC = true);

	//删除UI（UI类型，是否删除全部同类型的UI，UI标记）
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "2"))
		void DeleteUIForClass(TSubclassOf<UUserWidget> UIClass, bool DeleteAllSameType = false, FName UITag = "");
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
		void DeleteUI(UUserWidget* UI,FName UITag = "");
	
	//获取UI（UI类型，UI标记）
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "1", DeterminesOutputType = "UIClass"))
		UWidget* GetUI(TSubclassOf<UUserWidget> UIClass, FName UITag = "");

	//获取全部同类型的UI（UI类型）
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "1", DeterminesOutputType = "UIClass"))
		TArray<UWidget*> GetAllUIForClass(TSubclassOf<UUserWidget> UIClass, TArray<FName>& AllUITag);

	//设置UI输入模式
	UFUNCTION(BlueprintCallable)
		void SetUIInputMode(EUIInputMode InputMode, UWidget* UserWidget = nullptr);

	//设置主UI
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "NewMainPanel"))
		void SetMainPanel(UWidget* NewMainPanel);

	//调试面板
	UFUNCTION(BlueprintCallable)
	UWidget* DebugPanelOperate(bool IsShow = true);
	 
public:
	UPROPERTY(BlueprintAssignable)
	FUISubSystem_MainUIChange MainUIChange;
	UPROPERTY(BlueprintAssignable)
	FUISubSystem_UIInputModeChange UIInputModeChange;

	//玩家控制器，用于设置输入模式
	UPROPERTY()
		APlayerController* PC;

	UPROPERTY(BlueprintReadOnly)
		EUIInputMode CurInputMode = EUIInputMode::Game;

	//保存全部创建的UI<UIClass,同类型全部的UI>
	UPROPERTY(BlueprintReadOnly)
		TMap<TSubclassOf<UWidget>, FSameTypeUI> AllUI;

	/*当前游戏的主UI
	 * 主UI是常显在屏幕上的UI，不会被Esc列表管理起来
	 * 不同地图模式关卡下的主面板不同，所以需要使用SetMainPanel动态赋予
	 */
	UPROPERTY(BlueprintReadOnly)
		UWidget* MainPanel;

	//调试面板
	UPROPERTY(BlueprintReadOnly)
		UWidget* DebugPanel;

public:
	/*--------------------------------------ESC------------------------------------------
	* 当面板继承IUI_PanelInteract并且函数IsControlledByEsc返回为真时，该面板会被添加到ESCList中进行管理
	*/

	//单独显示UI处理：通常来说一定是一个继承接口的类才会调用该函数
	UFUNCTION()
		void SeparatelyUICheck(UWidget* UI, bool IsShow = true);

	//Esc控制的显示隐藏函数，这种显示隐藏不会调用面板的ShowOrHide函数
	UFUNCTION()
		void EscChangeUIDisplayState(UWidget* UI, UWidget* TriggerUI, bool IsShow = true);

	//设置EscPanel的类型
	UFUNCTION(BlueprintCallable)
		void SetEscPanelClass(TSubclassOf<UUserWidget> PanelClass);

	/*是否禁用ESC操作，某些游戏阶段你可能希望等待事件返回或者进行不可打断操作时，调用该函数暂时禁用Esc功能
	*/
	UFUNCTION(BlueprintCallable)
		void SetDisableEsc(bool IsDisable);

	UFUNCTION(BlueprintCallable)
		void SetDisableShowEscPanel(bool IsDisable);

	UFUNCTION()
		void HideTopEscUI();

	/*Esc操作，关闭ESCList最上层UI
	* 当ESCList为空时，尝试开启EscPanel面板
	*/
	UFUNCTION(BlueprintCallable)
		void Esc();

	//EscUI的有效检测 递归
	UFUNCTION()
		bool FindTopValidEscUI(int32 TopIndex,UUserWidget*& TopUI);

	/*隐藏全部esc列表的UI
	* IsClearEscList：要不要清除ESCList，隐藏不一定需要清空数据，现在开启一个单独显示的UI时会隐藏ESC列表的全部UI
	*/
	UFUNCTION(BlueprintCallable)
		void HideAllEscUI(bool IsClearEscList = false);

	//将UI添加进Esc列表的检查，如果该UI实现了UI_PanelInteract，那么它会被成功添加
	UFUNCTION(BlueprintCallable)
		void EscAddCheck(UWidget* Widget);

	//尝试创建或显示EscPanel 如果想要成功创建需要设置EscPanelClass并将bIsAllowShowEscPanel设置为true
	UFUNCTION(BlueprintCallable)
		void ShowEscPanel();

	//尝试从ESC中移除某个UI 若UI不在ESC列表中移除失败
	UFUNCTION(BlueprintCallable)
		bool TryRemoveUIFromEsc(UUserWidget* UI);

	//该UI在Esc列表的下标（第几位）最下层为0   -1表示为找到
	UFUNCTION(BlueprintPure)
		int32 FindUIIndexFormEscList(UWidget* UI);
	
	UFUNCTION(BlueprintPure)
		int32 GetEscUINum();

	//顶部UI离开检查
	UFUNCTION()
		void UpdateEscListTopPanel(UWidget* NewTopPanel, UWidget* TriggerUI);

public:
	UPROPERTY(BlueprintAssignable)
	FUISubSystem_ESCChange ESCListChange;

	UPROPERTY(BlueprintReadOnly)
		TArray<UUserWidget*> ESCList;

	//单独显示UI的下标标记
	UPROPERTY(BlueprintReadOnly)
		TArray<int32> SeparatelyUIIndex;

	//Esc面板
	UPROPERTY(BlueprintReadOnly)
		UWidget* EscPanel;

	//Esc最上层面板
	UPROPERTY(BlueprintReadOnly)
		UWidget* EscListTopPanel;

	//Esc面板类型
	UPROPERTY(BlueprintReadOnly)
		TSubclassOf<UUserWidget> EscPanelClass;

	UPROPERTY(BlueprintReadOnly)
		bool bIsDisableEsc = false;

	UPROPERTY(BlueprintReadOnly)
		bool bIsDisableShowEscPanel = false;
public:
	/*--------------------------------------RedPointTree------------------------------------------
	* 红点树：该函数最好在全部UI创建前调用
	*/
	UFUNCTION(BlueprintCallable)
		URedPointTree* SetRedPointTree(URedPointTree* RedPointTree);

public:
	UPROPERTY(BlueprintReadOnly)
		URedPointTree* CurRedPointTree;

//---------------------------------------------------------------------Layout
public:
	UFUNCTION()
		void RootPanelCheck();
	
	//通过Tag获取Layout
	UFUNCTION(BlueprintPure)
		UPanelWidget* GetLayoutFormTag(FGameplayTag Tag);

	//通过Tag获取Layout
	UFUNCTION(BlueprintPure)
		UCanvasPanel* GetCanvasPanelLayoutFormTag(FGameplayTag Tag);

	//通过Tag动态添加Layout
	UFUNCTION(BlueprintCallable)
		void AddLayout(FGameplayTag Tag, UPanelWidget* Layout);
	
	//将UI添加进Layout中去
	UFUNCTION(BlueprintCallable)
		bool AddUIToCanvasPanelLayout(UWidget* UI, UCanvasPanel* CanvasLayout);

	//通过Tag获取Layout
	UFUNCTION(BlueprintPure)
		UUserWidget* GetRootPanel();
public:
	//保存全部的Layout<Tag,Layout>
	UPROPERTY(BlueprintReadOnly)
		TMap<FGameplayTag, UPanelWidget*> AllLayout;

	//Root面板的类型
	UPROPERTY(BlueprintReadOnly)
		TSubclassOf<UUserWidget> RootPanelClass;
	
	//当前游戏的根UI
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* RootPanel;
};
