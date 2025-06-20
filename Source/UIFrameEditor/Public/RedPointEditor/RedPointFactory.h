// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include <AssetTypeActions_Base.h>
#include "RedPointFactory.generated.h"

class URedPointTree;
class URedPointTreeGraphEntryNode;

//红点资源
struct FAssetTypeAction_RedPointTree : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	// 返回资产的名字
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "FAssetTypeAction_RedPointTree", "RedPointTree"); }
	// 资产在Content Browser里的浏览颜色
	virtual FColor GetTypeColor() const override { return FColor(236, 41, 30); }
	// 该Action对应的类
	virtual UClass* GetSupportedClass() const override;
	//操作出现在哪个分类
	virtual uint32 GetCategories() override;

	// 编辑器图标
	//virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;

	//在资产上右键创建的菜单上添加一个操作
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	// 用于定义打开之后显示的编辑器
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

};

/**
 * 红点树的工厂类
决定了改资源创建的方法
 */
UCLASS()
class UIFRAMEEDITOR_API URedPointTreeFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	URedPointTreeFactory(); 
	//从编辑器界面鼠标右键菜单中创建
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	//外部拖动文件进行创建
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
};


