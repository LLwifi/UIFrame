// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "UI_CheckBoxCanvas.generated.h"

class UCheckBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckBoxsStateChanged, const TArray<int32>&, CheckBoxsIndex);

/**
 * 管理自身下的CheckBox们
 */
UCLASS()
class UIFRAME_API UUI_CheckBoxCanvas : public UCanvasPanel
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override;
#endif
	virtual void SynchronizeProperties() override;

	virtual TSharedRef<SWidget> RebuildWidget() override;
public:
	UFUNCTION(BlueprintCallable)
		void Init();

	UFUNCTION()
		void AutoFindCheckBox();

	//改变AllCheckBox中某个目标的状态
	void ChangeCheckState(TPair<UCheckBox*, bool>& OnceCheckBox, bool IsCheck);

	/*CheckBox的边界检查（带有还原操作）
	* Return : 是否处于边界中
	* CurCheckNum：当前勾选的CheckBox数量
	*/
	UFUNCTION(BlueprintPure)
		bool CheckBoxRangeCheck(int32 CurCheckNum);

	//某个CheckBox的回调
	UFUNCTION()
		void SomeCheckBoxChange(bool bIsChecked);

	//是否允许多选
	UFUNCTION(BlueprintPure)
		bool IsMultiCheck();

	//设置是否允许为空（一个都不选）
	UFUNCTION(BlueprintCallable)
		bool SetIsNoneCheck(bool IsNoneCheck);

	//设置最大勾选数量
	UFUNCTION(BlueprintCallable)
		void SetMaxCheckNum(int32 Num);

public:
	/*最大选择数量
	* 该值范围为1~CheckBox的数量
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCheckNum = 1;

	//是否允许一个都不选择
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsNoneCheck = false;

	//TMap<CheckUI,初始是否开启>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<UCheckBox*, bool> AllCheckBox;

	//按顺序勾选的下标
	UPROPERTY(VisibleAnywhere)
		TArray<UCheckBox*> CurSequenceCheckBox;

	UPROPERTY(BlueprintAssignable)
		FOnCheckBoxsStateChanged CheckBoxsStateChanged;
};



