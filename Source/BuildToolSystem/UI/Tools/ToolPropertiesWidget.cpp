#include "ToolPropertiesWidget.h"

#include "BuildToolSystem/Components/BuildToolComponent.h"
#include "BuildToolSystem/Data/BuildTool.h"
#include "ToolWidgetInterface.h"

void UToolPropertiesWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	ToolComponent = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	if (IsValid(ToolComponent)) ToolComponent->OnToolChanged.AddDynamic(this, &UToolPropertiesWidgetBase::OnActiveToolChanged);
	else UE_LOG(LogTemp, Warning, TEXT("ToolPropertiesWidgetBase: No ToolComponent found on owned player"));
}

void UToolPropertiesWidgetBase::NativeDestruct() {
	Super::NativeDestruct();
	if (IsValid(ToolComponent)) ToolComponent->OnToolChanged.RemoveDynamic(this, &UToolPropertiesWidgetBase::OnActiveToolChanged);
	ToolComponent = nullptr;
}

void UToolPropertiesWidgetBase::OnActiveToolChanged_Implementation(int32 index) {
	UBuildTool* tool = ToolComponent->GetActiveTool();
	UUserWidget* newWidget;

	if (IsValid(tool) && tool->ToolWidget) newWidget = CreateWidget(this, tool->ToolWidget);
	else newWidget = nullptr;

	if (ActiveWidget) {
		if (ActiveWidget->Implements<UToolWidgetInterface>()) IToolWidgetInterface::Execute_OnToolWidgetDeactivated(ActiveWidget);
		ActiveWidget->Destruct();
	}

	ActiveWidget = newWidget;
	if (ActiveWidget && ActiveWidget->Implements<UToolWidgetInterface>()) IToolWidgetInterface::Execute_OnToolWidgetActivated(ActiveWidget, tool);
	SetPropertyWidget(ActiveWidget);
}
