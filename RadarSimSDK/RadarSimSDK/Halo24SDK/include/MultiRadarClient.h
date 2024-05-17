//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Interface for discovering available radars on a network and unlocking them.
//-----------------------------------------------------------------------------
#ifndef NAVICO_SDK_MULTIRADARCLIENT_H
#define NAVICO_SDK_MULTIRADARCLIENT_H

#include <NavTypes.h>
#include <ExportSDK.h>
using namespace std;
#include <vector>
#include <list>
#include "../../IConnection.h"
#include "../../Lib/Types/Halo24Types.h"
#define MAX_NUMBER_OF_RADARS 2
#include "ImageClient.h"
#include <TargetTrackingClient.h>
#include "../../Halo24SDK/include/PPIController.h"

using namespace Navico::Protocol::NRP;
using namespace Navico::Image;
enum EMultiRadarState
{
    Idle,
    Init,
    WaitForRadars,
    Run,
};



namespace Navico {
namespace Protocol {


 

//-----------------------------------------------------------------------------
//! \ref tMultiRadarClient callback interface for observing changes to the list
//! of available radars
//-----------------------------------------------------------------------------
class iRadarListObserver
{
public:
    virtual ~iRadarListObserver() {}

    enum eAction
    { 
        Added,     ///< A new radar has been added 
        Removed,   ///< An existing radar is being removed (only happens when ClearRadars is invoked)
        Changed,   ///< The address of an existing radar has changed (may need to reconnect)
    };

    //-----------------------------------------------------------------------------------
    //! Callback to report changes to the list of available radars.
    //! \param pSerialNumber  Unique identifier of the radar being updated in the list
    //! \param action  Why the update is occurring
    //-----------------------------------------------------------------------------------
    virtual void UpdateRadarList( const char* pSerialNumber, eAction action ) = 0;
};

//-----------------------------------------------------------------------------
//! \ref tMultiRadarClient callback interface for observing changes to a radars
//! lock state
//-----------------------------------------------------------------------------
class iUnlockStateObserver
{
public:
    virtual ~iUnlockStateObserver() {}

    //-----------------------------------------------------------------------------------
    //! Callback to report changes to the lock-state of a radar.
    //! \param pSerialNumber  Unique identifier of the Radar
    //! \param lockState  Is negative if unlocking failed, otherwise it represents the
    //!     level of unlocking
    //-----------------------------------------------------------------------------------
    virtual void UpdateUnlockState( const char* pSerialNumber, int lockState ) = 0;
}; 

//-----------------------------------------------------------------------------
//! \ref tMultiRadarClient callback interface for supplying radar unlock-keys
//-----------------------------------------------------------------------------
class iUnlockKeySupplier
{
public:
    virtual ~iUnlockKeySupplier() {}

    //-----------------------------------------------------------------------------------
    //! User supplied method to obtain the unlock-key for the specified radar.
    //! \param pSerialNumber  Radar being unlocked
    //! \param pLockID  Start of memory block containing the devices LockID
    //! \param lockIDSize  Size of the LockID pointed to by 'pLockID'
    //! \param pUnlockKey  Area into which the Unlock-Key should be placed
    //! \param maxUnlockKeySize  Maximum acceptable size of the unlock key (size of
    //!     'pUnlockKey' area)
    //! \return  Size of the Unlock-Key data placed in the 'pUnlockKey' area, or -1 if
    //!     no key was provided
    //-----------------------------------------------------------------------------------
    virtual int GetUnlockKey( const char* pSerialNumber, 
        const uint8_t* pLockID, unsigned lockIDSize, uint8_t* pUnlockKey, unsigned maxUnlockKeySize ) = 0;
}; 

//-----------------------------------------------------------------------------

#define MAX_SERIALNUMBER_SIZE  16   ///< Including the terminating nul character
#define MAX_LOCKID_SIZE       128   ///< Maximum size of a Lock-ID
#define MAX_UNLOCKKEY_SIZE    128   ///< Maximum size of an Unlock-Key

struct SRadar
{

private:
    bool IsUnlocked;

public:
    string Serial;
    uint8_t UnlockKey[MAX_LOCKID_SIZE];
    char LockId[MAX_LOCKID_SIZE];
    int UnlockKeySize;
    IConnection* pConnection;
 
