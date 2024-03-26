
#pragma once

#include "CoreMinimal.h"
#include "Lib/Connection/ConnectionBase.h"

#include "SharedMemory.generated.h"

/**
 * 
 */
struct SSharedMemInitArgs{
	FString Name;
	INT32S size;
	INT32S HeaderSize;
};




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USharedMemory : public UConnectionBase
{
	GENERATED_BODY()

protected:

public:
	UPROPERTY(EditAnywhere)
		FString SharedMemoryName;
	virtual void InitConnection(void *p_args = nullptr) override;
	virtual bool SendData(const INT8U* p_bytes, INT32U count) override;
	virtual bool SendData(const INT8U* p_hdr, INT32U header_size, const INT8U* p_bytes, INT32U count) override;
	virtual void* GetHeader();
};
