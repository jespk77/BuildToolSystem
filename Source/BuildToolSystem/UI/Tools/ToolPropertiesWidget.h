#pragma once

#include "CommonUserWidget.h"
#include "ToolPropertiesWidget.generated.h"

class UBuildTool;
class UBuildToolComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolPropertiesWidgetBase : public UCommonUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> ToolComponent;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UUserWidget> ActiveWidget;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnActiveToolChanged(int32 index);
	virtual void OnActiveToolChanged_Implementation(int32 index);

	UFUNCTION(Category = "Events", BlueprintImplementableEvent)
	void SetPropertyWidget(UUserWidget* widget);
};
