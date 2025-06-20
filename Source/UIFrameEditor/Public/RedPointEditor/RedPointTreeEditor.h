// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WorkflowOrientedApp/WorkflowCentricApplication.h>
#include <Misc/NotifyHook.h>
#include <EditorUndoClient.h>
#include <WorkflowOrientedApp/WorkflowTabManager.h>

class URedPointTree;

/*�༭����
-----�༭������������һ��DocumentManager������DocumentManager�����滻TabManager�������ڹ������е�ͼ��ҳǩ��ͬ��ͼ�༭�����ơ�
-----FWorkflowCentricApplication ��һ���̳��� FAssetEditorToolkit ���࣬
����һ�����ڹ�������Ӧ�ó����ص��ǿ���֧�ֶ���ģʽ��FApplicationMode����
FWorkflowCentricApplication ��һЩ�༭����Ļ��࣬���� IBlueprintEditor��
FWorkflowCentricApplication ��ʹ�÷�ʽ�� FAssetEditorToolkit �������ƣ�����Ҫ�ڳ�ʼ��ʱ��Ӻ�����ģʽ��
*/
class UIFRAMEEDITOR_API FRedPointTreeEditor : public FAssetEditorToolkit //: public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient
{
public:
	FRedPointTreeEditor();
	virtual ~FRedPointTreeEditor();
	//��ʼ���༭������ ��������������麯����Ҳ������ģʽƥ�䣬Ϊ�����������ⲿ����
	void InitRedPointTreeEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, URedPointTree* InRedPointTree);

public:
	//~ Begin FWorkflowCentricApplication Interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	//~ End FWorkflowCentricApplication Interface

	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End IToolkit Interface

	virtual void RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager);
	URedPointTree* GetRedPointTree() { return RedPointTree.Get(); }
	TSharedPtr<FDocumentTracker> GetDocumentManager() { return DocumentManager; }

	//�Զ����Tab
	static const FName CustomTabID;
	TSharedRef<SDockTab> SpawnTab_CustomTab(const FSpawnTabArgs& Args);

	// ����ϸ�����
	TSharedRef<SDockTab> SpawnDetailTab(const FSpawnTabArgs& SpawnTabArgs) const;

	//�ڵ�
	static const FName GraphEditorTabName;
	static const FName PropertyEditorTabName;
	UEdGraphNode* CreateDebugNode(UEdGraph* ParentGraph, const FVector2D NodeLocation) const;

	void DeleteSelectedNode();

	//����Nodeʱ��ʾ��ϸ�������ʾ������
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);

private:
	TWeakObjectPtr<URedPointTree> RedPointTree;
	TSharedPtr<FDocumentTracker> DocumentManager;
	//�༭���Ǹ�ͼ�� ���ⲿ����
	UEdGraph* RedPointTreeGraph;
	TSharedPtr<SGraphEditor> GraphEditor;
	mutable TSharedPtr<IDetailsView> AssetPropertyView;

	TSharedPtr<class FUICommandList> AdvancedDialogueCommandList;
};

	
