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
	//�����������ĸ�����
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
	Formats.Add(FString(TEXT("uasset")));//ָ��һ���ļ���׺���ú�׺������ק�ļ�����UE�ʲ���FactoryCreateFile������
	bCreateNew = true;//CanCreateNew�����᷵�ظ�ֵ
	bEditAfterNew = true;/**��������ı༭��Ӧ���ڴ����¶����򿪣���Ϊtrue*/
	SupportedClass = URedPointTree::StaticClass();//֧�ֵ���
}

UObject* URedPointTreeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	//����ÿһ�δ��ʲ���ͼ���᲻ͬ
	URedPointTree* AssetsObject = NewObject<URedPointTree>(InParent, InClass, InName, Flags);
	UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(AssetsObject, NAME_None, UEdGraph::StaticClass(), URedPointTree::StaticClass());

	//Ĭ�Ͻڵ�
	FName RootName = "Root";
	URedPointTreeNode* DefaultGraphNode = NewObject<URedPointTreeNode>(Graph);
	if (AssetsObject->RedPointNodeInfo.Find(RootName))
	{
		DefaultGraphNode->RedPointNodeInfo = *AssetsObject->RedPointNodeInfo.Find(RootName);
	}
	else
	{
		DefaultGraphNode->RedPointNodeInfo.RedPointTreeNodeType = ERedPointTreeNodeType::Root;//���ڵ��޷���ɾ��
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


	////�Զ�����򴴽���
	//URedPointTree* RedPointTree = NewObject<URedPointTree>(InParent, InClass, InName, Flags);

	//bOutOperationCanceled = false;

	//return RedPointTree;
}
