#include "ToolSelectionWidget.h"
#include "CoreGameUI/Elements/ToggleableButton.h"
#include "BuildToolSystem/Components/BuildToolComponent.h"
#include "BuildToolSystem/Data/BuildTool.h"

void UToolSelectionWidget::OnToolsInitialized() {
	if (IsValid(ToolComponent)) {
		for (auto it = ToolComponent->GetToolIterator(); it; ++it)
			AddToolButton((*it)->ToolName);
		ToolComponent->OnToolChanged.AddDynamic(this, &UToolSelectionWidget::OnActiveToolChanged);
	}
}

void UToolSelectionWidget::OnActiveToolChanged(int32 index) {
	for (int32 i = 0; i < Buttons.Num(); i++)
		Buttons[i]->SetSelected(i == index);
}

void UToolSelectionWidget::AddToolButton(const FName& toolName) {
	TSharedPtr<SToggleableButton>& button = Buttons.AddZeroed_GetRef();
	Container->AddSlot().Padding(5.f).AutoWidth()[
		SAssignNew(button, SToggleableButton).ButtonStyle(&ButtonStyle).ContentPadding(10.f)
			.Text(FText::FromName(toolName)).TextStyle(&ButtonTextStyle)
			.OnClicked_Lambda([&]() {
			if (IsValid(ToolComponent)) {
				UBuildTool* tool = ToolComponent->GetActiveTool();
				if (tool && tool->ToolName.IsEqual(toolName)) ToolComponent->SetActiveTool(INDEX_NONE);
				else ToolComponent->SetActiveToolByName(toolName.ToString());
			}
			return FReply::Handled();
		})
	];
}

TSharedRef<SWidget> UToolSelectionWidget::RebuildWidget() {
	return SAssignNew(Container, SHorizontalBox);
}

void UToolSelectionWidget::ReleaseSlateResources(bool releaseChildren) {
	Container.Reset();
	Buttons.Reset();
	Super::ReleaseSlateResources(releaseChildren);
}

void UToolSelectionWidget::NativePreConstruct() {
	Super::NativePreConstruct();

	// make sure the widget is clean before adding new buttons
	for (const TSharedPtr<SToggleableButton>& button : Buttons) Container->RemoveSlot(button.ToSharedRef());
	Buttons.Reset();

	if (IsDesignTime()) {
		AddToolButton("Tool");
		AddToolButton("Buttons");
		AddToolButton("Preview");
	}
	else {
		ToolComponent = GetOwningPlayer() ? GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>() : nullptr;
#if WITH_EDITOR
		OnToolsInitialized();
#else
		if (IsValid(ToolComponent)) ToolComponent->OnToolsInitialized.AddDynamic(this, &UToolSelectionWidget::OnToolsInitialized);
#endif
	}
}

void UToolSelectionWidget::NativeDestruct() {
	Super::NativeDestruct();

	if (IsValid(ToolComponent)) {
		ToolComponent->OnToolsInitialized.RemoveDynamic(this, &UToolSelectionWidget::OnToolsInitialized);
		ToolComponent->OnToolChanged.RemoveDynamic(this, &UToolSelectionWidget::OnActiveToolChanged);
		ToolComponent = nullptr;
	}
}
