#pragma once

#include "ToolWidgetInterface.generated.h"

class UBuildTool;

UINTERFACE(BlueprintType)
class BUILDTOOLSYSTEM_API UToolWidgetInterface : public UInterface {
	GENERATED_BODY()
};

class BUILDTOOLSYSTEM_API IToolWidgetInterface {
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnToolWidgetActivated(UBuildTool* tool);

	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnToolWidgetDeactivated();
};
