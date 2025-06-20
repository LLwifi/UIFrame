#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI_PrefabInteract.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI,NotBlueprintable)
class UUI_PrefabInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * UI控件预制相关接口
 */
class UIFRAME_API IUI_PrefabInteract
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
#if WITH_EDITOR
	//覆盖当前行的预制信息
	UFUNCTION()
		virtual void OperationEvent_Cover();
	//新增预制行信息
	UFUNCTION()
		virtual void OperationEvent_New(FName RowName);

#endif
};
