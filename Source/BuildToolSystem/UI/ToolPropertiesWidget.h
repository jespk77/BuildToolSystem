#pragma once

#include "Blueprint/UserWidget.h"
#include "ToolPropertiesWidget.generated.h"

class UBuildTool;
class UBuildToolComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolPropertiesEditorWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<UBuildTool> Tool;

	template<class ToolClass>
	ToolClass* GetTool() const { return Cast<ToolClass>(Tool); }

public:
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void InitializeTool(UBuildTool* newTool);
};

UCLASS(NotBlueprintable)
class BUILDTOOLSYSTEM_API UToolPropertiesWidget : public UUserWidget {
	GENERATED_BODY()

private:
	UFUNCTION() void OnActiveToolChanged(int32 index);

protected:
	TSharedPtr<SBorder> Container;

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> ToolComponent;

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<UUserWidget> EditorWidget;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool releaseChildren) override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
};
