
#include "DSUtilsCoreFunctionLibrary.h"

#include "Components/Widget.h"

ULocalPlayer* UDSUtilsCoreFunctionLibrary::GetLocalPlayerForContext(const UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		return nullptr;
	}

	if (const ULocalPlayer* LP = Cast<ULocalPlayer>(WorldContextObject))
	{
		return const_cast<ULocalPlayer*>(LP);
	}

	if (const APlayerController* PC = Cast<APlayerController>(WorldContextObject))
	{
		return PC->GetLocalPlayer();
	}

	if (const UWidget* W = Cast<UWidget>(WorldContextObject))
	{
		return W->GetOwningLocalPlayer();
	}

	if (const APawn* Pawn = Cast<APawn>(WorldContextObject))
	{
		return GetLocalPlayerForContext(Cast<APlayerController>(Pawn->GetController()));
	}

	if (const UActorComponent* AsComp = Cast<UActorComponent>(WorldContextObject))
	{
		return GetLocalPlayerForContext(AsComp->GetOwner());
	}

	if (!WorldContextObject->IsA<AActor>())
	{
		const UObject* Outer = WorldContextObject->GetOuter();
		while (Outer)
		{
			if (const AActor* OuterActor = Cast<AActor>(Outer))
			{
				return GetLocalPlayerForContext(OuterActor);
			}
			Outer = Outer->GetOuter();
		}
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		return GetLocalPlayerForContext(World->GetFirstPlayerController());
	}

	return nullptr;
}

ULocalPlayerSubsystem* UDSUtilsCoreFunctionLibrary::GetLocalPlayerSubsystemForContext(const UObject* WorldContextObject, TSubclassOf<ULocalPlayerSubsystem> Class)
{
	if (ULocalPlayer* LP = GetLocalPlayerForContext(WorldContextObject))
	{
		return LP->GetSubsystemBase(Class);
	}

	return nullptr;
}

bool UDSUtilsCoreFunctionLibrary::GlobalTestFunction(const UObject* WorldContextObject)
{
	UE_LOG(LogCore, Warning, TEXT("GlobalTestFunction Activate"));
	return true;
}

// void UDSGetFunctionLibrary::BP_GetLocalPlayerSubsystemForContext(const UObject* WorldContextObject, TSubclassOf<ULocalPlayerSubsystem> Class, ULocalPlayerSubsystem*& OutSubsystem)
// {
// 	// We should never hit this! stubs to avoid NoExport on the class.
// 	checkNoEntry();
// }

// DEFINE_FUNCTION(UDSGetFunctionLibrary::execBP_GetLocalPlayerSubsystemForContext)
// {
// 	P_GET_OBJECT(UObject, WorldContextObject);
// 	P_GET_OBJECT(UClass, Class);
// 	P_GET_PROPERTY_REF(FIntProperty, Value); // это параметр "int32& Value", но на самом деле мы запишем туда UObject*
//
// 	P_FINISH;
//
// 	P_NATIVE_BEGIN;
//
// 	UObject* Result = nullptr;
//
// 	if (Class && Class->IsChildOf(ULocalPlayerSubsystem::StaticClass()))
// 	{
// 		if (ULocalPlayer* LocalPlayer = GetLocalPlayerForContext(WorldContextObject))
// 		{
// 			Result = LocalPlayer->GetSubsystemBase(Class);
// 		}
// 	}
//
// 	// Поскольку BlueprintCustomThunk не может вернуть UObject напрямую, мы вручную кладём результат в stack.
// 	*(UObject**)(&Value) = Result;
//
// 	P_NATIVE_END;
// }
