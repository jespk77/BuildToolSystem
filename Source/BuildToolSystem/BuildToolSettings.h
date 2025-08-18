#pragma once

#include "Engine/DeveloperSettings.h"
#include "BuildToolSettings.generated.h"

class UBuildTool;

USTRUCT(BlueprintType)
struct BUILDTOOLSYSTEM_API FToolSettings {
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Class data", Config, EditAnywhere)
	TSubclassOf<UBuildTool> ToolClass;

	UPROPERTY(Category = "Class data", Config, EditAnywhere)
	TSubclassOf<UUserWidget> ToolWidget;

	bool IsValid() const { return ToolClass && !ToolClass->HasAnyClassFlags(CLASS_Abstract); }
};

UCLASS(Config = Engine, DefaultConfig, meta = (DisplayName = "Tool System"))
class BUILDTOOLSYSTEM_API UBuildToolSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	UBuildToolSettings(const FObjectInitializer& initializer = FObjectInitializer::Get());
	FName GetContainerName() const override;

	UPROPERTY(Category = "Tools", Config, EditAnywhere)
	TArray<FToolSettings> Tools;

	UPROPERTY(Category = "Selection", Config, EditAnywhere)
	TEnumAsByte<ECollisionChannel> SelectionTraceChannel = ECC_Camera;
};
