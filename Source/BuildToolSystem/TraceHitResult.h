#pragma once

#include "TraceHitResult.generated.h"

typedef TArray<FHitResult> FHitResults;

/** Extended FHitResult with templated cast functions */
USTRUCT()
struct BUILDTOOLSYSTEM_API FTraceHitResult : public FHitResult {
	GENERATED_BODY()

public:
	FORCEINLINE AActor* GetActor() const {
		return Super::GetActor();
	}

	template<class ActorClass>
	FORCEINLINE ActorClass* GetActor() const {
		return Cast<ActorClass>(GetActor());
	}

	FORCEINLINE UPrimitiveComponent* GetComponent() {
		return Super::GetComponent();
	}

	template<class ComponentClass>
	FORCEINLINE ComponentClass* GetComponent() {
		return Cast<ComponentClass>(GetComponent());
	}
};

typedef TArray<FTraceHitResult> FTraceHitResults;

USTRUCT()
struct BUILDTOOLSYSTEM_API FTraceHitRangeResult {
	GENERATED_BODY()

public:
	FTraceHitResult Line;
	FTraceHitResults Hits;

	FORCEINLINE void Reset() {
		Line.Reset(0.f, false);
		Hits.Reset();
	}

	FVector GetLocation() const { return Line.Location; }
	FVector GetNormal() const { return Line.Normal; }
	float GetDistance() const { return Line.Distance; }

	/* Goes through all hits and find the first one that hit an actor of given class */
	template<class ActorClass>
	ActorClass* GetFirstRangedHitActor() const { return GetFirstRangedHitActor<ActorClass>(*this); }

	/* Goes through all hits and find the first one that hit an actor of given class */
	template<class ActorClass>
	static ActorClass* GetFirstRangedHitActor(const FTraceHitRangeResult& hits) {
		for (const FTraceHitResult& hit : hits.Hits) {
			ActorClass* actor = hit.GetActor<ActorClass>();
			if (actor) return actor;
		}

		return nullptr;
	}

	/* Goes through all hits and find the first one that hit a component of given class */
	template<class ComponentClass>
	ComponentClass* GetFirstRangedHitComponent() const { return GetFirstRangedHitComponent<ComponentClass>(*this); }

	/* Goes through all hits and find the first one that hit a component of given class */
	template<class ComponentClass>
	static ComponentClass* GetFirstRangedHitComponent(const FTraceHitRangeResult& hits) {
		for (const FTraceHitResult& hit : hits.Hits) {
			ComponentClass* component = hit.GetComponent<ComponentClass>();
			if (component) return component;
		}

		return nullptr;
	}
};
