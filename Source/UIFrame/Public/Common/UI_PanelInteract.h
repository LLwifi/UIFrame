#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "UI_StructAndEnum.h"
#include "UI_PanelInteract.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UIFrame, Log, All);

#define String_UI_Layout "UI.Layout."
#define Tag_UI_Layout_Root FGameplayTag::RequestGameplayTag(FName("UI.Layout.Root"))
#define Tag_UI_Layout_SeparatelyShow FGameplayTag::RequestGameplayTag(FName("UI.Layout.SeparatelyShow"))
#define Tag_UI_Layout_PanelShow FGameplayTag::RequestGameplayTag(FName("UI.Layout.PanelShow"))
#define Tag_UI_Layout_Bot FGameplayTag::RequestGameplayTag(FName("UI.Layout.Bot"))
#define Tag_UI_Layout_Mid FGameplayTag::RequestGameplayTag(FName("UI.Layout.Mid"))
#define Tag_UI_Layout_Top FGameplayTag::RequestGameplayTag(FName("UI.Layout.Top"))
#define Tag_UI_Layout_System FGameplayTag::RequestGameplayTag(FName("UI.Layout.System"))

//UI层级 枚举值对应ZOrder
UENUM(BlueprintType)
enum class EUILevel :uint8
{
	Background = 0 UMETA(DisplayName = "背景层"),
	Bot UMETA(DisplayName = "低层"),
	Mid UMETA(DisplayName = "中层-显示层"),
	Top UMETA(DisplayName = "上层"),
	System UMETA(DisplayName = "系统层")
};

//输入模式
UENUM(BlueprintType)
enum class EUIInputMode :uint8
{
	None = 0 UMETA(DisplayName = "无"),
	Game UMETA(DisplayName = "操作游戏"),
	UI UMETA(DisplayName = "操作UI_鼠标"),
	GameAndUI UMETA(DisplayName = "操作游戏和UI_鼠标")
};

//UI接口枚举
UENUM(BlueprintType)
enum class EUIInterface :uint8
{
	None = 0 UMETA(DisplayName = "无"),
	RemoveFromParent,
	UIInit,
	Show,
	Hide,
	Esc
};

//UI显示枚举
UENUM(BlueprintType)
enum class EUIShowHideType :uint8
{
	//通过UI_SubSystem或调用的Show/Hide
	Normal = 0 UMETA(DisplayName = "正常"),
	//由父节点调用的Show/Hide
	Parent UMETA(DisplayName = "父节点"),
	//当UI_SubSystem的EscList为空时，该函数会在Show之后被调用
	Esc_FirstUI UMETA(DisplayName = "Esc_首个"),
	//进入或离开ESC最上层
	Esc_Top UMETA(DisplayName = "Esc_顶层"),
	//Esc框架调用的Show/Hide
	Esc UMETA(DisplayName = "Esc")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUI_PanelInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * UI面板交互
 * 如果是直接继承该接口，至少需要实现以下函数
 * IsShow：获取当前UI的显示状态
 * Show：用于显示UI，需要注意的是该函数会被反复调用，可以利用IsShow使得最终的显示只运行一次（已经显示后不在显示）
 * Hide：用于隐藏UI，需要注意的是该函数会被反复调用，可以利用IsShow使得最终的显示只运行一次（已经显示后不在显示）
 * IsControlledByEsc：是否被Esc管理
 * Esc：当IsControlledByEsc返回为true时，该函数决定了这个UI在Esc时需要处理的操作，通常可以等同于HideUI
 */
class UIFRAME_API IUI_PanelInteract
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*
	* 初始化函数 不同于Construct在添加到屏幕才会被调用，init在创建时就会被调用
	* 使用UI_SubSystem的Create函数会调用该接口
	*/
	UFUNCTION(BlueprintNativeEvent)
		void UIInit();
	virtual void UIInit_Implementation();

