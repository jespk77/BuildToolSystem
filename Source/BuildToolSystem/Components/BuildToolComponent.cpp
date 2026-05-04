#include "../Data/BuildTool.h"
#include "../Data/ToolData.h"
#include "../BuildToolSystemLog.h"

void UBuildToolComponent::CreateTools() {
	TArray<FToolSettings> toolTypes;
	if (ToolData) ToolData->GetTools(toolTypes);
	TOOLSYSTEM_LOG(Verbose, "Found and creating custom %d tool classes...", toolTypes.Num());

	Tools.Reset(toolTypes.Num());
	for (const FToolSettings& settings : toolTypes) {
		if (!settings.IsValid()) continue;

		UBuildTool* tool = NewObject<UBuildTool>(this, settings.ToolClass);
		if (!tool->ToolWidget) tool->ToolWidget = settings.ToolWidget;
		tool->InitializeTool(GetOwner<APlayerController>());
		TOOLSYSTEM_LOG(Verbose, "Created tool with name '%s'", *tool->ToolName.ToString());
		Tools.Add(tool);
	}

	OnToolsInitialized.Broadcast();
}

void UBuildToolComponent::ProcessSelectionChanged() {
	Super::ProcessSelectionChanged();

	if (UBuildTool* tool = GetActiveTool()) {
		const bool supportSelection = tool->OnSelectionChanged(Selection);
		if (supportSelection) tool->OnSelectionUpdated.Broadcast();
		else if (Selection) {
			TOOLSYSTEM_LOG(Verbose, "Active tool does not support editing current selection, deactivating...");
			SetActiveTool(INDEX_NONE);
		}
	}
}

void UBuildToolComponent::ProcessToolChanged() {
	OnToolChanged.Broadcast(ActiveToolIndex);
	if (UBuildTool* tool = GetActiveTool()) {
		const bool supportSelection = tool->OnSelectionChanged(Selection);
		if (!supportSelection) {
			TOOLSYSTEM_LOG(Verbose, "Active tool does not support editing current selection, deselecting everything...");
			SetSelection(nullptr);
		}
		// fire the update event from here? otherwise the each tool would have to fire the event itself as you don't know what it's gonna return beforehand
		else tool->OnSelectionUpdated.Broadcast();
	}
}

void UBuildToolComponent::BeginPlay() {
	Super::BeginPlay();
	CreateTools();
}

void UBuildToolComponent::TickComponent(float delta, ELevelTick tick, FActorComponentTickFunction* function) {
	Super::TickComponent(delta, tick, function);

	if (UBuildTool* tool = GetActiveTool()) tool->Tick(delta);
}

const FRaycastParameters& UBuildToolComponent::GetActiveRaycastParameters() const {
	if (const UBuildTool* tool = GetActiveTool())
		if (tool->UseRaycastParametersForSelection) return tool->RaycastParameters;
	return Super::GetActiveRaycastParameters();
}

void UBuildToolComponent::SetActiveTool(int32 toolIndex) {
	int32 newIndex = Tools.IsValidIndex(toolIndex) ? toolIndex : INDEX_NONE;
	if (newIndex == ActiveToolIndex) newIndex = INDEX_NONE;

	if (UBuildTool* tool = GetActiveTool()) {
		TOOLSYSTEM_LOG(Verbose, "Deactivating tool '%s'", *tool->ToolName.ToString());
		tool->OnEndTool();
	}

	ActiveToolIndex = newIndex;
	if (UBuildTool* tool = GetActiveTool()) {
		TOOLSYSTEM_LOG(Verbose, "Activating tool '%s'", *tool->ToolName.ToString());
		tool->OnStartTool();
		if (tool->TickTime >= 0) SetComponentTickEnabled(true);
	}
	else TOOLSYSTEM_LOG(Verbose, "No tool active");
	ProcessToolChanged();
}

void UBuildToolComponent::SetActiveToolByName(const FString name) {
	for (int32 i = 0; i < Tools.Num(); i++) {
		if (Tools[i]->ToolName.ToString().Equals(name, ESearchCase::IgnoreCase)) {
			SetActiveTool(i);
			return;
		}
	}

	TOOLSYSTEM_LOG(Verbose, "No tool found with name '%s'", *name);
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
