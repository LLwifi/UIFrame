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

