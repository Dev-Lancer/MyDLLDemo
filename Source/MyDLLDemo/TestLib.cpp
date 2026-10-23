#include "TestLib.h"

#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Containers/StringConv.h"

// --- Static variable definitions ---
void* UTestLib::DllHandle = nullptr;
UTestLib::SetLangFn    UTestLib::pSetLang = nullptr;
UTestLib::GetLangFn    UTestLib::pGetLang = nullptr;
UTestLib::ScaleSpeedFn UTestLib::pScaleSpeed = nullptr;

// --- A simple test function ---
FString UTestLib::HelloTest()
{
    return TEXT("Hello from TestLib");
}

// --- Load the DLL into memory ---
bool UTestLib::LoadMyModDLL()
{
    if (DllHandle) return true;

#if PLATFORM_WINDOWS
    const FString DllPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/MyMod.dll"));

    DllHandle = FPlatformProcess::GetDllHandle(*DllPath);
    if (!DllHandle) return false;

    pSetLang = reinterpret_cast<SetLangFn>(FPlatformProcess::GetDllExport(DllHandle,
        TEXT("SetPreferredLanguage")));
    pGetLang = reinterpret_cast<GetLangFn>(FPlatformProcess::GetDllExport(DllHandle,
        TEXT("GetPreferredLanguage")));
    pScaleSpeed = reinterpret_cast<ScaleSpeedFn>(FPlatformProcess::GetDllExport(DllHandle,
        TEXT("ScaleSpeed")));

    if (!(pSetLang && pGetLang && pScaleSpeed))
    {
        FPlatformProcess::FreeDllHandle(DllHandle);
        DllHandle = nullptr;
        return false;
    }
    return true;
#else
    return false;
#endif
}

// --- Set preferred language (calls into DLL) ---
void UTestLib::SetPreferredLanguageBP(const FString& LangCode)
{
    if (!DllHandle && !LoadMyModDLL()) return;

    if (pSetLang)
    {
        // Convert FString (TCHAR*) -> ANSI C-string safely using Unreal's macro
        pSetLang(TCHAR_TO_ANSI(*LangCode));
    }
}

// --- Get preferred language (calls into DLL) ---
FString UTestLib::GetPreferredLanguageBP()
{
    if (!DllHandle && !LoadMyModDLL()) return TEXT("en");

    if (pGetLang)
    {
        const char* c = pGetLang();
        return FString(ANSI_TO_TCHAR(c ? c : "en"));
    }

    return TEXT("en");
}

// --- Scale a speed value (calls into DLL) ---
float UTestLib::ScaleSpeedBP(float BaseSpeed, float Multiplier)
{
    if (!DllHandle && !LoadMyModDLL()) return BaseSpeed * Multiplier;

    return pScaleSpeed ? pScaleSpeed(BaseSpeed, Multiplier) : BaseSpeed * Multiplier;
}