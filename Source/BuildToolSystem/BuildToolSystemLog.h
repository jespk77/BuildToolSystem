#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogToolSystem, Log, Verbose);

#define TOOLSYSTEM_LOG(Level, Text, ...) UE_LOG(LogToolSystem, Level, TEXT(Text), ##__VA_ARGS__)

// define custom categories that can be disabled to prevent unnecessary log spamming and improved performance

#define RAYCAST_DEBUG WITH_EDITOR && 1

#define SELECTION_DEBUG WITH_EDITOR && 1
