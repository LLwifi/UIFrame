// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorConfig/UI_CommonGameConfig.h"

UUI_CommonGameConfig* UUI_CommonGameConfig::GetInstance()
{
	return GetMutableDefault<UUI_CommonGameConfig>();
}
