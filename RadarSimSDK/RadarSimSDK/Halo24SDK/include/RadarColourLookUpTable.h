//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Base class for representing colour lookup tables
//-----------------------------------------------------------------------------
#ifndef _RADARCOLOURLOOKUPTABLE_H_
#define _RADARCOLOURLOOKUPTABLE_H_

#include "NavTypes.h"
#include <ExportSDK.h>

#include <cassert>


namespace Navico {

//-----------------------------------------------------------------------------
//! Definition of Color
//-----------------------------------------------------------------------------
typedef uint32_t tColor;

inline tColor FromARGB( uint8_t a, uint8_t r, uint8_t g, uint8_t b ) { return ((((uint32_t)a) << 24) | (((uint32_t)r) <<16 ) | (((uint32_t)g) << 8) | ((uint32_t)b)); }
inline tColor FromBGRA( uint8_t b, uint8_t g, uint8_t r, uint8_t a ) { return ((((uint32_t)b) << 24) | (((uint32_t)g) <<16 ) | (((uint32_t)r) << 8) | ((uint32_t)a)); }

//-----------------------------------------------------------------------------
// tRadarColourLookUpTable Class
//-----------------------------------------------------------------------------

// Dummy class for colour look up table
class NAVICO_SDK_EXPORT tRadarColourLookUpTable
{
private:
    const tColor *m_pColourLookup;

public:
    tRadarColourLookUpTable() : m_pColourLookup( nullptr ) { }

    //-------------------------------------------------------------------------
    //! Gets a colour at a particular index.
    //! Note: no check is done on boundaries of the vector, to speed things up!!!
    //!       Normally we have 16 colours.
    //! \return the color value in alpha-red-green-blue value
    //-------------------------------------------------------------------------
    inline tColor GetColour( uint32_t index ) const { assert( index < 256 ); return m_pColourLookup[index]; }

protected:
    inline void SetLookUpTablePointer( const tColor *pLookUpTable ) { m_pColourLookup = pLookUpTable; }
};

//-----------------------------------------------------------------------------
// Green colour look up table
//-----------------------------------------------------------------------------

class NAVICO_SDK_EXPORT tRadarColourLookUpTableGreen : public tRadarColourLookUpTable
{
public:
    tRadarColourLookUpTableGreen()
    {
    };
};

//-----------------------------------------------------------------------------
// Navico colour look-up tables
//-----------------------------------------------------------------------------

class NAVICO_SDK_EXPORT tRadarColourLookUpTableNavico : public tRadarColourLookUpTable
{
public:
    tRadarColourLookUpTableNavico(){}
};

class NAVICO_SDK_EXPORT tRadarColourLookUpTableNavicoDoppler : public tRadarColourLookUpTable
{
public:
    tRadarColourLookUpTableNavicoDoppler(){}
};

//-----------------------------------------------------------------------------
// Gray colour look-up tables
//-----------------------------------------------------------------------------

class NAVICO_SDK_EXPORT tRadarColourLookUpTableGray : public tRadarColourLookUpTable
{
public:
    tRadarColourLookUpTableGray();
};

class NAVICO_SDK_EXPORT tRadarColourLookUpTableGrayDoppler : public tRadarColourLookUpTable
{
public:
    tRadarColourLookUpTableGrayDoppler();
};

//-----------------------------------------------------------------------------

} // namespace Navico 

#endif // include guard
