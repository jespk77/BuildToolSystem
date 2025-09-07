#pragma once

#include "Engine/DeveloperSettings.h"
#include "BuildToolSettings.generated.h"

UCLASS(Config = Engine, DefaultConfig, meta = (DisplayName = "Tool System"))
class BUILDTOOLSYSTEM_API UBuildToolSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	UBuildToolSettings(const FObjectInitializer& initializer = FObjectInitializer::Get());
	FName GetContainerName() const override;

	UPROPERTY(Category = "Selection", Config, EditAnywhere)
	TEnumAsByte<ECollisionChannel> SelectionTraceChannel = ECC_Camera;

	UPROPERTY(Category = "Selection", Config, EditAnywhere)
	TSoftClassPtr<UUserWidget> GeneralObjectEditorWidget;
};
