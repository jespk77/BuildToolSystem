#include "ToolPropertiesWidget.h"
#include "../Components/BuildToolComponent.h"
#include "../Data/BuildTool.h"
#include "../BuildToolSystemLog.h"

void UToolPropertiesEditorWidget::InitializeTool(UBuildTool* newTool) {
	ensureAlways(newTool);
	Tool = newTool;
	IPropertyWidgetContainer::Execute_SetObjectOnPropertyWidgets(this, Tool);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UToolPropertiesWidget::OnActiveToolChanged(int32 index) {
	UUserWidget* toolWidget = nullptr;

	if (UBuildTool* tool = ToolComponent->GetActiveTool()) {
		if (IsValid(tool->ToolWidget)) {
			UToolPropertiesEditorWidget* widget = CreateWidget<UToolPropertiesEditorWidget>(this, tool->ToolWidget);
			widget->InitializeTool(tool);
			toolWidget = widget;
		}
	}

	if (IsValid(EditorWidget)) EditorWidget->MarkAsGarbage();
	EditorWidget = toolWidget;

	if (IsValid(EditorWidget)) {
		Container->SetContent(EditorWidget->TakeWidget());
		SetVisibility(ESlateVisibility::Visible);
	}
	else {
		Container->ClearContent();
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

TSharedRef<SWidget> UToolPropertiesWidget::RebuildWidget() {
	return SAssignNew(Container, SBorder);
}

void UToolPropertiesWidget::ReleaseSlateResources(bool releaseChildren) {
	Container.Reset();
	EditorWidget = nullptr;
	Super::ReleaseSlateResources(releaseChildren);
}

void UToolPropertiesWidget::NativePreConstruct() {
	Super::NativePreConstruct();

	if (!IsDesignTime()) {
		ToolComponent = GetOwningPlayer() ? GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>() : nullptr;
		if (IsValid(ToolComponent)) ToolComponent->OnToolChanged.AddDynamic(this, &UToolPropertiesWidget::OnActiveToolChanged);
		else TOOLSYSTEM_LOG(Warning, "No BuildToolComponent found on PlayerController");
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UToolPropertiesWidget::NativeDestruct() {
	Super::NativeDestruct();

	if (IsValid(ToolComponent)) ToolComponent->OnToolChanged.RemoveDynamic(this, &UToolPropertiesWidget::OnActiveToolChanged);
	ToolComponent = nullptr;
}
