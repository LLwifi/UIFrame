// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameplayTagContainer.h>
#include "RedPointTree.generated.h"

class UUserWidget;
class APawn;

//红点的事件改变类型
UENUM(BlueprintType)
enum class ERedPointNodeEventType :uint8
{
	None = 0 UMETA(DisplayName = "无"),
	Show UMETA(DisplayName = "显示"),
	Hide UMETA(DisplayName = "隐藏"), 
	Change UMETA(DisplayName = "改变")
};

//红点自定义参数
USTRUCT(BlueprintType)
struct FRedPointCustomParameter
{
	GENERATED_BODY()
public:
	FRedPointCustomParameter() {}
	FRedPointCustomParameter(UObject* ObjectValue) { CustomObject =ObjectValue; }
	FRedPointCustomParameter(FString StringValue) { CustomString.Add(StringValue); }
	FRedPointCustomParameter(TArray<FString> StringArrayValue) { CustomString = StringArrayValue; }
	FRedPointCustomParameter(FText TextValue) { CustomText.Add(TextValue); }
	FRedPointCustomParameter(TArray<FText> TextArrayValue) { CustomText = TextArrayValue; }
	FRedPointCustomParameter(float FloatValue) { CustomFloat.Add(FloatValue); }
	FRedPointCustomParameter(TArray<float> FloatArrayValue) { CustomFloat = FloatArrayValue; }
	FRedPointCustomParameter(FGameplayTag TagValue) { CustomTag.AddTag(TagValue); }
	FRedPointCustomParameter(FGameplayTagContainer TagArrayValue) { CustomTag = TagArrayValue; }
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UObject* CustomObject;
	/*String同时可以承载结构体的传输
	* 通过ULL_JsonDataManager的StructToJsonString和JsonStringToStruct函数
	* 可以将结构体通过字符传输在通过字符解析
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FString> CustomString;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FText> CustomText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<float> CustomFloat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayTagContainer CustomTag;
};

//红点接口的消息参数
USTRUCT(BlueprintType)
struct FRedPointMessage
{
	GENERATED_BODY()

public:
	FRedPointMessage() {};
	FRedPointMessage(URedPointNode* TriggerChildNode, URedPointNode* TriggerLeafNode, int32 DisplayNum)
	{
		ChildNode = TriggerChildNode;
		LeafNode = TriggerLeafNode;
		DisplayCount = DisplayNum;
	}

public:
	//触发该消息的子节点
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		URedPointNode* ChildNode = nullptr;
	//触发该消息的叶子节点
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		URedPointNode* LeafNode = nullptr;
	//当前红点链上显示的数量--可以通过Node的RedPointChainNodes.Num()函数获取
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 DisplayCount = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRedPointNodeEvent, FRedPointMessage, MessageInfo, ERedPointNodeEventType, Type);//红点的显示隐藏委托

//节点类型
UENUM(BlueprintType)
enum class ERedPointTreeNodeType :uint8
{
	None = 0 UMETA(DisplayName = "无"),
	Root UMETA(DisplayName = "根节点"),
	Mid UMETA(DisplayName = "中间节点"),
	Leaf UMETA(DisplayName = "叶子节点")
};

//红点节点的信息
USTRUCT(BlueprintType)
struct FRedPointNodeInfo
{
	GENERATED_BODY()

public:
	bool IsBindUI(TSubclassOf<UUserWidget> Class, FName Tag);

public:
	//节点类型
	UPROPERTY(/*VisibleAnywhere*/)
		ERedPointTreeNodeType RedPointTreeNodeType;
	//节点唯一标识
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
		FName NodeTag;
	//出问题时可以显示这些变量查找问题，平常不显示
	//父节点标识
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
		TArray<FName> ParentNodeTag;
	//子节点标识
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
		TArray<FName> ChildNodeTag;
	//对应的UI类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UUserWidget> UIClass;
	//识别UI的tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		FName UITag;
	//对应的节点类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
		TSubclassOf<URedPointNode> NodeClass;

	//自定义参数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	FRedPointCustomParameter CustomParameter;
};

/**
 * 红点树中的一个节点
 */
UCLASS(Blueprintable,BlueprintType)
class UIFRAME_API URedPointNode : public UObject
{
	GENERATED_BODY()

public:
	URedPointNode();

	virtual UWorld* GetWorld() const override;

	//判断UI是否是该节点对应的UI
	UFUNCTION(BlueprintPure)
		bool IsLinkUI(TSubclassOf<UUserWidget> UIClass, FName UITag);

	//红点的显示函数，该函数会调用对应UI的红点显示函数并且传达给父类
	UFUNCTION(BlueprintPure)
		APawn* GetPlayerPawn_RedPointNode();

	//获取当前红点链中显示的次数
	UFUNCTION(BlueprintPure)
		int32 GetDisplayCount();

