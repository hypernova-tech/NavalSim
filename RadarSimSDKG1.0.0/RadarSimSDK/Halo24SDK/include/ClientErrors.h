//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Definitions of NRP Protocol client return error codes.
//-----------------------------------------------------------------------------
#ifndef NAVICO_PROTOCOL_CLIENTERRORS_H
#define NAVICO_PROTOCOL_CLIENTERRORS_H


namespace Navico {
namespace Protocol {

//-----------------------------------------------------------------------------
//! Complete set of library specific error return codes. Certain library
//! methods will be documented to return a sub-set of these errors.
//-----------------------------------------------------------------------------

enum eErrors
{
    EOK              =  0,    ///< The meaning of this and higher levels actually depends on the method called
    ELocked          = -1,    ///< Radar has not been successfully unlocked
    EPending         = -2,    ///< Operation initiated, completion now pending
    ETimedOut        = -3,    ///< Operation timed-out
    EBusy            = -4,    ///< Same operation is already in progress and only one can be performed at a time
    EBadSerialNumber = -5,    ///< Invalid serial-number provided
    ENoUnlockKey     = -6,    ///< No unlock-key provided
    EBadUnlockKey    = -7,    ///< Invalid unlock-key provided
    EWrongUnlockKey  = -8,    ///< Wrong unlock-key provided for specified radar
    ENotRunning      = -9,    ///< Client has been Stopped
    EUnknownRadar    = -10,   ///< Radar has not announced itself to the tMultiRadarClient (requery the radar)
    ENonStdAddress   = -11,   ///< Radar is not at the standard NRP-1.0 protocol address as required for its unlock level
    ECommsFailure    = -12,   ///< Communications failure (problems opening sockets, etc)
    EThreadCreation  = -13,   ///< Failed to create required threads
    EBadParameter    = -14,   ///< One of the parameters is invalid, ie. lies outside its valid range
    EUnused          = -15,   ///< End of standardised errors
    EBadUnlockLevel  = -16,   ///< The unlock level is too low
};


//-----------------------------------------------------------------------------

} // Protocol
} // Navico

#endif  // include guard
