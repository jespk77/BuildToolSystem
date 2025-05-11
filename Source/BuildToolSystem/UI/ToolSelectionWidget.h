#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ToolSelectionWidget.generated.h"

class UBuildToolComponent;

UCLASS()
class BUILDTOOLSYSTEM_API UToolSelectionWidget : public UCommonUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> Owner;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
