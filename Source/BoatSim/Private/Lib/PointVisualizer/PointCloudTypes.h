#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>

#define POINT_CLOUD_DIST_SF			0.001
#define POINT_CLOUD_STRENGHT_SF		(1.5259021896696421759365224689097e-5)
#define ONE_OVER_POINT_CLOUD_DIST_SF 1000
#define ONE_OVER_POINT_CLOUD_DIST_SF			65535

#pragma pack(push, 1)
enum EPointCloudId :INT8U
{
	PointCloudInfo = 0,
	PointCloudData,
};
#pragma pack(pop)



#pragma pack(push, 1)
struct SPointCloudInfoPayload
{
	FLOAT64 TimeStamp;
	INT32U CaptureInd;
	INT32U PointCount;
	INT8U IsPoint3D;
	FLOAT64 ScanCenterX;
	FLOAT64 ScanCenterY;
	FLOAT64 ScanCenterZ;



};
#pragma pack(pop)

#pragma pack(push, 1)
struct S3DPointCloudDataPayloadEntry
{
	INT32S CaptureInd;
	INT32S DataIndex;
	INT32S X;
	INT32S Y;
	INT32S Z;
	INT16U Intensitiy; // [0-1.0]

	inline void SetPoint(FLOAT64 x_in, FLOAT64 y_in, FLOAT64 z_in, FLOAT32 intensity = 0)
	{
		X = x_in * ONE_OVER_POINT_CLOUD_DIST_SF;
		Y = y_in * ONE_OVER_POINT_CLOUD_DIST_SF;
		Z = z_in * ONE_OVER_POINT_CLOUD_DIST_SF;
		Intensitiy = intensity * ONE_OVER_POINT_CLOUD_DIST_SF;
	}

	inline void GetPoint(FVector& out)
	{
		out.X = X * POINT_CLOUD_DIST_SF;
		out.Y = Y * POINT_CLOUD_DIST_SF;
		out.Z = Z * POINT_CLOUD_DIST_SF;
	}
};
#pragma pack(pop)


#define MAX_3D_POINT_CLOUD_MESSAGE_RAW_DATA_SIZE (1550-10)
#define POINT_CLOUND_ENTRY_COUNT (MAX_3D_POINT_CLOUD_MESSAGE_RAW_DATA_SIZE / sizeof(S3DPointCloudDataPayloadEntry))

#pragma pack(push, 1)
struct S3DPointCloudDataPayload
{
	S3DPointCloudDataPayloadEntry Entries[POINT_CLOUND_ENTRY_COUNT];
};
#pragma pack(pop)


#pragma pack(push, 1)
struct S3DPointCloudMessage
{
public:
	INT8U MessageID;
	INT32S DataLength;

	INT8U Data[MAX_3D_POINT_CLOUD_MESSAGE_RAW_DATA_SIZE];

	void Reset()
	{
		DataLength = 0;
		//memset(this, 0, sizeof(S3DPointCloudMessage));
	}

	void SetMessageId(INT8U id)
	{
		MessageID = id;
	}

	INT8U* GetPayload()
	{
		return Data;
	}


	void SetDataLength(int len)
	{
		DataLength = len;
	}

	void SetMessage(INT32S id, INT8U* payload, INT32S payload_size)
	{
		MessageID = id;
		DataLength = payload_size;
		memcpy(Data, payload, payload_size);
	}

	INT32S GetMessageSize()
	{
		return sizeof(MessageID) + sizeof(DataLength) + DataLength;
	}

	INT32S GetDataPointCount()
	{
		return DataLength / sizeof(S3DPointCloudDataPayloadEntry);
	}


	virtual void Visualize(TArray<FVector>& cloud);

};
#pragma pack(pop)