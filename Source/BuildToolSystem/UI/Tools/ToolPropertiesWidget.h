#pragma once

#include "../PropertyWidgetBase.h"
#include "ToolPropertiesWidget.generated.h"

class UBuildTool;
class UBuildToolComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolPropertiesWidgetBase : public UPropertyWidgetBase {
	GENERATED_BODY()

protected:
	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnActiveToolChanged(int32 index);
	virtual void OnActiveToolChanged_Implementation(int32 index);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
