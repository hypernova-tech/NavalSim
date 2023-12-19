#include "PPIController.h"
#include "../../Lib/Types/Primitives.h"
#include <RadarColourLookUpTable.h>


Navico::Image::tPPIController::tPPIController()
{
}

Navico::Image::tPPIController::~tPPIController()
{
}

void Navico::Image::tPPIController::SetFrameBuffer(intptr_t pBuffer, uint32_t width, uint32_t height, tRadarColourLookUpTable* pLookUpTable, uint32_t xCentre, uint32_t yCentre)
{
	pFrame = (uint8_t*)pBuffer;
	Width = width;
	Height = height;
	pColorLookUpTable = pLookUpTable;
	XCentre = xCentre;
	YCentre = yCentre;
}

void Navico::Image::tPPIController::SetColourLookUpTable(tRadarColourLookUpTable* pLookUpTable)
{
	pColorLookUpTable = pLookUpTable;
}

void Navico::Image::tPPIController::Process(const Protocol::NRP::Spoke::t9174Spoke* pSpoke)
{
	// decode spoke azimuth;
	int spoke_azimuth = pSpoke->header.spokeAzimuth;

	double spoke_ang = spoke_azimuth * 360.0 / 4096;

	

	for (int i = 0; i < pSpoke->header.nOfSamples; i++) {
		int byte_ind = i / 2;
		int order = i & 0x1;

		unsigned char merged = pSpoke->data[byte_ind];
		unsigned char data = 0;
		
		if (order) {
			data = merged & 0xF;
		}
		else {
			data = (merged >> 4) & 0xf;
		}

	
		
		double distance = i * pSpoke->header.rangeCellSize_mm * 1000;

		double x = sin(spoke_azimuth * DEGTORAD)* distance;
		double y = cos(spoke_azimuth * DEGTORAD) * distance;

		int px = (int)(x / MetersPerPixel);
		int py = (int)(y / MetersPerPixel);

		px = Clamp(px, -1023, 1023);
		py = Clamp(py, -1023, 1023);

		// center to left top

		int addr = (1024 + px) * 1024 + (py - 1024) * sizeof(tColor);

		tColor* p_color = (tColor*)pFrame;

		if (data != 0) {
			
			p_color[addr] = 0x000000FF; // green
		}
		else {
			pFrame[addr] = 0;
		}
	}


}
void Navico::Image::tPPIController::ResetSpokeLineAtImage(int spoke_number, int spoke_count)
{

}
int Navico::Image::tPPIController::Clamp(int val, int min, int max)
{
	if (val > max) {
		return max;
	}
	if (val < min) {
		return min;
	}
	return val;
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
	MetersPerPixel = metersPerPixel;
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


