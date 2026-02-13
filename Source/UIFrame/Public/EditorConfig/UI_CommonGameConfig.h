// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include <Engine/DataTable.h>
#include "Common/UI_PanelInteract.h"
#include "UI_CommonGameConfig.generated.h"

UENUM(BlueprintType)
enum class ENOSScrollBoxStyle : uint8
{
	None = 0 UMETA(DisplayName = "无类型"),
	Common = 1 UMETA(DisplayName = "通用"),
};

USTRUCT(BlueprintType)
struct FUI_NameArray
{
	GENERATED_BODY()
public:
	//响应的UI类
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> NameArray;
};

/**
 * 编辑器下的通用UI框架配置
 */
UCLASS(config = UI_CommonGameConfig, defaultconfig)
class UIFRAME_API UUI_CommonGameConfig : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const final override
	{
		return FName("GameEditorConfig");
	}
	static UUI_CommonGameConfig* GetInstance();
	UFUNCTION(BlueprintPure, BlueprintCallable)
		static UUI_CommonGameConfig* GetUI_CommonGameConfig() { return GetInstance(); }

public:
	//预制按钮表格
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "PrefabDataTable")
		TSoftObjectPtr<UDataTable> PrefabButtonDatatable;
	//预制文本表格
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "PrefabDataTable")
		TSoftObjectPtr<UDataTable> PrefabTextDatatable;
	//预制CheckBox表格
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "PrefabDataTable")
		TSoftObjectPtr<UDataTable> PrefabCheckBoxDatatable;
	
	//默认音效
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Panel")
		TMap<EUIInterface, TSoftObjectPtr<USoundBase>> DefaultSound;

	//Esc面板
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Esc")
		TSoftClassPtr<UUserWidget> EscPanelClass;

	//Root面板
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
		TSoftClassPtr<UUserWidget> RootPanelClass;
		
	//作弊面板
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
		TSoftClassPtr<UUserWidget> CheatPanelClass;

	//调试面板
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
		TSoftClassPtr<UUserWidget> DebugPanelClass;

	//Tips面板
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Tip")
		TSoftClassPtr<UUserWidget> TipPanelClass;
	//Tip文本配置表
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Tip")
		TSoftObjectPtr<UDataTable> TipInfoDatatable;

	//ScrollBox的滚动条样式,Name是方案名称，Value是Style值
	UPROPERTY(Config,EditAnywhere,BlueprintReadOnly,Category = "ScrollBox")
	    TMap<ENOSScrollBoxStyle,FScrollBarStyle> ScrollBoxStyleMap;

	/*不同的按键UI映射配置表，表结构体类型为FUI_UIKeyBoardMapping
	* 通过该表可以实现按键开/关UI
	*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UIKeyBoard")
		TSoftObjectPtr<UDataTable> UIKeyBoardMappingDatatable;
	/*不同关卡下的默认UI映射配置<关卡,RowName数组>
	* 该配置通过UUI_RootPanel在构造时通知UISubSystem进行设置
	*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UIKeyBoard", meta = (AllowedClasses = "/Script/Engine.World"))
		TMap<FSoftObjectPath, FUI_NameArray> LevelUIKeyBoardMapping;
	//哪些UI在显示后需要屏蔽按键/屏蔽UI按键的UIclass
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UIKeyBoard")
		TArray<TSoftClassPtr<UUserWidget>> BlockUIKeyBoardUIClass;
};
