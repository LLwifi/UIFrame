// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WorkflowOrientedApp/WorkflowCentricApplication.h>
#include <Misc/NotifyHook.h>
#include <EditorUndoClient.h>
#include <WorkflowOrientedApp/WorkflowTabManager.h>

class URedPointTree;

/*编辑器类
-----编辑器类额外添加了一个DocumentManager变量，DocumentManager可以替换TabManager可以用于管理所有的图表页签，同蓝图编辑器类似。
-----FWorkflowCentricApplication 是一个继承自 FAssetEditorToolkit 的类，
它是一个基于工作流的应用程序，特点是可以支持多种模式（FApplicationMode）。
FWorkflowCentricApplication 是一些编辑器类的基类，例如 IBlueprintEditor。
FWorkflowCentricApplication 的使用方式与 FAssetEditorToolkit 基本相似，但需要在初始化时添加和设置模式。
*/
class UIFRAMEEDITOR_API FRedPointTreeEditor : public FAssetEditorToolkit //: public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient
{
public:
	FRedPointTreeEditor();
	virtual ~FRedPointTreeEditor();
	//初始化编辑器函数 这个函数并不是虚函数，也不含有模式匹配，为公开函数被外部调用
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

	//自定义的Tab
	static const FName CustomTabID;
	TSharedRef<SDockTab> SpawnTab_CustomTab(const FSpawnTabArgs& Args);

	// 生成细节面板
	TSharedRef<SDockTab> SpawnDetailTab(const FSpawnTabArgs& SpawnTabArgs) const;

	//节点
	static const FName GraphEditorTabName;
	static const FName PropertyEditorTabName;
	UEdGraphNode* CreateDebugNode(UEdGraph* ParentGraph, const FVector2D NodeLocation) const;

	void DeleteSelectedNode();

	//按下Node时显示在细节面板显示其属性
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);

private:
	TWeakObjectPtr<URedPointTree> RedPointTree;
	TSharedPtr<FDocumentTracker> DocumentManager;
	//编辑的那个图表 由外部传达
	UEdGraph* RedPointTreeGraph;
	TSharedPtr<SGraphEditor> GraphEditor;
	mutable TSharedPtr<IDetailsView> AssetPropertyView;

	TSharedPtr<class FUICommandList> AdvancedDialogueCommandList;
};

	
