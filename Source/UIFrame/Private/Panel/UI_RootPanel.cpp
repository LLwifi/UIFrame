// Fill out your copyright notice in the Description page of Project Settings.


#include "Panel/UI_RootPanel.h"
#include <EditorConfig/UI_CommonGameConfig.h>
#include <Subsystems/SubsystemBlueprintLibrary.h>
#include "UI_SubSystem.h"

void UUI_RootPanel::NativeConstruct()
{
	Super::NativeConstruct();
	UWorld* World = GetWorld();
	if (World)
	{
		ULevel* CurrentLevel = World->GetCurrentLevel();
		FSoftObjectPath Map = FSoftObjectPath(CurrentLevel);
		FString MapPath,L,R;
		Map.GetLongPackageName().Split("/", &L, &R, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		MapPath = L + "/";
		Map.GetLongPackageName().Split("_", &L, &R,ESearchCase::IgnoreCase,ESearchDir::FromEnd);
		MapPath += R;
		Map = FSoftObjectPath(FTopLevelAssetPath(FName(MapPath), Map.GetAssetFName()));
		if (UUI_CommonGameConfig::GetInstance()->LevelUIKeyBoardMapping.Contains(Map))
		{
			UUI_SubSystem* UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UUI_SubSystem::StaticClass()));
			if (UISubSystem)
			{
				for (FName& RowName : UUI_CommonGameConfig::GetInstance()->LevelUIKeyBoardMapping[Map].NameArray)
				{
					UISubSystem->AppendUIKeyBoardMappingFromRowName(RowName);
				}
			}
		}
	}
}

void UUI_RootPanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_RootPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
