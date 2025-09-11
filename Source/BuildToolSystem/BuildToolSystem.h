#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogToolSystem, Log, Log);

class FBuildToolSystemModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
