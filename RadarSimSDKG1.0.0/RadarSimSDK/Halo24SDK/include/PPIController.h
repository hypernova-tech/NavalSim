//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Class capable of performing scan conversion
//-----------------------------------------------------------------------------
#ifndef NAVICO_IMAGE_TPPICONTROLLER_H
#define NAVICO_IMAGE_TPPICONTROLLER_H

#include "NavTypes.h"
#include "ExportSDK.h"
#include "NavRadarSpoke.h"


namespace Navico {
    class NAVICO_SDK_EXPORT tRadarColourLookUpTable;

namespace Image {
    class NAVICO_SDK_EXPORT tPPIControllerImpl;

//---------------------------------------------------------------------------
//! Different kinds of PPI range interpolation
//---------------------------------------------------------------------------
    enum tPPIRangeInterpolation
    {
        eRangeInterpolationNone,   ///< No interpolation done
        eRangeInterpolationPeak,   ///< peak preservation and possibility to use the range resolution
    };

//---------------------------------------------------------------------------
//! Different kinds of PPI azimuth interpolation
//---------------------------------------------------------------------------
enum tPPIAzimuthInterpolation
{
    eAzimuthInterpolationNone,     ///< No azimuth interpolation
    eAzimuthInterpolationRepeat,   ///< Repetition of the spoke in azimuth if azimuth hole is present
};


//---------------------------------------------------------------------------
//! Class for converting polar image data vectors (radar spokes) into a 
//! single cartesian image
//---------------------------------------------------------------------------
class NAVICO_SDK_EXPORT tPPIController
{
public:
    tPPIController();
    ~tPPIController();

    //-------------------------------------------------------------------------
    //! Function that is used to set information on where image will be drawn
    //! \param pBuffer  Radar frame buffer where spoke will be drawn
    //! \param width  Width of the memory frame buffer
    //! \param height  Height of the memory frame buffer
    //! \param pLookUpTable  Pointer to the colour look up table
    //! \param xCentre  X-centre coordinate of where the PPI will be drawn 
    //!     (default Width/2)
    //! \param yCentre  Y-centre coordinate of where the PPI will be drawn 
    //!     (default Height/2)
    //-------------------------------------------------------------------------
    void SetFrameBuffer( intptr_t pBuffer, uint32_t width, uint32_t height, tRadarColourLookUpTable *pLookUpTable, uint32_t xCentre, uint32_t yCentre );

    //-------------------------------------------------------------------------
    //! Function that is used to set the new colour map
    //! \param pLookUpTable pointer to the colour look up table
    //-------------------------------------------------------------------------
    void SetColourLookUpTable( tRadarColourLookUpTable *pLookUpTable );

    //-------------------------------------------------------------------------
    //! Function that is used to add a spoke that will be drawn using the PPI
    //! representation
    //! \param pSpoke Spoke will be added to the PPI
    //-------------------------------------------------------------------------
    void Process( const Protocol::NRP::Spoke::t9174Spoke *pSpoke );

    //-------------------------------------------------------------------------
    //! Enable different kind of azimuth interpolation
    //! \param azimuthInterp It can be eAzimuthInterpolationNone (doesn't perform
    //! any azimuth interpolation and draws holes if huge azimuth are missing) or
    //! eAzimuthInterpolationRepeat (it repeats last spoke to fill the azimuth gap)
    //-------------------------------------------------------------------------
    void SetAzimuthInterpolation( tPPIAzimuthInterpolation azimuthInterp );

    //-------------------------------------------------------------------------
    //! Return the type of azimuth interpolation used.
    //! \return It can be eAzimuthInterpolationNone (doesn't perform
    //! any azimuth interpolation and draws holes if huge azimuth are missing) or
    //! eAzimuthInterpolationRepeat (it repeats last spoke to fill the azimuth gap)
    //-------------------------------------------------------------------------
    tPPIAzimuthInterpolation GetAzimuthInterpolation() const;

    //-------------------------------------------------------------------------
    //! Enable different kind of range interpolation used.
    //! \param rangeInterp It can be eRangeInterpolationNone (doesn't perform
    //! any range interpolation and each single spoke sample is mapped as pixel)
    //! or eRangeInterpolationPeak (it uses the range resolution value to express
    //! how many meters represents each single pixel)
    //-------------------------------------------------------------------------
    void SetRangeInterpolation( tPPIRangeInterpolation rangeInterp );

    //-------------------------------------------------------------------------
    //! Return the type of range interpolation used.
    //! \return It can be eRangeInterpolationNone (doesn't perform
    //! any range interpolation and each pixel is mapped to a single spoke sample)
    //! or eRangeInterpolationPeak (it uses the range resolution value to express
    //! how many meters represents each single pixel)
    //-------------------------------------------------------------------------
    tPPIRangeInterpolation GetRangeInterpolation() const;

    //-------------------------------------------------------------------------
    //! Sets the range resolution as meters per pixels (how many meters will be
    //! mapped for a pixel)
    //! \param metersPerPixel The new range resolution
    //-------------------------------------------------------------------------
    void SetRangeResolution( float metersPerPixel );

    //-------------------------------------------------------------------------
    //! Returns the current range resolution
    //! \return It returns the range resolution expressed as meters per pixel
    //-------------------------------------------------------------------------
    float GetRangeResolution() const;

    //-------------------------------------------------------------------------
    //! Returns if target trails are on
    //! \return The state of trails
    //-------------------------------------------------------------------------
    bool GetTrailsOn();

    //-------------------------------------------------------------------------
    //! Sets the trails time.
    //! \param time_sec The time expressed in seconds: 0 = off, -1 = infinite,
    //! otherwise the time expressed in seconds
    //-------------------------------------------------------------------------
    void SetTrailsTime( int32_t time_sec );

    //-------------------------------------------------------------------------
    //! Return the trails time.
    //! \return The time expressed in seconds: 0 = off, -1 = infinite,
    //! otherwise the time expressed in seconds
    //-------------------------------------------------------------------------
    int32_t GetTrailsTime();

    //-------------------------------------------------------------------------
    //! Gets the library version: major.minor.build
    //! \param major The major number of the version library
    //! \param minor The minor number of the version library
    //! \param build The build number of the version library
    //-------------------------------------------------------------------------
    void GetVersion( uint32_t &major, uint32_t &minor, uint32_t &build );

private:
    tPPIControllerImpl *m_pImpl;
};

//-----------------------------------------------------------------------------

} // namespace Image
} // namespace Navico

#endif  // include guard



