// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include "CoreMinimal.h"
#include "Lib/Connection/SharedMemory.h"
#include "SharedMemoryWinImpl.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USharedMemoryWinImpl : public USharedMemory
{
	GENERATED_BODY()
protected:
	HANDLE Handle;
	PVOID pMemPtr;
	HANDLE CreateOrAccessSharedMemory(const FString sharedMemoryName, size_t size, size_t header_size);

public:
	virtual void InitConnection(void* p_args = nullptr) override;
	virtual bool SendData(const INT8U* p_bytes, INT32U count) override;
	virtual bool SendData(const INT8U* p_hdr, INT32U header_size, const INT8U* p_bytes, INT32U count) override;
	virtual void* GetHeader() override;
};
