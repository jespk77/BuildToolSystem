#pragma once

#include "Blueprint/UserWidget.h"
#include "ToolSelectionWidget.generated.h"

UCLASS()
class BUILDTOOLSYSTEM_API UToolSelectionWidget : public UUserWidget {
	GENERATED_BODY()

private:
	UFUNCTION() void OnActiveToolChanged(int32 index);

protected:
	TSharedPtr<SHorizontalBox> Container;
	TArray<TSharedPtr<class SToggleableButton>> Buttons;

	UPROPERTY(Category = "Interface", EditAnywhere)
	FButtonStyle ButtonStyle = FButtonStyle::GetDefault();

	UPROPERTY(Category = "Interface", EditAnywhere)
	FTextBlockStyle ButtonTextStyle = FTextBlockStyle::GetDefault();

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<class UBuildToolComponent> ToolComponent;

	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void AddToolButton(const FName& toolName);

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool releaseChildren) override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
};
