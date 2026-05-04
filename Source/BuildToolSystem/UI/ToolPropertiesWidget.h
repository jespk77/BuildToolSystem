#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreGameUI/Widgets/PropertyWidgetContainer.h"
#include "ToolPropertiesWidget.generated.h"

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolPropertiesEditorWidget : public UUserWidget, public IPropertyWidgetContainer {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<class UBuildTool> Tool;

	template<class ToolClass>
	ToolClass* GetTool() const { return Cast<ToolClass>(Tool); }
	UFUNCTION(Category="Tools", BlueprintCallable, meta=(DeterminesOutputType="toolClass"))
	class UBuildTool* GetToolAs(TSubclassOf<class UBuildTool> toolClass) const { return Tool; }

	UFUNCTION(Category = "Tools", BlueprintImplementableEvent)
	void OnToolSelectionUpdated();

public:
	virtual void NativeDestruct() override;

	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void InitializeTool(UBuildTool* newTool);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(NotBlueprintable)
class BUILDTOOLSYSTEM_API UToolPropertiesWidget : public UUserWidget {
	GENERATED_BODY()

private:
	UFUNCTION() void OnActiveToolChanged(int32 index);

protected:
	TSharedPtr<SBorder> Container;

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<class UBuildToolComponent> ToolComponent;

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<UUserWidget> EditorWidget;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool releaseChildren) override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) { return FReply::Handled(); }
	virtual FReply NativeOnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) { return FReply::Handled(); }
};
