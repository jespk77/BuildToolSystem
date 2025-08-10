#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ToolPropertiesWidget.generated.h"

class UBuildTool;
class UBuildToolComponent;
#if WITH_EDITORONLY_DATA
class UDynamicEditorWidget;
#endif

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UToolPropertiesWidgetBase : public UCommonUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> ToolComponent;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UUserWidget> ActiveWidget;

	UFUNCTION(Category = "Property Widget", BlueprintImplementableEvent)
	void SetPropertyWidget(UUserWidget* widget);

	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnActiveToolChanged(int32 index);
	virtual void OnActiveToolChanged_Implementation(int32 index);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
