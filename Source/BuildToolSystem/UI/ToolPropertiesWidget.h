#pragma once

#include "CoreMinimal.h"
#include "WidgetUtilities/Widgets/DynamicEditorWidget.h"
#include "ToolPropertiesWidget.generated.h"

class UBuildTool;
class UBuildToolComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolPropertiesWidgetBase : public UDynamicEditorWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> ToolComponent;

	UFUNCTION() void OnActiveToolChanged(int32 index);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
