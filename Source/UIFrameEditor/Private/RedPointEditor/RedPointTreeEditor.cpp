#include "../../Public/RedPointEditor/RedPointTreeEditor.h"
#include <RedPointTree/RedPointTree.h>
#include "../../Public/RedPointEditor/RedPointTreeGraph.h"
#include "../../Public/RedPointEditor/RedPointTreeGraphSchema.h"
#include <WorkflowOrientedApp/WorkflowUObjectDocuments.h>
#include "../../Public/RedPointEditor/RedPointTreeNode.h"
#include <Templates/SharedPointer.h>
#include <Framework/Commands/UICommandList.h>
#include <Framework/Commands/GenericCommands.h>

#define LOCTEXT_NAMESPACE "RedPointTreeEditor"

const FName FRedPointTreeEditor::CustomTabID(TEXT("CustomAsset_Custom"));
const FName FRedPointTreeEditor::GraphEditorTabName(TEXT("OurAssetsGraphEditor"));
const FName FRedPointTreeEditor::PropertyEditorTabName(TEXT("OurAssetsPropertyEditor"));

UEdGraphNode* FRedPointTreeEditor::CreateDebugNode(UEdGraph* ParentGraph, const FVector2D NodeLocation) const
{
	check(ParentGraph != nullptr)
		UEdGraphNode* ResultGraphNode = NewObject<URedPointTreeNode>(ParentGraph);
	ParentGraph->Modify();
	ResultGraphNode->SetFlags(RF_Transactional);

	ResultGraphNode->Rename(nullptr, ParentGraph, REN_NonTransactional);
	ResultGraphNode->CreateNewGuid();
	ResultGraphNode->NodePosX = NodeLocation.X;
	ResultGraphNode->NodePosY = NodeLocation.Y;

	ResultGraphNode->AllocateDefaultPins();
	return ResultGraphNode;
}

void FRedPointTreeEditor::DeleteSelectedNode()
{
	if (!GraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	GraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIterator(SelectedNodes); NodeIterator; ++NodeIterator)
	{
		if (UEdGraphNode* GraphNodeToDelete = Cast<UEdGraphNode>(*NodeIterator))
		{
			if (GraphNodeToDelete->CanUserDeleteNode())
			{
				GraphNodeToDelete->Modify();
				GraphNodeToDelete->DestroyNode();
			}
		}
	}
}

void FRedPointTreeEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}
	if (Selection.Num() == 0)
	{
		AssetPropertyView->SetObject(GetEditingObject());
	}
	else
	{
		AssetPropertyView->SetObjects(Selection);
	}
	
	//�޸������ı���ֵ
	RedPointTree->RedPointNodeInfo.Empty();
	for (UEdGraphNode* Node : RedPointTreeGraph->Nodes)
	{
		URedPointTreeNode* RedPointTreeNode = Cast<URedPointTreeNode>(Node);
		if (RedPointTreeNode && RedPointTreeNode->IsLink())
		{
			RedPointTreeNode->RedPointTree = RedPointTree.Get();

			RedPointTree->RedPointNodeInfo.Add(RedPointTreeNode->RedPointNodeInfo.NodeTag,RedPointTreeNode->RedPointNodeInfo);
		}
	}
}

void FRedPointTreeEditor::OnNodeDoubleClicked(UEdGraphNode* Node)
{

}

FRedPointTreeEditor::FRedPointTreeEditor()
{

}

FRedPointTreeEditor::~FRedPointTreeEditor()
{

}

void FRedPointTreeEditor::InitRedPointTreeEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, URedPointTree* InRedPointTree)
{
	const TSharedRef<FTabManager::FLayout> StandaloneOurAssetEditor = FTabManager::NewLayout("OutAssetEditor")->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(EOrientation::Orient_Horizontal)
		->Split(FTabManager::NewStack()->AddTab(FName("OutAssetsGraphEditorTab"), ETabState::OpenedTab))
		->Split(FTabManager::NewStack()->AddTab(FName("OurAssetPropertyTab"), ETabState::OpenedTab))
	);
	RedPointTree = InRedPointTree;
	RedPointTreeGraph = InRedPointTree->GraphView;
	RedPointTreeGraph->Schema = URedPointTreeGraphSchema::StaticClass();
	InitAssetEditor(Mode, InitToolkitHost, FName("OurAssetEditor"), StandaloneOurAssetEditor, true, true, InRedPointTree);
	RegenerateMenusAndToolbars();
}

void FRedPointTreeEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	InTabManager->RegisterTabSpawner(FName("OurAssetPropertyTab"), FOnSpawnTab::CreateRaw(this, &FRedPointTreeEditor::SpawnDetailTab));

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FRedPointTreeEditor::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FRedPointTreeEditor::OnNodeDoubleClicked);

	//CreateCommandList
	AdvancedDialogueCommandList = MakeShareable(new FUICommandList);
	AdvancedDialogueCommandList->MapAction(FGenericCommands::Get().Delete, FExecuteAction::CreateRaw(this, &FRedPointTreeEditor::DeleteSelectedNode));

	GraphEditor = SNew(SGraphEditor)
		.GraphToEdit(RedPointTreeGraph)
		.GraphEvents(InEvents)//����Nodeʱ��ʾ��ϸ�������ʾ������
		.AdditionalCommands(AdvancedDialogueCommandList)
		;


	InTabManager->RegisterTabSpawner(FName("OutAssetsGraphEditorTab"), FOnSpawnTab::CreateLambda([&](const FSpawnTabArgs& SpawnTabArgs)
		{
			return SNew(SDockTab)
				[
					GraphEditor.ToSharedRef()
				];
		}));
}


void FRedPointTreeEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(FName("OurAssetPropertyTab"));
	InTabManager->UnregisterTabSpawner(FName("OutAssetsGraphEditorTab"));
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

FName FRedPointTreeEditor::GetToolkitFName() const
{
	return FName("RedPointTreeEditor");
}

FText FRedPointTreeEditor::GetBaseToolkitName() const
{
	return LOCTEXT("RedPointTreeEditor", "State Machine Editor");
}

FString FRedPointTreeEditor::GetWorldCentricTabPrefix() const
{
	return FString();
}

FLinearColor FRedPointTreeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

void FRedPointTreeEditor::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager)
{
	//���ø������������ͻᴴ��Ĭ�ϵ�ToolbarTab
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FRedPointTreeEditor::SpawnTab_CustomTab(const FSpawnTabArgs& Args)
{
	return
		SNew(SDockTab)
		[
			//Add A New S Element
			SNew(SButton)
			.Text(FText::FromString("Custom Tab!"))
		];
}

TSharedRef<SDockTab> FRedPointTreeEditor::SpawnDetailTab(const FSpawnTabArgs& SpawnTabArgs) const
{
	//�������Ա༭��ģ��
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs;
	//�������Ա༭����Slate
	AssetPropertyView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);//TSharedPtr<IDetailsView>
	//�������룬���������Զ����ɶ�����������
	AssetPropertyView->SetObject(GetEditingObject());
	return SNew(SDockTab)
		[
			AssetPropertyView.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE