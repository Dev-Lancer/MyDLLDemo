#include "MyModBridgeBPLibrary.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include <string>

void* UMyModBridgeBPLibrary::DllHandle = nullptr;
UMyModBridgeBPLibrary::SetLangFn    UMyModBridgeBPLibrary::pSetLang = nullptr;
UMyModBridgeBPLibrary::GetLangFn    UMyModBridgeBPLibrary::pGetLang = nullptr;
UMyModBridgeBPLibrary::ScaleSpeedFn UMyModBridgeBPLibrary::pScaleSpeed = nullptr;

bool UMyModBridgeBPLibrary::LoadMyModDLL()
{
    if (DllHandle) return true;

#if PLATFORM_WINDOWS
    // When using a plugin we can still reference the staged DLL by name at runtime:
    // Editor runs from Binaries/Win64; packaged runs from the exe folder. RuntimeDependencies handles copying.
    DllHandle = FPlatformProcess::GetDllHandle(TEXT("MyMod.dll"));
    if (!DllHandle) return false;

    pSetLang = reinterpret_cast<SetLangFn>(FPlatformProcess::GetDllExport(DllHandle,
        TEXT("SetPreferredLanguage")));
    pGetLang = reinterpret_cast<GetLangFn>(FPlatformProcess::GetDllExport(DllHandle,
        TEXT("GetPreferredLanguage")));
    pScaleSpeed = reinterpret_cast<ScaleSpeedFn>(FPlatformProcess::GetDllExport(DllHandle,
        TEXT("ScaleSpeed")));

    const bool ok = (pSetLang && pGetLang && pScaleSpeed);
    if (!ok)
    {
        FPlatformProcess::FreeDllHandle(DllHandle);
        DllHandle = nullptr;
    }
    return ok;
#else
    return false;
#endif
}

void UMyModBridgeBPLibrary::SetPreferredLanguage(const FString& LangCode)
{
    if (!DllHandle && !LoadMyModDLL()) return;
    if (pSetLang)
    {
        std::string s = TCHAR_TO_ANSI(*LangCode);
        pSetLang(s.c_str());
    }
}

FString UMyModBridgeBPLibrary::GetPreferredLanguage()
{
    if (!DllHandle && !LoadMyModDLL()) return TEXT("en");
    if (pGetLang)
    {
        const char* c = pGetLang();
        return FString(ANSI_TO_TCHAR(c ? c : "en"));
    }
    return TEXT("en");
}

float UMyModBridgeBPLibrary::ScaleSpeed(float BaseSpeed, float Multiplier)
{
    if (!DllHandle && !LoadMyModDLL()) return BaseSpeed * Multiplier;
    return pScaleSpeed ? pScaleSpeed(BaseSpeed, Multiplier) : BaseSpeed * Multiplier;
}