	/*显示或隐藏该节点，这个操作会不断往父节点上传递
	* IsShow:是否是显示
	*/
	UFUNCTION(BlueprintCallable)
		void ShowOrHide(bool IsShow = true);

	/*红点的显示函数，该函数会调用对应UI的红点显示函数并且传达给父类
	*/
	UFUNCTION()
		void RedPointNodeShow(FRedPointMessage MessageInfo);

	/*红点的隐藏函数，该函数会调用对应UI的红点隐藏函数并且传达给父类
	*/
	UFUNCTION()
		void RedPointNodeHide(FRedPointMessage MessageInfo);

	/*红点的改变函数，当一个红点在显示后又有子节点的数量改变时会调用该函数
	*/
	UFUNCTION()
		void RedPointNodeChange(FRedPointMessage MessageInfo);

	UFUNCTION()
		UUI_SubSystem* GetUISubSystem();

	//获取自定义参数
	UFUNCTION(BlueprintPure)
		FRedPointCustomParameter GetCustomParameter();

	//获取自定义Float参数
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = "1"))
		bool GetCustomParameter_Float(float& Value,int32 Index = 0);

	//获取自定义String参数
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = "1"))
		bool GetCustomParameter_String(FString& Value, int32 Index = 0);

	//获取自定义Text参数
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = "1"))
		bool GetCustomParameter_Text(FText& Value, int32 Index = 0);

	//获取自定义GameplayTag参数
	UFUNCTION(BlueprintPure)
		FGameplayTagContainer GetCustomParameter_TagContainer();

public:
	/*初始化，在节点创建完后立刻调用*/
	UFUNCTION(BlueprintNativeEvent)
		void Init();
	virtual void Init_Implementation();

public:

	UPROPERTY(BlueprintReadOnly)
		UUI_SubSystem* UISubSystem;

	UPROPERTY(BlueprintReadOnly)
		FRedPointNodeInfo RedPointNodeInfo;

	UPROPERTY(BlueprintReadOnly)
		FRedPointNodeEvent RedPointNodeEvent;

	//父节点
	UPROPERTY(BlueprintReadWrite)
		TArray<URedPointNode*> ParentNodes;

	//子节点
	UPROPERTY(BlueprintReadWrite)
		TArray<URedPointNode*> ChildNodes;

	//红点链节点-保存由哪些叶子节点点亮的红点链
	UPROPERTY(BlueprintReadWrite)
		TArray<URedPointNode*> RedPointChainNodes;

	//当前红点是否开启
	UPROPERTY(BlueprintReadOnly)
		bool bIsShow = false;
};

/**
 * 红点树
 */
UCLASS(Blueprintable,BlueprintType)
class UIFRAME_API URedPointTree : public UObject
{
	GENERATED_BODY()
public:

	//红点面板的显示检查
	UFUNCTION()
		void RedPointPanelShowCheck(TSubclassOf<UUserWidget> UIClass, FName UITag);

	UFUNCTION()
		void Init(UObject* Outer);

	UFUNCTION(BlueprintPure)
		URedPointNode* GetRootNode();

	UFUNCTION(BlueprintPure)
		URedPointNode* GetNodeFromName(FName NodeName);

	UFUNCTION()
		UUI_SubSystem* GetUISubSystem();
public:
	UPROPERTY(BlueprintReadOnly)
		UUI_SubSystem* UISubSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName RedPointTreeTag;

	//全部节点----运行后在Init创建并赋值
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TMap<FName, URedPointNode*> AllNode;

	//全部叶子节点----运行后在Init创建并赋值
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TMap<FName, URedPointNode*> AllLeafNode;

	//全部绑定了UI的节点----运行后在Init创建并赋值
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TMap<FName, URedPointNode*> AllBindUINode;

	//全部节点信息----编辑器模式下操作的内容，运行后Init通过该数据进行红点树的构建
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TMap<FName,FRedPointNodeInfo> RedPointNodeInfo;

	//外部上下文
	UPROPERTY(VisibleAnywhere)
		UObject* RedPointTreeOuter;

	UPROPERTY(VisibleAnywhere)
		class UEdGraph* GraphView;
};

UINTERFACE(MinimalAPI)
class UUI_RedPointInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 红点系统相关接口
 */
class UIFRAME_API IUI_RedPointInteract
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*
	* 红点改变：除了在ShowRedPoint和HideRedPoint该函数会被调用外，当红点树的显示数量发生更改时该函数也会调用
	*/
	UFUNCTION(BlueprintNativeEvent)
		void LinkRedPointNodeChange(FRedPointMessage MessageInfo, ERedPointNodeEventType EventType);
	virtual void LinkRedPointNodeChange_Implementation(FRedPointMessage MessageInfo, ERedPointNodeEventType EventType);
};
