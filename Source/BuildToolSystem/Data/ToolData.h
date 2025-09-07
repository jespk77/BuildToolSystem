#pragma once

#include "ToolData.generated.h"

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

UCLASS()
class BUILDTOOLSYSTEM_API UToolData : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Tools", EditAnywhere)
	TArray<FToolSettings> Tools;
};
