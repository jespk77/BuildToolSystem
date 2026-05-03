#pragma once

#include "RaycastParameters.generated.h"

/** Wrapper of FCollisionQueryParams with options for doing raycasts in tools */
USTRUCT(BlueprintType)
struct BUILDTOOLSYSTEM_API FRaycastQueryParameters {
	GENERATED_BODY()

public:
	FRaycastQueryParameters(const FCollisionQueryParams& params = FCollisionQueryParams::DefaultQueryParam) { FromParams(params); }

	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly)
	bool DebugQuery = false;

	/** Whether we want to find out initial overlap or not. If true, it will return if this was initial overlap. */
	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly)
	bool FindInitialOverlap = true;

	/** Whether to ignore traces to the cluster union and trace against its children instead. */
	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly)
	bool SubcomponentTrace = true;

	/** Whether we should trace against complex collision */
	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly)
	bool ComplexCollisionTrace = false;

	/** Whether we want to return the triangle face index for complex static mesh traces */
	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly)
	bool ComplexTraceFaceIndex = false;

	/** Whether we want to include the physical material in the results. */
	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly)
	bool ComplexTracePhysicalMaterial = false;

	/** This filter allows us to refine queries (channel, object) with an additional level of ignore by tagging entire classes of objects (e.g. "Red team", "Blue team")
		If(QueryIgnoreMask & ShapeFilter != 0) filter out */
	UPROPERTY(Category = "Query parameters", EditAnywhere, BlueprintReadOnly, meta = (BitMask))
	uint8 IgnoreMask = 0;

	FCollisionQueryParams ToParams() const {
		FCollisionQueryParams params;
		ToParams(params);
		return params;
	}

	void FromParams(const FCollisionQueryParams& params) {
#if WITH_EDITOR
		DebugQuery = params.bDebugQuery;
#endif
		FindInitialOverlap = params.bFindInitialOverlaps;
		SubcomponentTrace = params.bTraceIntoSubComponents;
		ComplexCollisionTrace = params.bTraceComplex;
		ComplexTraceFaceIndex = params.bReturnFaceIndex;
		ComplexTracePhysicalMaterial = params.bReturnPhysicalMaterial;
		IgnoreMask = params.IgnoreMask;
	}

	void ToParams(FCollisionQueryParams& params) const {
#if WITH_EDITOR
		params.bDebugQuery = DebugQuery;
#endif
		params.bFindInitialOverlaps = FindInitialOverlap;
		params.bTraceIntoSubComponents = SubcomponentTrace;
		params.bTraceComplex = ComplexCollisionTrace;
		params.bReturnFaceIndex = ComplexTraceFaceIndex;
		params.bReturnPhysicalMaterial = ComplexTracePhysicalMaterial;
		params.IgnoreMask = IgnoreMask;
	}

	operator const FCollisionQueryParams() const { return ToParams(); }
	operator FCollisionQueryParams() { return ToParams(); }
	void operator=(FCollisionQueryParams& params) const { ToParams(params); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/** Wrapper of FCollisionResponseParams with options for doing raycasts in tools */
USTRUCT(BlueprintType)
struct BUILDTOOLSYSTEM_API FRaycastResponseParameters {
	GENERATED_BODY()

public:
	FRaycastResponseParameters(const FCollisionResponseParams& params = FCollisionResponseParams::DefaultResponseParam) { FromParams(params); }

	// Uses an FBodyInstance struct so that the default collision preset editor widget is shown when editing its properties

	UPROPERTY(Category = "Response parameters", EditAnywhere)
	FBodyInstance Response;

	void FromParams(const FCollisionResponseParams& params) {
		Response.SetResponseToChannels(params.CollisionResponse);
	}

	FCollisionResponseParams ToParams() const {
		FCollisionResponseParams params;
		ToParams(params);
		return params;
	}

	void ToParams(FCollisionResponseParams& params) const {
		params.CollisionResponse = Response.GetResponseToChannels();
	}

	operator const FCollisionResponseParams() const { return ToParams(); }
	operator FCollisionResponseParams() { return ToParams(); }
	void operator=(FCollisionResponseParams& params) const { ToParams(params); }
};

/** Container with all customization options for doing raycasts in tools */
USTRUCT(BlueprintType)
struct BUILDTOOLSYSTEM_API FRaycastParameters {
	GENERATED_BODY()

public:
	FRaycastParameters() = default;
	explicit FRaycastParameters(TEnumAsByte<ECollisionChannel> channel, FRaycastQueryParameters queryParameters, FRaycastResponseParameters responseParameters)
		: Channel(channel), Query(queryParameters), Response(responseParameters) { }

	UPROPERTY(Category = "Raycast parameters", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> Channel = ECC_Visibility;
	UPROPERTY(Category = "Raycast parameters", EditAnywhere, BlueprintReadWrite)
	FRaycastQueryParameters Query;
	UPROPERTY(Category = "Raycast parameters", EditAnywhere, BlueprintReadWrite)
	FRaycastResponseParameters Response;
};
