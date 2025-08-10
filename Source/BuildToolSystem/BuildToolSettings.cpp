#include "BuildToolSettings.h"

UBuildToolSettings::UBuildToolSettings(const FObjectInitializer& initializer) : Super(initializer) {
	CategoryName = "Plugins";
}

FName UBuildToolSettings::GetContainerName() const {
	return "Project";
}