    tImageClient* pImageClient = nullptr;
    tTargetTrackingClient* pTargetTrackingClient = nullptr;
    tPPIController* pPPIController = nullptr;

    SRadar()
    {
        Serial = "";
        IsUnlocked = false;
        UnlockKeySize = 0;
    }

    bool  SetUnlockKey(const uint8_t* p_key, int key_size)
    {
        if (key_size <= MAX_LOCKID_SIZE) {
            memcpy(UnlockKey, p_key, key_size);
            UnlockKeySize = key_size;
            return true;
        }
        else {
            return false;
        }

    }

    void SetIsUnlocked(bool val)
    {
        IsUnlocked = val;
    }
    bool GetIsUnlocked()
    {
        return IsUnlocked;
    }

    bool GetIsImageStreamConnected(int stream_no);

    template<typename T>
    void SendPacket(ESimSDKDataIDS id, T* p_data) {
        SRadarSimSDKPacket pack;
        pack.PrepareMessage<T>(id, p_data);
        pConnection->SendData((INT8U*)&pack, pack.GetTransmitSize(), pConnection->GetRemotePort());
    }

    template<typename T>
    void SendPacket(ESimSDKDataIDS id, char*p_serial, T* p_data) {
        SRadarSimSDKPacket pack;
        SSerialData* p_serial_data = (SSerialData*)p_data;

        strcpy((char*)p_serial_data->SerialKey, p_serial);
        p_serial_data->SerialLen = strlen(p_serial);

        pack.PrepareMessage<T>(id, p_data);
        pConnection->SendData((INT8U*)&pack, pack.GetTransmitSize(), pConnection->GetRemotePort());
    }


   
};


class tMultiRadarClientImpl;

//-----------------------------------------------------------------------------
//! Class for discovering and unlocking radars attached to a network.
//-----------------------------------------------------------------------------
class  tMultiRadarClient
{
public:
    ~tMultiRadarClient();

    //-----------------------------------------------------------------------------------
    //! Use this method to create and get access to the single instance of this singleton
    //-----------------------------------------------------------------------------------
    static tMultiRadarClient* GetInstance();

    //-----------------------------------------------------------------------------------
    //! Connects to the network and starts this objects internal threads. Once connected
    //! it will listen to the service broadcasts of other devices and build up a data-base
    //! of the devices and their supported services.
    //! \return  0 if successful, or one of the Protocol::eErrors (ClientErrors.h) enums
    //!     if failed 
    //-----------------------------------------------------------------------------------
    int Connect();

    //-----------------------------------------------------------------------------------
    //! Stop this objects threads and disconnect from network. This performs a civilised
    //! shutdown by notifying the thread and waiting until it terminates. 
    //! \return  False if the stop was unnecessary, true otherwise
    //-----------------------------------------------------------------------------------
    bool Disconnect();