	/*
	* 获取UI的Tag
	* 在UI被添加进UI框架时，会使用该函数的返回值
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FName GetUITag();
	virtual FName GetUITag_Implementation();

	//获取UI的父面板
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UUserWidget* GetParentUI();
	virtual UUserWidget* GetParentUI_Implementation();
	//设置父面板
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetParentUI(UUserWidget* Parent);
	virtual void SetParentUI_Implementation(UUserWidget* Parent);

	//获取当前UI显示枚举
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EUIShowHideType GetCurUIShowHideType();
	virtual EUIShowHideType GetCurUIShowHideType_Implementation() = 0;
	
	//显示事件（触发显示事件的UI,显示的时机枚举）该事件可能会被多次调用，可以通过子类实现不同时机Show该干的事情（需要注意该事件需要调用父类的处理）
	UFUNCTION(BlueprintNativeEvent)
		void Show(UWidget* TriggerUI, EUIShowHideType Type = EUIShowHideType::Normal);
	virtual void Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type = EUIShowHideType::Normal);
	//隐藏事件（触发显示事件的UI,显示的时机枚举）该事件可能会被多次调用，可以通过子类实现不同时机Show该干的事情（需要注意该事件需要调用父类的处理）
	UFUNCTION(BlueprintNativeEvent)
		void Hide(UWidget* TriggerUI, EUIShowHideType Type = EUIShowHideType::Normal);
	virtual void Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type = EUIShowHideType::Normal);

	//显示UI，通常只用于UI开启的显示逻辑（例如使用UI动画显示或是简单的设置Visible）
	UFUNCTION(BlueprintNativeEvent)
	void ShowUI(UWidget* TriggerUI);
	virtual void ShowUI_Implementation(UWidget* TriggerUI);
	//隐藏UI，通常只用于UI关闭的隐藏逻辑（例如使用UI动画显示或是简单的设置Visible）
	UFUNCTION(BlueprintNativeEvent)
	void HideUI(UWidget* TriggerUI);
	virtual void HideUI_Implementation(UWidget* TriggerUI);
	
	//是否跟随父UI一同调用IUI_PanelInteract相关接口
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsFollowParentInteract();
	virtual bool IsFollowParentInteract_Implementation();

	//Show和Hide要在哪些情况下被调用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<EUIShowHideType> GetFollowParentShowHideType();
	virtual TArray<EUIShowHideType> GetFollowParentShowHideType_Implementation() = 0;

	//是否被Esc控制
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsControlledByEsc();
	virtual bool IsControlledByEsc_Implementation();

	/*当“我”已经存在于ESC列表中被再次要求显示（Show）时，是否要移除之前ESC列表中的“我”（是否要移除之前关于“我”的记录）
	* 想要实现该功能，该UI必须被ESC管理起来：IsControlledByEsc返回为true
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsClearHistoricalRecords();
	virtual bool IsClearHistoricalRecords_Implementation();

	/*Esc框架下隐藏UI会调用的函数，默认情况下会调用UI的Hide函数
	* 返回值：表示UI是否准备好了从Esc列表中移除 返回true时才会真正从EscList中移除自身
	* 该函数C++默认调用了自身的Hide 默认返回为true
	* 
	* 注意：在该函数的中使用UUI_SubSystem调用Hide的时候CheckESC 必须为false 否则会触发死循环
	*/
	UFUNCTION(BlueprintNativeEvent)
		bool Esc(UWidget* TriggerUI);
	virtual bool Esc_Implementation(UWidget* TriggerUI);

	/*当前UI在Show时是否需要独立显示
	想要实现该功能，该UI必须被ESC管理起来：IsControlledByEsc返回为true
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsSeparatelyShow();
	virtual bool IsSeparatelyShow_Implementation();

	/*获取当前UI的显示状态
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsShow();
	virtual bool IsShow_Implementation() = 0;

	//当该面板显示到最上层时调用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		EUIInputMode GetUIInputMode();
	virtual EUIInputMode GetUIInputMode_Implementation();

	//获取当前UI层级
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EUILevel GetUILevel();
	virtual EUILevel GetUILevel_Implementation();

	//获取UI会被添加到的Slot标记
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetUILayoutTag();
	virtual FGameplayTag GetUILayoutTag_Implementation();
};

UINTERFACE(MinimalAPI)
class UUI_Tip : public UInterface
{
	GENERATED_BODY()
};

/**
 * UI提示接口
 */
class UIFRAME_API IUI_Tip
{
	GENERATED_BODY()
public:
	/*
	* 添加一个提示文本（提示文本，显示时长）
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddTipText(FUI_TipInfo TipInfo);
	virtual void AddTipText_Implementation(FUI_TipInfo TipInfo);

};