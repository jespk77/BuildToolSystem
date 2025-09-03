#pragma once

#include "Blueprint/UserWidget.h"
#include "ToolInteractionWidget.generated.h"

class UBuildToolComponent;

UCLASS()
class BUILDTOOLSYSTEM_API UToolInteractionWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	TSharedPtr<SOverlay> Overlay;

	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> ToolComponent;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool includeChildren) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyChar(const FGeometry& geometry, const FCharacterEvent& event) override;
	virtual FReply NativeOnKeyDown(const FGeometry& geometry, const FKeyEvent& event) override;
	virtual FReply NativeOnKeyUp(const FGeometry& geometry, const FKeyEvent& event) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseMove(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& geometry, const FPointerEvent& event) override;
};