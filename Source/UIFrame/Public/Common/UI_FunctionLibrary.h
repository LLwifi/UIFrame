// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Components/PanelWidget.h>
#include "UI_FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UIFRAME_API UUI_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//从某个节点开始获取往下的全部UI控件
	template< class T >
	UFUNCTION()
	static TArray<T*> GetAllWidgetFromClass(UPanelWidget* PanelWidget, TSubclassOf<T> WidgetClass, bool IsContainUserWidget = false)
	{
		TArray<T*> AllWidget;
		if (IsValid(PanelWidget) && IsValid(WidgetClass))
		{
			TArray<UWidget*> AllChildren = PanelWidget->GetAllChildren();
			for (int32 i = 0; i < AllChildren.Num(); i++)
			{
				//是否是带Slot的Widget
				UPanelWidget* Panel = Cast<UPanelWidget>(AllChildren[i]);
				if (!Panel && IsContainUserWidget)//UserWidget
				{
					UUserWidget* UserWidget = Cast<UUserWidget>(AllChildren[i]);
					if (UserWidget)
					{
						Panel = Cast<UPanelWidget>(UserWidget->GetRootWidget());
					}
				}
				if (IsValid(Panel))
				{
					TArray<T*> Widgets = GetAllWidgetFromClass<T>(Panel, WidgetClass, IsContainUserWidget);
					for (T* W : Widgets)
					{
						AllWidget.Add(W);
					}
				}
				//自己是否是该类型
				if (AllChildren[i]->IsA(WidgetClass))
				{
					AllWidget.Add(Cast<T>(AllChildren[i]));
				}
			}
			//Root是否是该类型
			if (PanelWidget->IsA(WidgetClass))
			{
				AllWidget.Add(Cast<T>(PanelWidget));
			}
		}
		return  AllWidget;
	}
	
	//从某个节点开始获取往下的全部UI控件
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "WidgetClass"))
		static TArray<UWidget*> GetAllWidgetFromClass(UPanelWidget* PanelWidget, TSubclassOf<UWidget> WidgetClass, bool IsContainUserWidget = false);
	
	//2D贝塞尔曲线——通过t（0~1）获得一个具体的点
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "WidgetClass"))
	static FVector2D BezierCurve_T_V2(float T, TArray<FVector2D> P0_3);

	//贝塞尔曲线——通过Num（点的数量）获得一个全部的点
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "WidgetClass"))
	static TArray<FVector2D> BezierCurve_Num_V2(int32 Num, TArray<FVector2D> P0_3);

	/*获取控件处于某个Panel下的的具体位置
	* Widget：哪个控件
	* InPanel：用于计算的Panel控件，在计算多个“同空间”下的控件位置时应该保证他们用于计算的Panel一致
	*/
	UFUNCTION(BlueprintPure)
	static FVector2D GetWidgetPosFormPanel(UWidget* Widget, UWidget* InPanel);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UUI_SubSystem* GetUISubSystem(UObject* WorldContextObject);

	/*通过Tip信息显示提示文本
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ShowTipFromTipInfo(UObject* WorldContextObject, FUI_TipInfo TipInfo, FName UITag = "None");

	/*通过ID显示提示文本
	* ID：通过该ID会查询【提示表】UUI_CommonGameConfig-》TipInfoDatatable获取提示文本
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ShowTipFromID(UObject* WorldContextObject, FName ID, FName UITag = "None");
};


