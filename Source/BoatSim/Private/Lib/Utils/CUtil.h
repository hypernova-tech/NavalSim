// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"
#include "Lib/Utils/CScanResult.h"
#include "Lib/Clutter/ClutterDefs.h"
#include "Camera/CameraComponent.h"
/**
 * 
 */


struct STraceArgs
{

public:
	STraceArgs()
	{
		include_actor_list = nullptr;
	}
	AActor* p_actor;
	FVector scan_center;
	FVector scan_rpy_world_deg;
	bool is_world;







	float min_range_meter;
	float range_meter;
	float azimuth_start_deg;
	float azimuth_end_deg;

	float elevation_start_deg;
	float elevation_end_deg;
	float azimuth_angle_step_deg;
	float elevation_angle_step_deg;
	float measurement_error_mean;
	float measurement_error_std;
    SClutterParams clutter_params;

	bool show_radar_beam;
	TArray<AActor*> *p_ignore_list;
	TArray<AActor*> additional_ignore_list;
	bool create_scan_line;
	bool use_render_target = false;
	bool is_normal_distribution = false;

	TArray<AActor*> *include_actor_list;
};

class CUtil
{
public:
	CUtil();
	~CUtil();


public:
	static bool DoesExist(FString name, UWorld* p_world);
	static bool Trace(const STraceArgs &args, SScanResult* pscan_result);

	static inline void ScanPie(const STraceArgs& args, double azimuth, double elevation, FVector& look_dir, FVector& start_loc, FVector& end, const FVector& right_vec, const FVector& up_vec, bool& ret, FHitResult& result, FCollisionQueryParams& query_params, SScanResult* pscan_result, int horizantal_ind, int vertical_ind, SSectorInfo* p_current_sektor, int& success_count);


	static void DrawDebugRay(UWorld* p_world, const FVector& Start, const FVector& End, FColor Color, float Duration, float Thickness);

	static bool ParseVector2D(const FString& Text, FVector2D& ret);
	static bool ParseVector3D(const FString& Text, FVector& ret);
	static bool ParseColor(const FString& Text, FColor& ret);
	static FString ColorToString(const FColor& color);
	static bool ParsePose(const FString& Text, FVector& translation, FVector& rpy);
	static void DebugLog(FString str);
	static void DebugLogScreen(FString str, FLOAT32 duration_sec = 5, FColor col = FColor::Red);
	template <typename T>
	static T* FindComponent(AActor* p_parent);
	template <typename T>
	static T* FindChildActor(AActor* p_parent);
	template <typename T>
	static TArray<T*> FindChildActors(AActor* p_parent);


	template<typename T>
	static T* FindComponentOfTag(AActor* p_actor, FString tag)
	{
		TArray<T*> comps;
		p_actor->GetComponents<T>(comps);

		// Now StaticMeshComponents contains all UStaticMeshComponent instances within the actor
		for (T* cmp : comps)
		{
			if (cmp->ComponentHasTag(FName(tag)))
			{
				// If yes, add it to the output array
				return cmp;
			}
		}

		return nullptr;
	}

	template<typename T>
	static void FindChildActorsRecursive(AActor* ParentActor, TArray<T*>& OutChildren)
	{
		TArray<AActor*> AttachedActors;
		ParentActor->GetAttachedActors(AttachedActors);

		for (AActor* Child : AttachedActors)
		{
			if (Child && !Child->IsPendingKill())
			{
				if (Child->IsA<T>()) {
					T* p_ret = Cast<T>(Child);
					OutChildren.Add(p_ret);
				}

				// Optionally, add a recursive call here to delve deeper into the hierarchy
				FindChildActorsRecursive(Child, OutChildren);
			}
		}
	};

	template<typename T>
	static T* FindChildComponent(AActor* Parent)
	{
		if (Parent == nullptr)
		{
			return nullptr;
		}

		// Iterate over all components of the parent actor
		TArray<UActorComponent*> Components;
		Parent->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			// Check if the component is of the desired class
			T* DesiredComponent = Cast<T>(Component);
			if (DesiredComponent != nullptr)
			{
				// Found the component, return it
				return DesiredComponent;
			}
		}

