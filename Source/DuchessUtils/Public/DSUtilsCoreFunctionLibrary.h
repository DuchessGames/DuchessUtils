
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DSUtilsCoreFunctionLibrary.generated.h"

UCLASS()
class DUCHESSUTILS_API UDSUtilsCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject"), Category = "DSUtils")
	static ULocalPlayer* GetLocalPlayerForContext(const UObject* WorldContextObject);
	
	static ULocalPlayerSubsystem* GetLocalPlayerSubsystemForContext(const UObject* WorldContextObject, TSubclassOf<ULocalPlayerSubsystem> Class);
	
	template <typename TSubsystemClass>
	static TSubsystemClass* GetLocalPlayerSubsystemForContext(const UObject* WorldContextObject)
	{
		static_assert(TIsDerivedFrom<TSubsystemClass, ULocalPlayerSubsystem>::IsDerived, "TSubsystemClass must be derived from ULocalPlayerSubsystem");
		return Cast<TSubsystemClass>(GetLocalPlayerSubsystemForContext(WorldContextObject, TSubsystemClass::StaticClass()));
	}

	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject"), Category = "DSUtils")
	static bool GlobalTestFunction(const UObject* WorldContextObject);
	
	// /** Blueprint-версия GetLocalPlayerSubsystemForContext с динамическим типом возвращаемого значения. */
	// UFUNCTION(BlueprintCallable, CustomThunk, Category = "DSUtils", meta = (DisplayName = "Get Local Player Subsystem (by Class)", WorldContext = "WorldContextObject", DeterminesOutputType = "Class", DynamicOutputParam = "OutSubsystem"))
	// static void BP_GetLocalPlayerSubsystemForContext(const UObject* WorldContextObject, TSubclassOf<ULocalPlayerSubsystem> Class, ULocalPlayerSubsystem*& OutSubsystem);
	//
	// DECLARE_FUNCTION(execBP_GetLocalPlayerSubsystemForContext);
private:
	
};
