#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyModBridgeBPLibrary.generated.h"

UCLASS()
class MYMODBRIDGE_API UMyModBridgeBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MyMod|DLL")
    static bool LoadMyModDLL();

    UFUNCTION(BlueprintCallable, Category = "MyMod|Language", DisplayName = "MyMod Set Preferred Language")
    static void SetPreferredLanguage(const FString& LangCode);

    UFUNCTION(BlueprintCallable, Category = "MyMod|Language", DisplayName = "MyMod Get Preferred Language")
    static FString GetPreferredLanguage();

    UFUNCTION(BlueprintPure, Category = "MyMod|Gameplay", DisplayName = "MyMod Scale Speed")
    static float ScaleSpeed(float BaseSpeed, float Multiplier);

private:
    static void* DllHandle;
    typedef void        (*SetLangFn)(const char*);
    typedef const char* (*GetLangFn)();
    typedef float       (*ScaleSpeedFn)(float, float);
    static SetLangFn     pSetLang;
    static GetLangFn     pGetLang;
    static ScaleSpeedFn pScaleSpeed;
};