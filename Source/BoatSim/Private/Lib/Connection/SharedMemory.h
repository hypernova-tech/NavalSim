
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
	float ContrastLevel;
	float BrightnessLevel;
	struct {
		unsigned int IsIr			: 1;
		unsigned int IsWhiteHot		: 1;
		unsigned int EnableDefog	: 1;
		unsigned int DefogLevel		: 2;
		unsigned int IsICREnabled	: 1;
		unsigned int Reserved		: 26;
	}ImageInfo;
	
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USharedMemory : public UConnectionBase
{
	GENERATED_BODY()

protected:
	unsigned int DataHeight;
	unsigned int DataWidth;
	bool IsIR;
	bool IsWhiteHot;
	bool IsDefogEnabled;
	int   DefogLevel;
	bool IsICREnabled;
public:
	UPROPERTY(EditAnywhere)
		FString SharedMemoryName;
	virtual void InitConnection(void *p_args = nullptr) override;
	virtual bool SendData(const INT8U* p_bytes, INT32U count) override;
	void SetDataDimension(unsigned int width, unsigned int Height, bool is_ir, bool is_white_hot, bool defog_enabled, int defog_level, bool is_icr_enabled);
	
};
