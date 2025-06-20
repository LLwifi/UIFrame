// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <RedPointTree/RedPointTree.h>
#include "../UI_WidgetBase.h"
#include "UI_RedPointShowBase.generated.h"

class UUI_SubSystem;

/**
 * 红点显示基类
 * 该类可以自动去绑定一个红点树节点进行监听
 */
UCLASS()
class UIFRAME_API UUI_RedPointShowBase : public UUI_WidgetBase, public IUI_RedPointInteract
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override { return FText::FromString("UI_RedPointShow"); }
#endif

	virtual void NativeConstruct() override;

	virtual void LinkRedPointNodeChange_Implementation(FRedPointMessage MessageInfo, ERedPointNodeEventType EventType) override;

	UFUNCTION(BlueprintPure)
		UUI_SubSystem* GetUISubSystem();

	//移除与当前节点的绑定关系
	UFUNCTION(BlueprintCallable)
		void RemoveBindRedPointNodeEvent(FName NodeName);

	/*绑定红点事件回调
	NodeName：红点名称
	*/
	UFUNCTION(BlueprintCallable)
		void BindRedPointEventBack(FName NodeName);


public:
	UPROPERTY(BlueprintReadOnly)
		UUI_SubSystem* UISubSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NodeTag;

	UPROPERTY(BlueprintReadOnly)
		URedPointNode* Node;
};
