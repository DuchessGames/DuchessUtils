#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "DSInstancedStruct.generated.h"

template<typename T>
struct THasTBaseStructureGet
{
private:
	// Проверяем, можно ли корректно вызвать TBaseStructure<U>::Get()
	template<typename U>
	static auto Test(int) -> decltype((void)TBaseStructure<U>::Get(), std::true_type{});

	// fallback: если выражение некорректно
	template<typename>
	static std::false_type Test(...);

public:
	using CleanT = std::remove_cv_t<std::remove_reference_t<T>>;
	static constexpr bool Value = decltype(Test<CleanT>(0))::value;
};

// helper для static_assert
template<typename T>
inline constexpr bool THasTBaseStructureGet_v = THasTBaseStructureGet<T>::Value;

template<typename T>
struct always_false : std::false_type {};

USTRUCT(BlueprintType)
struct DUCHESSUTILS_API FDSWrapperInt
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct DUCHESSUTILS_API FDSWrapperFloat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;
};

USTRUCT(BlueprintType)
struct DUCHESSUTILS_API FDSWrapperBool
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Value = false;
};

USTRUCT(BlueprintType)
struct DUCHESSUTILS_API FDSWrapperName
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Value;
};

USTRUCT(BlueprintType)
struct DUCHESSUTILS_API FDSWrapperString
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value;
};

USTRUCT(BlueprintType)
struct DUCHESSUTILS_API FDSWrapperText
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Value;
};

// ======================================================
// 1. Базовый шаблон — по умолчанию тип сам USTRUCT
// ======================================================
template<typename T, typename Enable = void>
struct TDSWrapperTraits
{
	static FInstancedStruct Make(const T& Value)
	{
		static_assert(THasTBaseStructureGet<T>::Value, "T должен быть USTRUCT или иметь TBaseStructure<T>::Get()");
		return FInstancedStruct::Make(Value);
	}

	static bool Extract(const FInstancedStruct& Data, T& OutValue)
	{
		if (!Data.IsValid() || !Data.GetScriptStruct())
			return false;

		if (Data.GetScriptStruct() == TBaseStructure<T>::Get())
		{
			OutValue = Data.Get<T>();
			return true;
		}
		return false;
	}
};

// ======================================================
// 2. Макрос для простых врапперов (типы с полем Value)
// ======================================================
#define DECLARE_DSITEMDATA_WRAPPER(SimpleType, WrapperType) \
template<> struct TDSWrapperTraits<SimpleType> \
{ \
static FInstancedStruct Make(const SimpleType& Value) \
{ \
return FInstancedStruct::Make(WrapperType{ Value }); \
} \
static bool Extract(const FInstancedStruct& Data, SimpleType& OutValue) \
{ \
if (Data.GetScriptStruct() == WrapperType::StaticStruct()) \
{ \
OutValue = Data.Get<WrapperType>().Value; \
return true; \
} \
return false; \
} \
};

DECLARE_DSITEMDATA_WRAPPER(int32, FDSWrapperInt)
DECLARE_DSITEMDATA_WRAPPER(float, FDSWrapperFloat)
DECLARE_DSITEMDATA_WRAPPER(bool, FDSWrapperBool)
DECLARE_DSITEMDATA_WRAPPER(FName, FDSWrapperName)
DECLARE_DSITEMDATA_WRAPPER(FString, FDSWrapperString)
DECLARE_DSITEMDATA_WRAPPER(FText, FDSWrapperText)

// ===================================================================
// DSMakeInstancedStruct
// Упаковывает любой поддерживаемый тип в FInstancedStruct
// ===================================================================

template<typename T>
FInstancedStruct DSMakeInstancedStruct(const T& Value)
{
	using CleanT = std::remove_cv_t<std::remove_reference_t<T>>;
	return TDSWrapperTraits<CleanT>::Make(Value);
}

template<typename T>
bool DSGetValueFromInstancedStruct(const FInstancedStruct& Data, T& OutValue)
{
	using CleanT = std::remove_cv_t<std::remove_reference_t<T>>;
	return TDSWrapperTraits<CleanT>::Extract(Data, OutValue);
}

