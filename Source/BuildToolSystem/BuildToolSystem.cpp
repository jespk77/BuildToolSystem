#include "BuildToolSystem.h"

#define LOCTEXT_NAMESPACE "FBuildToolSystemModule"

DEFINE_LOG_CATEGORY(LogToolSystem);

void FBuildToolSystemModule::StartupModule() { }

void FBuildToolSystemModule::ShutdownModule() { }

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBuildToolSystemModule, BuildToolSystem)
