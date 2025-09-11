#include "ToolInteractionWidget.h"
#include "BuildToolSystem/Components/BuildToolComponent.h"

TSharedRef<SWidget> UToolInteractionWidget::RebuildWidget() {
	return SAssignNew(Overlay, SOverlay);
}

void UToolInteractionWidget::ReleaseSlateResources(bool includeChildren) {
	Overlay.Reset();
	Super::ReleaseSlateResources(includeChildren);
}

void UToolInteractionWidget::NativeConstruct() {
	Super::NativeConstruct();
	ToolComponent = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	SetVisibility(ESlateVisibility::Visible);
}

void UToolInteractionWidget::NativeDestruct() {
	Super::NativeDestruct();
	ToolComponent = nullptr;
}

FReply UToolInteractionWidget::NativeOnKeyChar(const FGeometry& geometry, const FCharacterEvent& event) {
	return Super::NativeOnKeyChar(geometry, event);
}

FReply UToolInteractionWidget::NativeOnKeyDown(const FGeometry& geometry, const FKeyEvent& event) {
	return Super::NativeOnKeyDown(geometry, event);
}

FReply UToolInteractionWidget::NativeOnKeyUp(const FGeometry& geometry, const FKeyEvent& event) {
	return Super::NativeOnKeyUp(geometry, event);
}

FReply UToolInteractionWidget::NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) {
	if (ToolComponent && ToolComponent->OnMouseDown(geometry, event)) return FReply::Handled();
	else return Super::NativeOnMouseButtonDown(geometry, event);
}

FReply UToolInteractionWidget::NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) {
	if (ToolComponent && ToolComponent->OnMouseUp(geometry, event)) return FReply::Handled();
	else return Super::NativeOnMouseButtonUp(geometry, event);
}

FReply UToolInteractionWidget::NativeOnMouseMove(const FGeometry& geometry, const FPointerEvent& event) {
	if (!ToolComponent || !ToolComponent->OnMouseMove(geometry, event)) Super::NativeOnMouseMove(geometry, event);
	// never return FReply::Handled(): we want this to keep getting called when the mouse is moved
	return FReply::Unhandled();
}

FReply UToolInteractionWidget::NativeOnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) {
	if (ToolComponent && ToolComponent->OnMouseWheel(geometry, event)) return FReply::Handled();
	else return Super::NativeOnMouseWheel(geometry, event);
}

FReply UToolInteractionWidget::NativeOnMouseButtonDoubleClick(const FGeometry& geometry, const FPointerEvent& event) {
	if (ToolComponent && ToolComponent->OnMouseDoubleDown(geometry, event)) return FReply::Handled();
	else return Super::NativeOnMouseButtonDoubleClick(geometry, event);
}
