#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestLib.generated.h"

// This class is exported with MYDLLDEMO_API (your module name in ALL CAPS)
UCLASS()
class MYDLLDEMO_API UTestLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Simple test to prove the library shows in Blueprints
	UFUNCTION(BlueprintPure, Category = "MyMod|Test")
	static FString HelloTest();

	// Load the external DLL (returns true if loaded successfully)
	UFUNCTION(BlueprintCallable, Category = "MyMod|DLL")
	static bool LoadMyModDLL();

	// Call into DLL to set the preferred language
	UFUNCTION(BlueprintCallable, Category = "MyMod|DLL")
	static void SetPreferredLanguageBP(const FString& LangCode);

	// Call into DLL to read back the language
	UFUNCTION(BlueprintPure, Category = "MyMod|DLL")
	static FString GetPreferredLanguageBP();

	// Call into DLL to scale a speed value
	UFUNCTION(BlueprintPure, Category = "MyMod|DLL")
	static float ScaleSpeedBP(float BaseSpeed, float Multiplier);

private:
	// Handle to the loaded DLL
	static void* DllHandle;

	// Typedefs for the functions we expect inside the DLL
	typedef void        (*SetLangFn)(const char*);
	typedef const char* (*GetLangFn)();
	typedef float       (*ScaleSpeedFn)(float, float);

	// Function pointers bound when the DLL is loaded
	static SetLangFn     pSetLang;
	static GetLangFn     pGetLang;
	static ScaleSpeedFn pScaleSpeed;
};