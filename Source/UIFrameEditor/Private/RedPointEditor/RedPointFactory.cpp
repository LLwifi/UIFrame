// Fill out your copyright notice in the Description page of Project Settings.


#include "RedPointEditor/RedPointFactory.h"
#include <RedPointTree/RedPointTree.h>
#include "../../Public/RedPointEditor/RedPointTreeEditor.h"
#include <Misc/FileHelper.h>
#include "Kismet2/BlueprintEditorUtils.h"
#include "../../Public/RedPointEditor/RedPointTreeNode.h"

UClass* FAssetTypeAction_RedPointTree::GetSupportedClass() const
{
	return URedPointTree::StaticClass();
}


uint32 FAssetTypeAction_RedPointTree::GetCategories()
{
	//操作出现在哪个分类
	return EAssetTypeCategories::UI;
}

void FAssetTypeAction_RedPointTree::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	MenuBuilder.AddMenuEntry(
		FText::FromString("Custom Action"),
		FText::FromString("This is a custom action"),
		FSlateIcon(),
		FUIAction()
	);
}

bool FAssetTypeAction_RedPointTree::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FAssetTypeAction_RedPointTree::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	const EToolkitMode::Type ToolKitModeType = EditWithinLevelEditor ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjectIterator = InObjects.CreateConstIterator(); ObjectIterator; ++ObjectIterator)
	{
		if (URedPointTree* OurAsset = Cast<URedPointTree>(*ObjectIterator))
		{
			const TSharedRef<FRedPointTreeEditor> RecoilAssetEditorToolKit = MakeShareable(new FRedPointTreeEditor());
			RecoilAssetEditorToolKit->InitRedPointTreeEditor(ToolKitModeType, EditWithinLevelEditor, OurAsset);
		}
	}
}

URedPointTreeFactory::URedPointTreeFactory()
{
	Formats.Add(FString(TEXT("uasset")));//指定一个文件后缀，该后缀用于拖拽文件创建UE资产（FactoryCreateFile函数）
	bCreateNew = true;//CanCreateNew函数会返回该值
	bEditAfterNew = true;/**如果关联的编辑器应该在创建新对象后打开，则为true*/
	SupportedClass = URedPointTree::StaticClass();//支持的类
}

UObject* URedPointTreeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	//避免每一次打开资产的图表都会不同
	URedPointTree* AssetsObject = NewObject<URedPointTree>(InParent, InClass, InName, Flags);
	UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(AssetsObject, NAME_None, UEdGraph::StaticClass(), URedPointTree::StaticClass());

	//默认节点
	FName RootName = "Root";
	URedPointTreeNode* DefaultGraphNode = NewObject<URedPointTreeNode>(Graph);
	if (AssetsObject->RedPointNodeInfo.Find(RootName))
	{
		DefaultGraphNode->RedPointNodeInfo = *AssetsObject->RedPointNodeInfo.Find(RootName);
	}
	else
	{
		DefaultGraphNode->RedPointNodeInfo.RedPointTreeNodeType = ERedPointTreeNodeType::Root;//根节点无法被删除
		DefaultGraphNode->TitleColor = FLinearColor::Yellow;
		DefaultGraphNode->RedPointNodeInfo.NodeTag = RootName;
	}
	Graph->Modify();
	DefaultGraphNode->SetFlags(RF_Transactional);
	DefaultGraphNode->Rename(nullptr, Graph, REN_NonTransactional);
	DefaultGraphNode->CreateNewGuid();
	DefaultGraphNode->NodePosX = 0;
	DefaultGraphNode->NodePosY = 0;
	DefaultGraphNode->AllocateDefaultPins();
	Graph->AddNode(DefaultGraphNode);

	AssetsObject->GraphView = Graph;
	return AssetsObject;

	//check(InClass->IsChildOf(URedPointTree::StaticClass()));
	//return NewObject<URedPointTree>(InParent, InClass, InName, Flags);
}

UObject* URedPointTreeFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	return Super::FactoryCreateFile(InClass,InParent,InName,Flags,Filename,Parms,Warn,bOutOperationCanceled);


	////自定义规则创建类
	//URedPointTree* RedPointTree = NewObject<URedPointTree>(InParent, InClass, InName, Flags);

	//bOutOperationCanceled = false;

	//return RedPointTree;
}
