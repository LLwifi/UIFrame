#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "UI_StructAndEnum.generated.h"

/*
* UI控件的预制管理
* 读：通过预制好的信息生成外表
* 写：将当前控件的信息储存到预制表
*/
USTRUCT(BlueprintType)
struct FUI_PrefabOperation
{
	GENERATED_BODY()
public:

};

USTRUCT(BlueprintType)
struct FUI_Prefab
{
	GENERATED_BODY()
public:
	//该变量由控件在构造中赋值 之后在细节面板中使用该变量调用控件函数
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Prefab")
		UObject* Widget;

	//要使用的预制行
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prefab")
		FDataTableRowHandle UseRow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prefab")
		FUI_PrefabOperation Operation;
};

/*提示信息结构
*/
USTRUCT(BlueprintType)
struct FUI_TipInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		FUI_TipInfo* Info = InDataTable->FindRow<FUI_TipInfo>(InRowName, TEXT(""));
		if (Info && InRowName.ToString().IsNumeric())
		{
			Info->ID = InRowName;
		}
	}
public:
	/*提示文本的唯一标识
	* 该值无法被设置，自动读取行名称设置
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ID;
	//提示文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TipText; 
	//显示时长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisplayTime = 3.0f;
	/*提示音效
	* 在出现提示内容时播放（通常是2D音效）
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> TipSound;
};
