#include "PPIController.h"

Navico::Image::tPPIController::tPPIController()
{
}

Navico::Image::tPPIController::~tPPIController()
{
}

void Navico::Image::tPPIController::SetFrameBuffer(intptr_t pBuffer, uint32_t width, uint32_t height, tRadarColourLookUpTable* pLookUpTable, uint32_t xCentre, uint32_t yCentre)
{
}

void Navico::Image::tPPIController::SetColourLookUpTable(tRadarColourLookUpTable* pLookUpTable)
{
}

void Navico::Image::tPPIController::Process(const Protocol::NRP::Spoke::t9174Spoke* pSpoke)
{
}

void Navico::Image::tPPIController::SetAzimuthInterpolation(tPPIAzimuthInterpolation azimuthInterp)
{
}

Navico::Image::tPPIAzimuthInterpolation Navico::Image::tPPIController::GetAzimuthInterpolation() const
{
	return tPPIAzimuthInterpolation();
}

void Navico::Image::tPPIController::SetRangeInterpolation(tPPIRangeInterpolation rangeInterp)
{
}

Navico::Image::tPPIRangeInterpolation Navico::Image::tPPIController::GetRangeInterpolation() const
{
	return tPPIRangeInterpolation();
}

void Navico::Image::tPPIController::SetRangeResolution(float metersPerPixel)
{
}

float Navico::Image::tPPIController::GetRangeResolution() const
{
	return 0.0f;
}

bool Navico::Image::tPPIController::GetTrailsOn()
{
	return false;
}

void Navico::Image::tPPIController::SetTrailsTime(int32_t time_sec)
{
}

int32_t Navico::Image::tPPIController::GetTrailsTime()
{
	return 0;
}

void Navico::Image::tPPIController::GetVersion(uint32_t& major, uint32_t& minor, uint32_t& build)
{
}