    //-----------------------------------------------------------------------------------
    //! Add an observer for any changes to the list of known radars
    //! \param pObserver Pointer to the new observer to be added
    //-----------------------------------------------------------------------------------
    void AddRadarListObserver( iRadarListObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Remove an existing observer for changes to the list of known radars
    //! \param pObserver Pointer to the existing observer to be removed
    //-----------------------------------------------------------------------------------
    void RemoveRadarListObserver( iRadarListObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Add an observer for changes to the unlocked state of any radars
    //! \param pObserver Pointer to the new observer to be added
    //-----------------------------------------------------------------------------------
    void AddUnlockStateObserver( iUnlockStateObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Remove an existing observer for changes to the unlocked state of any radars
    //! \param pObserver Pointer to the existing observer to be removed
    //-----------------------------------------------------------------------------------
    void RemoveUnlockStateObserver( iUnlockStateObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Provide an implementation of the 'iUnlockKeySupplier' class that can be used to
    //! get an unlock-key for a particular radar. Any successful unlock-key provided will
    //! be retained/cached until the 'ClearUnlockKeys' method is called.
    //! \param pSupplier  Pointer to the new unlock-key supplier
    //-----------------------------------------------------------------------------------
    void SetUnlockKeySupplier( iUnlockKeySupplier * pSupplier );

    //-----------------------------------------------------------------------------------
    //! Set the unlock-key to be used for unlocking the specified radar. These cached
    //! unlock keys maybe removed by calling the 'ClearUnlockKeys' method. This method will
    //! also automatically initiate an unlock sequence with radar.
    //! \param pSerialNumber  Unique identifier for the radar
    //! \param pUnlockKey     Pointer to the start of the unlock-key
    //! \param unlockKeySize  Size of the unlock key
    //! \return  0 if the unlock-key was accepted, or an Protocol::eErrors value otherwise 
    //-----------------------------------------------------------------------------------
    int SetUnlockKey( const char * pSerialNumber, const uint8_t * pUnlockKey, unsigned unlockKeySize );

    //-----------------------------------------------------------------------------------
    //! Removes all cached unlock-keys
    //-----------------------------------------------------------------------------------
    void ClearUnlockKeys();

    //-----------------------------------------------------------------------------------
    //! Default/reasonable timeout period for the completion of an 'UnlockRadar' method
    //! call (see below)
    //-----------------------------------------------------------------------------------
    NavClassConst( cDefaultUnlockPeriod, uint32_t, 500 );

    //-----------------------------------------------------------------------------------
    //! Initiates, and optionally waits for the completion of, the process of unlocking a
    //! single radar. It assumes the unlock-key has already been provided, or will be 
    //! provided by an iUnlockKeySupplier.
    //! \param pSerialNumber  Unique identifier for the radar (if this is NULL and wait_ms
    //!     is 0, then a query will be broadcast to all radars for their lock-IDs)
    //! \param wait_ms  Maximum number of milli-seconds to wait for the unlock to finish.
    //!     If 'wait_ms' is zero the call is non-blocking and will return immediately
    //!     (see 'AddUnlockStateObserver' for detection of when the operation finishes).
    //!     Otherwise the method will block for up to the number of milli-seconds specified; 
    //!     'cDefaultUnlockPeriod should be a reasonable default for this)
    //! \return  The lock-level (>= 0, 0 meaning its locked) if successful, otherwise one
    //!     of the Protocol::eErrors enum values
    //-----------------------------------------------------------------------------------
    int UnlockRadar( const char * pSerialNumber, uint32_t wait_ms = 0 );

    //-----------------------------------------------------------------------------------
    //! Initiates, and optionally waits for the completion of, the process of unlocking a
    //! single radar.
    //! \param pSerialNumber  Unique identifier for the radar
    //! \param pUnlockKey  Pointer to the start of the unlock-key
    //! \param unlockKeySize  Size of the unlock key
    //! \param wait_ms  Maximum number of milli-seconds to wait for the unlock to finish.
    //!     If 'wait_ms' is zero the call is non-blocking and will return immediately
    //!     (see 'AddUnlockStateObserver' for detection of when the operation finishes).
    //!     Otherwise the method will block for up to the number of milli-seconds specified; 
    //!     'cDefaultUnlockPeriod should be a reasonable default for this)
    //! \return  The lock-level (>= 0, 0 meaning its locked) if successful, otherwise one
    //!     of the Protocol::eErrors enum values
    //-----------------------------------------------------------------------------------
    int UnlockRadar( const char * pSerialNumber, const uint8_t * pUnlockKey, unsigned unlockKeySize, uint32_t wait_ms = 0 );

    //-----------------------------------------------------------------------------------
    //! Fills 'radars' with a list of all the radars that have announced themselves on
    //! the network (see 'SendQuery' method). As many radar serial-numbers as possible
    //! will be placed into 'radars'. By specifying a value of 0 for 'maxRadars' you may
    //! find out the number of radar serial-numbers available without actually getting
    //! any.
    //! \param radars  Location in which to return the radar serial-numbers
    //! \param maxRadars  The maximum number of serial-numbers 'radars' is capable of
    //!      holding
    //! \return  Total number of radars known about, not just the number added to 'radars'
    //-----------------------------------------------------------------------------------
    unsigned GetRadars( char radars[][ MAX_SERIALNUMBER_SIZE ], unsigned maxRadars );

    //-----------------------------------------------------------------------------------
    //! Fills 'lockID' with the lock ID of the radar of specified serial number in ASCII.
    //! \param lockID  Location in which to return the lock ID of the radar. It must be a
    //!      character array of size of at least MAX_LOCKID_SIZE * 2.
    //! \param pSerialNumber  Unique identifier for the radar
    //! \return  Number of bytes in lockID, or 0 if the specified radar is not found.
    //-----------------------------------------------------------------------------------
    int GetLockID( char lockID[MAX_LOCKID_SIZE * 2], const char* pSerialNumber );

    //-----------------------------------------------------------------------------------
    //! Removes all existing radars from the list; you will need to invoke 'SendQuery' to
    //! recreate an up to date list of available radars
    //-----------------------------------------------------------------------------------
    void ClearRadars();

    //-----------------------------------------------------------------------------------
    //! Broadcasts a query for all attached radars to announce their presence.
    //! \return  True if the send was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QueryRadars();

    //-----------------------------------------------------------------------------------
    //! Gets the number of image-streams (virtual-radars) provided by the specified radar.
    //! \param pSerialNumber  Unique identifier for the radar
    //! \return  The number of image-streams supported. 1 or greater if the method succeeds,
    //!     or a Protocol::eErrors enum value otherwise
    //-----------------------------------------------------------------------------------
    int GetImageStreamCount( const char * pSerialNumber );

    //-----------------------------------------------------------------------------------
    //! Enumeration for the different types of services supported
    //-----------------------------------------------------------------------------------
    enum eService
    {
        Service_Image,
        Service_TargetTracking,

        Service_END   ///< End of valid Service enumerations
    };

    //-----------------------------------------------------------------------------------
    //! Returns whether a particular service is provided by a particular virtual-radar
    //! instance. Note by definition all image-streams should support the image service.
    //! \param pSerialNumber  Unique identifier for the radar
    //! \param service  The service to check support for
    //! \param imageStream  The stream to check support for the service on
    //! \return  1 if the service is supported, 0 if its not, or a Protocol::eErrors
    //!     enum value otherwise.
    //-----------------------------------------------------------------------------------
    int IsServiceSupported( const char * pSerialNumber, eService service, unsigned imageStream = 0 );

    //-----------------------------------------------------------------------------------
    //! Broadcasts a command to force all radars to renegotiate their unique device IDs,
    //! the next time they are restarted or disconnected & reconnected to the ethernet.
    //! \return  True if the send was successfully initiated
    //-----------------------------------------------------------------------------------
    bool ResetDeviceIDs();







/// <summary>
/// Added by Hypernova
/// </summary>
  
private:
    //-----------------------------------------------------------------------------------
    static tMultiRadarClient* pInstance;
    tMultiRadarClient();   // its a singleton, don't allow anyone else to create one of these

    tMultiRadarClient( const tMultiRadarClient & );
    tMultiRadarClient & operator=( const tMultiRadarClient & );

    ////tMultiRadarClientImpl *  m_pImpl;

    list<iRadarListObserver*> RadarListObservers;
    list<iUnlockStateObserver*> UnlockStateObserver;
    iUnlockKeySupplier* pUnlockKeySupplier;

    vector<SRadar*> Radars;


    void StateMachine();
    
    EMultiRadarState MultiRadarState = EMultiRadarState::Idle;

public:
    void ExternalUpdate();
    void RegisterRadar(IConnection* p_conn, const char *serial_number);
    SRadar* FindRadar(const char* serial_number);
    SRadar* GetRadar(INT32U ind);
    SRadar* FindRadarFromConnection(IConnection* p_conn);
    void HandleReponse(IConnection* p_conn, SCommandAckNackResponse* p_res);
    void HandleRadarState(IConnection* p_conn, SRadarState* p_res);
    void HandleRadarSetup(IConnection* p_conn, SRadarSetupPayload* p_res);
    void HandleSpoke(IConnection* p_conn, SHalo24SpokePayload* p_res);
    void HandleTrackingStatus(IConnection* p_conn, STrackingTargetStatusPayload* p_res);

};

//-----------------------------------------------------------------------------

} // Protocol
} // Navico

#endif  // include guard
