// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/UI_PanelInteract.h"
#include "Blueprint/UserWidget.h"
#include "UI_WidgetBase.generated.h"

/**
 * 控件基类
 * 当玩家希望创建包含某种功能却不独立显示的控件时，继承该类
 * 该类通常依附UI_Panel上显示
 * 控件功能：
 * 继承IUI_PanelInteract接口，在UI框架许多地方使用，该接口包含统一的初始化、显示、隐藏等功能
 * UITag：在Widget层面通常理解为UIName，一般是自动获取该控件在某个Panel下的控件名作为UITag
 * bIsShow：控件的显示/隐藏状态（某些特殊情况下如：透明度为0时视为该控件隐藏）
 * bFollowParentInteract：是否要跟随父类的UI接口同时调用（如：父UI在Show时调用子UI的show）
 * 父级UI结构ParentUI，UI_Widget可以标记自己属于哪个UI
 */
UCLASS()
class UIFRAME_API UUI_WidgetBase : public UUserWidget, public IUI_PanelInteract
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void SynchronizeProperties() override;
#endif

	virtual void RemoveFromParent() override;
	virtual void BeginDestroy() override;

	//初始化函数 不同于Construct在添加到屏幕才会被调用，init在创建时就会被调用
	virtual void UIInit_Implementation() override;
	virtual FName GetUITag_Implementation() override;
	virtual UUserWidget* GetParentUI_Implementation() override;
	virtual void SetParentUI_Implementation(UUserWidget* Parent) override;
	//显示隐藏
	virtual void Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type = EUIShowHideType::Normal) override;
	virtual void Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type = EUIShowHideType::Normal) override;
	virtual void ShowUI_Implementation(UWidget* TriggerUI) override;
	virtual void HideUI_Implementation(UWidget* TriggerUI) override;
	virtual bool IsFollowParentInteract_Implementation() override;
	virtual bool IsControlledByEsc_Implementation() override;
	virtual bool IsClearHistoricalRecords_Implementation() override;
	virtual bool Esc_Implementation(UWidget* TriggerUI) override;
	virtual bool IsSeparatelyShow_Implementation() override;
	virtual EUIInputMode GetUIInputMode_Implementation() override;
	virtual EUILevel GetUILevel_Implementation() override;
	virtual FGameplayTag GetUILayoutTag_Implementation() override;
	virtual bool IsShow_Implementation() override;
	virtual EUIShowHideType GetCurUIShowHideType_Implementation() override;
	virtual TArray<EUIShowHideType> GetFollowParentShowHideType_Implementation() override;
	
	UFUNCTION(BlueprintPure)
		UWidget* GetWidgetFormTag(FGameplayTag Tag);

public:
	//父面板
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Widget")
		UUserWidget* ParentUI;

	//渲染层级
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
		EUILevel UILevel = EUILevel::Mid;

	//UI会被添加到的Slot标记
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
		FGameplayTag LayoutTag = Tag_UI_Layout_Root;

	//该UI的输入模式，当开始操作该UI时会根据该值设置输入模式
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		EUIInputMode UIInputMode = EUIInputMode::None;

	//该UI的显示状态  在自定义的某些情况下透明度为0也可以视为隐藏了
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = True))
		bool bIsShow = false;
	
	//UI标签 该值通常用于区分多个同类型的UI
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (ExposeOnSpawn = True), Category = "Widget")
		FName UITag = "";

	//给子UI判断当前父UI是因为什么原因被Show/Hide
	UPROPERTY()
		EUIShowHideType CurUIShowHideType;

	//是否跟随父节点一同调用IUI_PanelInteract相关接口
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract")
		bool bFollowParentInteract = false;

	//Show和Hide要在哪些情况下被调用
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract", meta = (EditCondition = "bFollowParentInteract"))
		TArray<EUIShowHideType> FollowParentShowHideType = {EUIShowHideType::Normal};

	//是否被Esc控制
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract")
		bool bIsControlledByEsc = false;

	/*当“我”已经存在于ESC列表中被再次要求显示（Show）时，是否要移除之前ESC列表中的“我”（是否要移除之前关于“我”的记录）
	* 想要实现该功能，该UI必须被ESC管理起来：IsControlledByEsc返回为true
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract", meta = (EditCondition = "bIsControlledByEsc"))
		bool bIsClearHistoricalRecords = false;

	//是否在Show时独立显示
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract", meta = (EditCondition = "bIsControlledByEsc"))
		bool bIsSeparatelyShow = false;
};
