#include "ToolPropertiesWidget.h"
#include "Blueprint/WidgetTree.h"
#include "BuildToolSystem/Components/BuildToolComponent.h"
#include "BuildToolSystem/Data/BuildTool.h"
#include "CoreGameUI/Widgets/PropertyWidgets.h"
#include "BuildToolSystem/BuildToolSystem.h"

void UToolPropertiesEditorWidget::InitializeTool(UBuildTool* newTool) {
	ensureAlways(newTool);
	Tool = newTool;

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);
	for (UWidget* widget : widgets) {
		if (widget->Implements<UPropertyObjectEditor>())
			IPropertyObjectEditor::Execute_SetObject(widget, Tool);
	}
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
		else UE_LOG(LogToolSystem, Warning, TEXT("UToolPropertiesWidget: No BuildToolComponent found on PlayerController"));
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UToolPropertiesWidget::NativeDestruct() {
	Super::NativeDestruct();

	if (IsValid(ToolComponent)) ToolComponent->OnToolChanged.RemoveDynamic(this, &UToolPropertiesWidget::OnActiveToolChanged);
	ToolComponent = nullptr;
}
