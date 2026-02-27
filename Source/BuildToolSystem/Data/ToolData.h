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

	const UBuildTool* GetDefaultTool() const;
	FName GetToolName() const;
	void GetToolName(FName& name) const;
	bool IsValid() const { return ToolClass && !ToolClass->HasAnyClassFlags(CLASS_Abstract); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class BUILDTOOLSYSTEM_API UToolData : public UDataAsset {
	GENERATED_BODY()

protected:
	/** Optional parent object, tools contained will be appended to this tool list */
	UPROPERTY(Category = "Inheritance", EditAnywhere)
	TObjectPtr<UToolData> Parent;

	UPROPERTY(Category = "Tools", EditAnywhere)
	TArray<FToolSettings> Tools;

public:
	UFUNCTION(Category = "Tools", BlueprintCallable)
	bool HasTool(const FName& toolName) const;

	UFUNCTION(Category = "Tools", BlueprintCallable)
	void GetTools(TArray<FToolSettings>& activeTools) const;
};
