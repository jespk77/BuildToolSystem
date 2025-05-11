#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ToolOverlayWidget.generated.h"

class UBuildToolComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolOverlayWidgetBase : public UCommonUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> Owner;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseMove(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& geometry, const FPointerEvent& event) override;
};