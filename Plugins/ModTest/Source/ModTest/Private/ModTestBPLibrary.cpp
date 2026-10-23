// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModTestBPLibrary.h"
#include "ModTest.h"

UModTestBPLibrary::UModTestBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UModTestBPLibrary::ModTestSampleFunction(float Param)
{
	return -1;
}