		// Component not found
		return nullptr;
	};

	static void LookAt(AActor* p_actor, FVector& look_dir, FLOAT32 distance = 5);
	
	static FRotator GetActorRelativeRotation(AActor* p_actor);
	/// <summary>
	/// String conversions
	/// </summary>
	/// <param name="p_str"></param>
	/// <returns></returns>
	static float ConvertToFloat(const char* p_str);
	static FString ConvertToFString(const char* p_str);
	static int StringToInt(FString& str);
	static FString BoolToStringBinary(bool val);
	static FString IntToString(INT32S val);
	static FString IntToString(INT32U val);
	static FString IntToString(INT64S val);
	static FString IntToString(INT64U val);
	static FString FloatToString(FLOAT32 val);
	static FString FloatToString(FLOAT64 val);

	static FString CharToFString(const char* p_char);
	static void FStringToAsciiChar(const FString& str, char* p_dest, INT32U dest_len);


	static bool StringToBool(FString& value) ;
	static INT8S StringToInt8(FString& value) ;
	static INT8U StringToInt8U(FString& value) ;
	static INT32U StringToInt32U(FString& value) ;
	static INT32S StringToInt32S(FString& value) ;
	static FLOAT32 StringToFloat(FString& str);
	static FLOAT64 StringToFloat64(FString& str);
	static FLOAT64 StringToDouble(FString& value);
	static FVector2D StringToFVector2D(FString& value) ;
	static FVector StringToFVector(FString& value) ;

	static FString VectorToString(FVector& value, INT32S digit = -1);
	static FString VectorToString(FVector2D& value, INT32S digit = -1);
	static FString VectorToString(FVector4& value, INT32S digit = -1);

	/// <summary>
	/// Byte operations
	/// </summary>
	/// <param name="str"></param>
	/// <param name="p_dest"></param>
	/// <param name="dest_capacity"></param>
	static void ByteCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void IPAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void MacAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static AActor* GetTopParent(AActor *p_actor);
	static AActor* SpawnObjectFromBlueprint(AActor* p_template, FString blueprint_path, UWorld* p_world, AActor* p_owner, FString name, FVector pos, FVector rot_rpy, FVector scale);
	static FLOAT64 GetTimeSeconds();
	static inline INT16U LittleToBig(INT16U val)
	{
		return (((val >> 8) & 0xFF) )| ((val << 8) & 0xFF00);
	}
	static inline INT32U LittleToBig(INT32U val)
	{
		return ((val << 24) & 0xFF000000) |
			((val << 8) & 0x00FF0000) |
			((val >> 8) & 0x0000FF00) |
			((val >> 24) & 0x000000FF);
	}
	static inline INT32U ReverseCopyBytes(INT8U* p_src, INT8U* p_dest, INT32U len)
	{
		for (INT32U i = 0; i < len; i++) {
			p_dest[i] = p_src[len - 1 - i];
		}
	}
	
	static FVector GetActorRPY(AActor* p_actor);
	static AActor* GetParentActor(AActor *p_child);
	static FString GetActorFullComponentPath(AActor* p_actor);
	static void GetOwnAndParents(AActor* p_child, TArray<AActor*> &ret);
	static FLOAT64 Tick();
	static FLOAT64 Tock(FLOAT64 start_time_sec);
	static bool SetParent(AActor* p_child, AActor* p_parent);
	static bool SetParentComponent(AActor* p_child, USceneComponent* p_parent);
	static void SetActorActive(AActor* p_actor, bool is_active);
	static bool GetIsActorTickEnabled(AActor* p_actor);
	static bool GetIsActorVisible(AActor* p_actor);
	static bool IsPossedByPlayerController(UWorld *p_world, APawn *p_pawn);
	static void CameraLookAt(UCameraComponent* p_looker, AActor* p_target, FLOAT32 distance);
	static void CameraLookAt(UCameraComponent* p_looker, FVector pos);
	static void GetActorHierarchy(const AActor* Object, TArray<FString>& Hierarchy);
	static void GetActorAndComponentHierarchy(const AActor* Actor, TArray<FString>& Hierarchy);
	static void GetComponentHierarchy(const USceneComponent* Component, TArray<FString>& Hierarchy);
	static TArray<uint8> HexStringToByteArray(const FString& HexString);
	static FString CharToHexString(INT8U* p_data, INT32U len);
	static void FindMinMaxPixelValue16Bit(UTexture2D* Texture, uint16& MinValue, uint16& MaxValue);
	static FString GetFilePathProject(FString file_path);
	static bool IsObjectCreatedAtRuntime(UObject* Object);
	static TArray<UActorComponent*> GetComponentHierarchyToTop(UActorComponent* StartComponent);
	static void AddComponentsToHierarchyArray(USceneComponent* Component, TArray<UActorComponent*>& HierarchyArray);
		
	static USceneComponent* FindComponentByPath(USceneComponent* RootComponent, const TArray<FString>& Parts, int32 PartIndex);
	static USceneComponent* FindComponentByPath(AActor* RootActor, const FString& Path);
	static bool IsDefaultSceneComponent(USceneComponent* p_comp);


	static void BuildComponentPaths(USceneComponent* Component, const FString& BasePath, TArray<FString>& OutPaths);
	static TArray<FString> GetAllComponentPaths(AActor* Actor);
	static void ReattachComponentToActor(UPrimitiveComponent* ChildComponent, AActor* ParentActor, FName SocketName = NAME_None);
};

