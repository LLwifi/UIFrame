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
	UFUNCTION()
		void Init();

	//改变AllCheckBox中某个目标的状态
	void ChangeCheckState(TPair<UCheckBox*, bool>& OnceCheckBox, bool IsCheck);

	//单选更新
	UFUNCTION()
		void SingleUpdate();

	//多选更新
	UFUNCTION()
		void MultiUpdate();

	//空选检查
	UFUNCTION()
		void IsNoneCheck();

	//CheckBox回调
	UFUNCTION()
		void CheckBoxChange(bool bIsChecked);

	//更新选择下标
	UFUNCTION()
		void CurCheckIndexUpdate();

public:
	UPROPERTY()
		bool bIsInit = false;
	//是否允许多选
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMultiCheck = false;

	//最大多选数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsMultiCheck", EditConditionHides))
		int32 MaxCheckNum = 1;

	//是否允许一个都不选择
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsNoneCheck = false;

	//是否自动寻找CheckBox（只会寻找该面板下的CheckBox）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAutoFindCheckBox = true;

	//TMap<UI,初始是否开启>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<UCheckBox*, bool> AllCheckBox;

	//初始开启CheckBox的下标  -1表示多选
	UPROPERTY(VisibleAnywhere)
		TArray<int32> CurCheckIndex;
	//上次选择的下标----该值在运行后才开始生效
	UPROPERTY()
		int32 LastCheckIndex = 0;

	UPROPERTY(BlueprintAssignable)
		FOnCheckBoxsStateChanged CheckBoxsStateChanged;
};



