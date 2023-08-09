#include "CHalo24SimIF.h"
using namespace Navico::Protocol;

void CHalo24IF::InitSDK()
{
	pMultiRadarClient = tMultiRadarClient::GetInstance();
}

void CHalo24IF::RadarStart()
{
}

void CHalo24IF::RadarStop()
{
}

void CHalo24IF::AdjustRange(FLOAT32 range_meter)
{
}
void CHalo24IF::OnReceivedData(INT8U* p_data, INT32U len)
{

}