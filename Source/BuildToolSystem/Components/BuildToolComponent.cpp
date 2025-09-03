#include "BuildToolComponent.h"
#include "BuildToolSystem/Data/BuildTool.h"
#include "BuildToolSystem/BuildToolSettings.h"

void UBuildToolComponent::CreateTools() {
	const TArray<FToolSettings>& toolTypes = GetDefault<UBuildToolSettings>()->Tools;
	UE_LOG(LogToolSystem, Log, TEXT("Found and creating custom %d tool classes..."), toolTypes.Num());

	Tools.Reset(toolTypes.Num());
	for (const FToolSettings& settings : toolTypes) {
		if (!settings.IsValid()) continue;

		UBuildTool* tool = NewObject<UBuildTool>(this, settings.ToolClass);
		if (!tool->ToolWidget) tool->ToolWidget = settings.ToolWidget;
		tool->InitializeTool(GetOwner<APlayerController>());
		UE_LOG(LogToolSystem, Log, TEXT("Created tool with name '%s'"), *tool->ToolName.ToString());
		Tools.Add(tool);
	}
}

UBuildToolComponent::UBuildToolComponent(const FObjectInitializer& initializer) : Super(initializer) { }

void UBuildToolComponent::BeginPlay() {
	Super::BeginPlay();
	CreateTools();
}

void UBuildToolComponent::TickComponent(float delta, ELevelTick tick, FActorComponentTickFunction* function) {
	Super::TickComponent(delta, tick, function);

	if (UBuildTool* tool = GetActiveTool()) tool->Tick(delta);
}

void UBuildToolComponent::SetActiveTool(int32 toolIndex) {
	int32 newIndex = Tools.IsValidIndex(toolIndex) ? toolIndex : INDEX_NONE;
	if (newIndex == ActiveToolIndex) newIndex = INDEX_NONE;

	if (UBuildTool* tool = GetActiveTool()) {
		UE_LOG(LogToolSystem, Log, TEXT("Deactivating tool '%s'"), *tool->ToolName.ToString());
		tool->OnEndTool();
	}

	ActiveToolIndex = newIndex;
	if (UBuildTool* tool = GetActiveTool()) {
		UE_LOG(LogToolSystem, Log, TEXT("Activating tool '%s'"), *tool->ToolName.ToString());
		tool->OnStartTool();
		if (tool->TickTime >= 0) SetComponentTickEnabled(true);
	}
	else UE_LOG(LogToolSystem, Log, TEXT("No tool active"));
	OnToolChanged.Broadcast(newIndex);
}

void UBuildToolComponent::SetActiveToolByName(const FString name) {
	for (int32 i = 0; i < Tools.Num(); i++) {
		if (Tools[i]->ToolName.ToString().Equals(name, ESearchCase::IgnoreCase)) {
			SetActiveTool(i);
			return;
		}
	}

	SetActiveTool(INDEX_NONE);
}

#define HandleEvent(ToolFunction, ...)\
	if(UBuildTool* tool = GetActiveTool()) tool->ToolFunction(__VA_ARGS__);

#define HandleEventWithReply(ToolFunction, ...)\
	bool result = false;\
	if(UBuildTool* tool = GetActiveTool()) result = tool->ToolFunction(__VA_ARGS__);\
	if(!result) result = Super::ToolFunction(__VA_ARGS__);\
	return result;

bool UBuildToolComponent::OnMouseDown(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(OnMouseDown, geometry, event);
}

bool UBuildToolComponent::OnMouseUp(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(OnMouseUp, geometry, event);
}

bool UBuildToolComponent::OnMouseDoubleDown(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(OnMouseDoubleDown, geometry, event);
}

bool UBuildToolComponent::OnMouseMove(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(OnMouseMove, geometry, event);
}

bool UBuildToolComponent::OnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(OnMouseWheel, geometry, event);
}

#undef HandleEvent
#undef HandleEventWithReply
