// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/UI_FunctionLibrary.h"

TArray<UWidget*> UUI_FunctionLibrary::GetAllWidgetFromClass(UPanelWidget* PanelWidget, TSubclassOf<UWidget> WidgetClass, bool IsContainUserWidget/* = false*/)
{
	return UUI_FunctionLibrary::GetAllWidgetFromClass<UWidget>(PanelWidget,WidgetClass,IsContainUserWidget);
}

FVector2D UUI_FunctionLibrary::BezierCurve_T_V2(float T, TArray<FVector2D> P0_3)
{
	FVector2D ReturnV;
	if (P0_3.Num() >= 4)
	{
		FVector2D v1 = pow((1 - T), 3) * P0_3[0];
		FVector2D v2 = pow((1 - T), 2) * 3 * P0_3[1] * T;
		FVector2D v3 = (1 - T) * pow(T, 2) * 3 * P0_3[2];
		FVector2D v4 = pow(T, 3) * P0_3[3];
		ReturnV = v1 + v2 + v3 + v4;
	}
	return ReturnV;
}

TArray<FVector2D> UUI_FunctionLibrary::BezierCurve_Num_V2(int32 Num, TArray<FVector2D> P0_3)
{
	TArray<FVector2D> ReturnV;
	if (P0_3.Num() >= 4)
	{
		for (int32 i = 0; i < Num; i++)
		{
			ReturnV.Add(UUI_FunctionLibrary::BezierCurve_T_V2(i/ Num, P0_3));
		}
	}

	return ReturnV;
}


FVector2D UUI_FunctionLibrary::GetWidgetPosFormPanel(UWidget* Widget, UWidget* InPanel)
{
	FVector2D Position;
	if (Widget && InPanel)
	{
		FGeometry Geometry = InPanel->GetCachedGeometry();
		Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->GetCachedGeometry().GetLocalSize() / 2.0f;
	}
	return Position;
}