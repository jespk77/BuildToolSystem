#include "ToolData.h"
#include "BuildTool.h"

const UBuildTool* FToolSettings::GetDefaultTool() const {
	return IsValid() ? GetDefault<UBuildTool>(ToolClass) : nullptr;
}

FName FToolSettings::GetToolName() const {
	const UBuildTool* tool = GetDefaultTool();
	return tool ? tool->ToolName : NAME_None;
}

void FToolSettings::GetToolName(FName& name) const {
	if (const UBuildTool* tool = GetDefaultTool()) name = tool->ToolName;
	else name = NAME_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool UToolData::HasTool(const FName& toolName) const {
	return Tools.ContainsByPredicate([toolName](const FToolSettings& tool) {
		return tool.GetToolName().IsEqual(toolName);
	});
}

void UToolData::GetTools(TArray<FToolSettings>& activeTools) const {
	activeTools.Reset();
	if (Parent) {
		Parent->GetTools(activeTools);
		// ensure no duplicate tools exist: in case of a duplicate name prefer ours over the parent's
		for (auto it = activeTools.CreateIterator(); it; ++it) {
			const FToolSettings& tool = *it;
			if (!tool.IsValid() || HasTool(tool.GetToolName())) it.RemoveCurrent();
		}
	}

	activeTools.Append(Tools);
}
