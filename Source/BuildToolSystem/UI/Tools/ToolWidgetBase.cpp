#include "ToolWidgetBase.h"
#include "BuildToolSystem/Components/BuildToolComponent.h"
#include "Components/LineBatchComponent.h"

void UToolWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	Owner = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	if (!Owner) Owner = GetOwningPlayerPawn()->GetComponentByClass<UBuildToolComponent>();
	if (!Owner) UE_LOG(LogToolSystem, Warning, TEXT("Missing BuildToolComponent on player controller or player pawn! Input will not work for tools"));
}

void UToolWidgetBase::NativeDestruct() {
	Super::NativeDestruct();
	Owner = nullptr;
}

#define HandleEvent(NativeFunction, ControllerFunction, ...)\
	Super::NativeFunction(__VA_ARGS__);\
	if(Owner) Owner->ControllerFunction(__VA_ARGS__);

#define HandleEventWithReply(NativeFunction, ControllerFunction, ReplyType, ...)\
	const ReplyType reply = Super::NativeFunction(__VA_ARGS__);\
	const bool handled = Owner->ControllerFunction(__VA_ARGS__);\
	return handled ? FReply::Handled() : reply;

FReply UToolWidgetBase::NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseButtonDown, OnMouseDown, FReply, geometry, event);
}

FReply UToolWidgetBase::NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseButtonUp, OnMouseUp, FReply, geometry, event);
}

FReply UToolWidgetBase::NativeOnMouseMove(const FGeometry& geometry, const FPointerEvent& event) {
	Owner->OnMouseMove(geometry, event);
	return FReply::Unhandled();
}

FReply UToolWidgetBase::NativeOnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseWheel, OnMouseWheel, FReply, geometry, event);
}

FReply UToolWidgetBase::NativeOnMouseButtonDoubleClick(const FGeometry& geometry, const FPointerEvent& event) {
	HandleEventWithReply(NativeOnMouseButtonDoubleClick, OnMouseDoubleDown, FReply, geometry, event);
}

#undef HandleEvent
#undef HandleEventWithReply
