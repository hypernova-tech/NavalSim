#pragma once
#include "Lib/Types/Primitives.h"
#include "IDataIF.h"

#include "Halo24SDK/include/MultiRadarClient.h"
#include <MultiRadarClient.h>
#include "IConnection.h"
#include <list>

using namespace Navico::Protocol;

struct SHaloEntry{
	IConnection* pConnection;

	
};

class CHalo24IF:public IDataIF
{

	

public:
	CHalo24IF();
	void AddSDK(IConnection *p_conn);
	void RadarStart();
	void RadarStop();
	void AdjustRange(FLOAT32 range_meter);

	virtual void OnReceivedData(void* p_if, INT8U* p_data, INT32U len) override;

 
private:
	list<SHaloEntry*> Entries;
	tMultiRadarClient* pMultiRadarClient;
	
	SHaloEntry* FindEntry(IConnection* p_con);
	SRadarSimSDKPacket* pRes;
};

/*

SVI-Radar SDK kullanılan özellikler
Kullanılan radarın 2 kanalı (A-B) vardır. Aşağıdaki ayarlamalar her iki radar için de ayrı ayrı yapılmaktadır.
-Radarı açma/kapama
-Range ayarlama
-Gain ayarlama
-Sea Clutter ayarlama
-Rain Clutter ayarlama
-Sector Blanking: belirlenen açılar arasından gelen radar datasını silme
-Radar direction ayarları (Head-up/ North up/ Course up)
-Target tracking başlatma
-Target boost: hedefi daha iyi vurgulamak için genişletme ayarı
-Target özellikleri okuma (Target Count ve her target için ayrı ayrı target-cpa, target-tcpa, target distance, target latitude-longitude-heading, target id, target status(dangerous-safe),target speed, target course)
-2 adet Guard zone için ayarlamalar (GZ sensitivity, GZ enabled, GZ alarm type, GZ start-end range, GZ bearing, GZ width, GZ alarm cancel)
-Radar interference rejection
-Radar local interference rejection filter
-Radar scan speed ayarı




*/