#pragma once

#include "ObjectWidgetInterface.generated.h"

UINTERFACE(BlueprintType)
class BUILDTOOLSYSTEM_API UObjectWidgetInterface : public UInterface {
	GENERATED_BODY()
};

class BUILDTOOLSYSTEM_API IObjectWidgetInterface {
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnWidgetActivated(UObjectSelection* selection);

	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnWidgetDeactivated();
};
