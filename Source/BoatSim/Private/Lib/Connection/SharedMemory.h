
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
};


struct SSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int Height;
	unsigned int Width;
};



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USharedMemory : public UConnectionBase
{
	GENERATED_BODY()

protected:
	unsigned int DataHeight;
	unsigned int DataWidth;
public:
	UPROPERTY(EditAnywhere)
		FString SharedMemoryName;
	virtual void InitConnection(void *p_args = nullptr) override;
	virtual bool SendData(const INT8U* p_bytes, INT32U count) override;
	void SetDataDimension(unsigned int width, unsigned int Height);
	
};
