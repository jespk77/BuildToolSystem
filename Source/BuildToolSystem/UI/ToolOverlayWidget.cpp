#include "ToolOverlayWidget.h"
#include "../Game/BuildToolComponent.h"

void UToolOverlayWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	Owner = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	if (!Owner) Owner = GetOwningPlayerPawn()->GetComponentByClass<UBuildToolComponent>();
	if (!Owner) UE_LOG(LogToolSystem, Warning, TEXT("Missing BuildToolComponent on player controller or player pawn! Input will not work for tools"));
}

void UToolOverlayWidgetBase::NativeDestruct() {
	Super::NativeDestruct();
	Owner = nullptr;
}

#define HandleEvent(NativeFunction, ControllerFunction, ...)\
	Super::NativeFunction(__VA_ARGS__);\
	if(Owner) Owner->ControllerFunction(__VA_ARGS__);

#define HandleEventWithReply(NativeFunction, ControllerFunction, ReplyType, ...)\
	const ReplyType reply = Super::NativeFunction(__VA_ARGS__);\
	return !reply.IsEventHandled() && Owner && Owner->ControllerFunction(__VA_ARGS__) ? ReplyType::Handled() : reply;

FReply UToolOverlayWidgetBase::NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseButtonDown, OnMouseDown, FReply, geometry, event);
}

FReply UToolOverlayWidgetBase::NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseButtonUp, OnMouseUp, FReply, geometry, event);
}

FReply UToolOverlayWidgetBase::NativeOnMouseMove(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseMove, OnMouseMove, FReply, geometry, event);
}

FReply UToolOverlayWidgetBase::NativeOnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseWheel, OnMouseWheel, FReply, geometry, event);
}

FReply UToolOverlayWidgetBase::NativeOnMouseButtonDoubleClick(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseButtonDoubleClick, OnMouseDoubleDown, FReply, geometry, event);
}

#undef HandleEvent
#undef HandleEventWithReply
