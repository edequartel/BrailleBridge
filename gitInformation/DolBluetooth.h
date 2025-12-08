////////////////////////////////////////////////////////////////////////////////
//
// Project      : Dolphin Foldable Keyboard Driver
// File Name    : DolBluetooth.h
// Description  : Bluetooth comms routines
//
// Copyright (C) 2004 Dolphin Oceanic Ltd.
//
////////////////////////////////////////////////////////////////////////////////
// Version History:
//
// $Log: /Library/DolBluetooth.h $
// 
// 15    26/10/07 10:01 Druck
// Bond wait semaphore replaced with waiting on thread handle
// 
// 14    12/09/06 11:52 Druck
// Comment corrections
// 
// 13    8/09/06 12:56 Druck
// Power monitoring thread added to Mocrosoft implementation, allowing
// stack state messages to be simulated
// 
// 12    16/05/06 10:18 Druck
// Changes necessary to integrate BTW-CE SDK in to Visual Studio 2005 -
// BT_CHAR redefined from WCHAR to UINT16 in DolBluetooth.h and recast to
// WCHAR when used, so independent of wchar_t as built in type setting.
// 
// 11    13/04/06 11:57 Druck
// BTCHAR type changed from TCHAR to UNIT16 for VS2005 compatibility
// 
// 10    15/03/06 10:18 Druck
// Dependencies on Widcomm Bluetooth SDK header files removed
// 
// 9     24/02/06 10:40 Mike
// Changes for VS2005 compatibility
// 
// 8     5/07/05 15:57 Druck
// DolBluetooth split in to Widcomm and Microsoft specific DLLs
// 
// 7     24/06/05 17:02 Druck
// Microsoft stack code mergered in (requires Widcomm DLLs)
// 
// 6     18/05/05 10:27 Druck
// DolBtBond code integrated in to class structure to allow multiple
// instantiation
// 
// 5     11/05/05 16:10 Mike
// 
// 4     10/05/05 9:11 Druck
// Stack type added
// 
// 3     22/04/05 9:23 Druck
// Service discovery methods added
// 
// 2     6/04/05 14:26 Druck
// Stack Status Changed event added
// Message literals renamed
// 
// 1     24/03/05 14:56 Druck
// Initial revision
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <winsock2.h>
#include <ws2bth.h>

/////////////////////////////////////////////////////////////////////////////
// literals

#define DOL_BLUETOOTH					// used to enable Bluetooth code

#ifdef  DOL_BLUETOOTH_DLL
#define DECL_DOL_BLUETOOTH __declspec(dllexport)
#else
#define DECL_DOL_BLUETOOTH __declspec(dllimport)
#endif

#define	DOLBT_CONNECT_TIMEOUT	2000	// connection timeout

// custom messages for bluetooth actions
#define WM_DOLBT_STACKSTATUSCHANGE (WM_USER + 0x100)
#define WM_DOLBT_DEVICEFOUND       (WM_USER + 0x101)
#define WM_DOLBT_INQUIRYCOMPLETE   (WM_USER + 0x102)
#define WM_DOLBT_DISCOVERYCOMPLETE (WM_USER + 0x103)
#define WM_DOLBT_EVENTRECEIVED     (WM_USER + 0x104)
#define WM_DOLBT_DATARECEIVED      (WM_USER + 0x105)

/////////////////////////////////////////////////////////////////////////////
// definitions from BtIfClasses.h and BtIfDefinitions.h

#ifndef _BTIFCLASSES_H 

// must define BT_CHAR as other than WCHAR to prevent linking problems in VS2005
#ifdef _WIN32_WCE
    #define BT_CHAR UINT16
#else 
    #define BT_CHAR char
#endif // _WIN32_WCE

#define BD_ADDR_LEN     6             		// Device address length
typedef UINT8 BD_ADDR[BD_ADDR_LEN];			// Device address
typedef UINT8 *BD_ADDR_PTR;					// Pointer to Device Address

#define BD_NAME_LEN     248
typedef UINT8 BD_NAME[BD_NAME_LEN];         // Device name

#define PIN_CODE_LEN    16

#define DEV_CLASS_LEN   3
typedef UINT8 DEV_CLASS[DEV_CLASS_LEN];     // Device class

#define PORT_EV_RXCHAR		0x00000001
#define PORT_EV_RXFLAG		0x00000002
#define PORT_EV_TXEMPTY		0x00000004
#define PORT_EV_CTS			0x00000008
#define PORT_EV_DSR			0x00000010
#define PORT_EV_RLSD		0x00000020
#define PORT_EV_BREAK		0x00000040
#define PORT_EV_ERR			0x00000080
#define PORT_EV_RING		0x00000100
#define PORT_EV_CTSS		0x00000400
#define PORT_EV_DSRS		0x00000800
#define PORT_EV_RLSDS		0x00001000
#define PORT_EV_OVERRUN		0x00002000
#define PORT_EV_TXCHAR		0x00004000
#define PORT_EV_CONNECTED   0x00000200
#define PORT_EV_CONNECT_ERR 0x00008000
#define PORT_EV_FC		    0x00010000
#define PORT_EV_FCS			0x00020000

#define MAJOR_DEV_CLASS_MASK	0x1F
#define MINOR_DEV_CLASS_MASK	0xFC

class DECL_DOL_BLUETOOTH CSdpDiscoveryRec;

#endif // _BTIFCLASSES_H

#ifndef _BTIFDEFINITIONS_H

typedef struct
{
    UINT32   bIsConnected;
    INT32    Rssi;
    UINT32   BytesSent;
    UINT32   BytesRcvd;
    UINT32   Duration;
} tBT_CONN_STATS;

#endif // _BTIFDEFINITIONS_H

/////////////////////////////////////////////////////////////////////////////
// class CBtAddress
/////////////////////////////////////////////////////////////////////////////
// encapsulates a bluetooth address

class DECL_DOL_BLUETOOTH CBtAddress 
{
public:
	BD_ADDR		m_Address;

public:
	// constructors
	CBtAddress()
	{
		Clear();
	}

	CBtAddress(BD_ADDR_PTR address)
	{
		memcpy(m_Address, address, BD_ADDR_LEN);
	}

	CBtAddress(const bt_addr& address)
	{
		m_Address[0] = (BYTE)(address >> 40);
		m_Address[1] = (BYTE)(address >> 32);
		m_Address[2] = (BYTE)(address >> 24);
		m_Address[3] = (BYTE)(address >> 16);
		m_Address[4] = (BYTE)(address >>  8);
		m_Address[5] = (BYTE)(address >>  0);
	}

	CBtAddress(const CBtAddress& address)
	{
		memcpy(m_Address, address.m_Address, BD_ADDR_LEN);
	}

	// virtual distructor
	virtual ~CBtAddress() {};

	// assigment operators
	const CBtAddress& operator=(const BD_ADDR_PTR& address)
	{
		memcpy(m_Address,  address,  BD_ADDR_LEN);
		return *this;
	}

	const CBtAddress& operator=(const CBtAddress& address)
	{
		memcpy(m_Address,  address.m_Address,  BD_ADDR_LEN);
		return *this;
	}

	// comparision operators
	bool operator==(const CBtAddress& address)
	{
		return memcmp(m_Address, address.m_Address, BD_ADDR_LEN)==0;
	}

	bool operator!=(const CBtAddress& address)
	{
		return memcmp(m_Address, address.m_Address, BD_ADDR_LEN)!=0;
	}

	// casting operators
	operator BYTE*()
	{
		return (BYTE*)m_Address;
	}

	operator BD_ADDR*()
	{
		return &m_Address;
	}

	operator bt_addr()
	{
		bt_addr bta = ((bt_addr)m_Address[0] << 40)
					| ((bt_addr)m_Address[1] << 32)
					| ((bt_addr)m_Address[2] << 24)
					| ((bt_addr)m_Address[3] << 16)
					| ((bt_addr)m_Address[4] <<  8)
					| ((bt_addr)m_Address[5] <<  0);

		return bta;
	}

	operator const BT_CHAR*()
	{
		static BT_CHAR address[18];

		wsprintf((WCHAR*)address, L"%02X:%02X:%02X:%02X:%02X:%02X",
						m_Address[0],
						m_Address[1],
						m_Address[2],
						m_Address[3],
						m_Address[4],
						m_Address[5]);
		return address;
	}

	// methods
	void Clear()
	{
		memset(m_Address, 0, BD_ADDR_LEN);
	}

	bool IsValid()
	{
		CBtAddress empty;
		return *this!=empty;
	}

	int GetSize()
	{
		return BD_ADDR_LEN;
	}
};

/////////////////////////////////////////////////////////////////////////////
// class CBtStack
/////////////////////////////////////////////////////////////////////////////
// Main stack interface class, generates WM_DEVICEFOUNDMESSAGE messages

class DECL_DOL_BLUETOOTH CBtStack
{
public:
	typedef enum
	{
		stack_NONE,
		stack_WIDCOMM,
		stack_MICROSOFT
	} BtStackType;

    typedef enum
    {
        SUCCESS,
        ALREADY_BONDED,
        BAD_PARAMETER,
        FAIL
    } BOND_RETURN_CODE;

    typedef enum
	{
        DEVST_DOWN,							// Device is present, but down
        DEVST_UP,							// Device is present and UP
        DEVST_ERROR,						// Device is in error (maybe being removed)
        DEVST_UNLOADED,						// Stack is being unloaded
        DEVST_RELOADED						// Stack reloaded after being unloaded
    } STACK_STATUS;

protected:
	BtStackType			m_StackType;		// Widcomm or Microsoft stack

private:									// Widcomm stack implementation
	class CDolBtIf*		m_pBtIf;			// Pointer to Widcomm stack class

private:									// Microsoft stack implementation
	HANDLE				m_hLookup;			// WSALookup handle
	HANDLE				m_hPowerThread;		// Thread for power handling
	HANDLE				m_hPowerStop;		// Power Thread stop event handle
	HANDLE				m_hInquiryThread;	// Thread for inquiry handling
	BOOL				m_bInquiryStop;		// Inqury Thread stop flag

public:
	HWND				m_hWnd;				// window handle to send messages to
	BD_ADDR				m_BdAddr;			// Bluetooth address of local device

public:
	CBtStack(HWND hWnd);
	virtual ~CBtStack();

	BtStackType	GetStackType()	{ return m_StackType; }

private:									// Microsoft stack implementation
											// power and inquiry thread handling
	static DWORD PowerProc(LPVOID lpParameter);
	static DWORD InquiryProc(LPVOID lpParameter);
	DWORD		 PowerMonitor();
	DWORD		 DoInquiry();

public:
    // methods to replace virtual methods in base class CBtIf 
	virtual void OnStackStatusChange(CBtStack::STACK_STATUS new_status);
    virtual void OnDeviceResponded(BD_ADDR bda, DEV_CLASS devClass, BD_NAME bdName, BOOL bConnected);
    virtual void OnInquiryComplete(BOOL success, short num_responses); 
    virtual void OnDiscoveryComplete();

public:
	// methods for compatibility with Widcomm SDK
    BOOL				GetBTWCEVersionInfo(BT_CHAR *lpVersion, int nBufSize);
    BOOL				GetDKCEVersionInfo(BT_CHAR *lpVersion, int nBufSize);
    BOOL				GetLocalDeviceInfo();
    BOOL				GetLocalDeviceName(BD_NAME *BdName);
    BOOL				IsDeviceConnectable(); 
    BOOL				SetAutoReconnect(BOOL bAutoConnect);
    BOOL				StartInquiry();
    void				StopInquiry();
    BOOL				StartDiscovery (BD_ADDR p_bda, GUID *p_service_guid);
    int					ReadDiscoveryRecords(BD_ADDR p_bda, int max_size, CSdpDiscoveryRec *p_list, GUID *p_guid_filter = NULL);
    BOND_RETURN_CODE	Bond(BD_ADDR bda, BT_CHAR* pin_code);
    BOOL				BondQuery(BD_ADDR bda);
    BOOL				UnBond(BD_ADDR bda);
};

/////////////////////////////////////////////////////////////////////////////
// class CBtDevice
/////////////////////////////////////////////////////////////////////////////
// A Bluetooth comm port device, given as wParam in WM_EVENTRECEIVEDMESSAGE
// and WM_EVENTRECEIVEDMESSAGE messages

class DECL_DOL_BLUETOOTH CBtDevice
{
public:
	typedef enum
	{
		SUCCESS,
		UNKNOWN_ERROR,
		ALREADY_OPENED,								// Client tried to open port to existing DLCI/BD_ADDR
		NOT_OPENED,									// Function called before conn opened, or after closed
		LINE_ERR,									// Line error
		START_FAILED,								// Connection attempt failed
		PAR_NEG_FAILED,								// Parameter negotiation failed, currently only MTU
		PORT_NEG_FAILED,							// Port negotiation failed
		PEER_CONNECTION_FAILED,						// Connection ended by remote side
		PEER_TIMEOUT,           
		INVALID_PARAMETER
	} PORT_RETURN_CODE;

protected:
	// from discovery
	CBtAddress			m_Address;					// address of device
	DEV_CLASS			m_DevClass;					// device class information
	BT_CHAR				m_Name[BD_NAME_LEN];		// name of the devices
	bool				m_Connected;				// connected flag

	// additional data
	CBtStack*			m_pBtStack;					// pointer to stack class
 	HWND				m_hWnd;						// window handle to send messages to
	BT_CHAR				m_Passkey[PIN_CODE_LEN];	// Passkey for pairing

	// bonding dialog intercept
	bool				m_BondRunning;				// bonding moinitor running
	HANDLE				m_hBondThread;				// bonding thread handle

private:											// Widcomm stack implementation
	class CDolCommPort*	m_pRfCommPort;				// Pointer to Widcomm device class
    class CRfCommIf*	m_pRfCommIf;				// Interface class for creating connections

private:											// Microsoft stack implementation
	SOCKET				m_Socket;					// socket handle
	HANDLE				m_hThread;					// monitoring thread hadle
	bool				m_Open;						// comms open flag
	char*				m_Buffer;					// socket reception buffer

public:
	CBtDevice(CBtStack*			pBtStack,
			  CBtAddress&		address,
			  const BT_CHAR*	name,
			  const BT_CHAR*	passkey);

	// constructor used by inquiry
	CBtDevice(CBtStack*		pBtStack,
			  BD_ADDR		address,
			  DEV_CLASS		devclass,
			  BD_NAME		name,
			  BOOL			connected);
	virtual ~CBtDevice();

	// copy constructor
	CBtDevice(const CBtDevice& device);

	// data access
	CBtAddress		GetAddress()			{ return m_Address; }
	int				GetClass()				{ return m_DevClass[1] & MAJOR_DEV_CLASS_MASK; }
	int				GetType()				{ return m_DevClass[2] & MINOR_DEV_CLASS_MASK; }
	const BT_CHAR*	GetName()				{ return m_Name; }
	const BT_CHAR*	GetPasskey()			{ return m_Passkey; }
	bool			IsConnected()			{ return m_Connected; }

	void SetPasskey(const BT_CHAR* passkey)	{ wcsncpy((WCHAR*)m_Passkey, (WCHAR*)passkey, PIN_CODE_LEN); }
	void SetConnected(bool connected)		{ m_Connected = connected; }

	// operations
	int  Open();							// Pair if necessary, and open

	// for compatibility with Widcomm SDK
	PORT_RETURN_CODE	Close();
	PORT_RETURN_CODE	Write (void *p_data, WORD len_to_write, UINT16 *p_len_written);
	PORT_RETURN_CODE	SetLinkSupervisionTimeOut(UINT16 timeout);
	PORT_RETURN_CODE	GetConnectionStats (tBT_CONN_STATS *p_conn_stats);

private:									// Microsoft stack implementation, comms thread handling
	static DWORD		ThreadProc(LPVOID lpParameter);
	DWORD				Monitor();

protected:									// Widcomm stack implementation, bond handling
	static DWORD WINAPI BondThreadProc(LPVOID device);
	DWORD				BondMonitor();
	void				InitBondMonitor();
	void				CloseBondMonitor();

public:
    // Methods that are required in order to receive data and
    // events respectively from the RFCommPort base class.
	virtual void OnEventReceived(UINT32 eventcode);
	virtual void OnDataReceived(void *pData, UINT16 len);
};

/////////////////////////////////////////////////////////////////////////////
// Class CBtData
/////////////////////////////////////////////////////////////////////////////
// Stores a bluetooth data package, given as lParam in WM_DATARECEIVEDMESSAGE
// Allocates in the constructor, freed by destructor

class DECL_DOL_BLUETOOTH CBtData
{
public:
	BYTE* m_pData;					// pointer to data
	UINT  m_Len;					// length of data in bytes

public:
	CBtData(void* pData, UINT len)
	{
		m_Len   = len;
		m_pData = new BYTE[len];
		memcpy(m_pData, pData, len);
	}

	virtual ~CBtData()
	{
		delete[] m_pData;
	}
};

/////////////////////////////////////////////////////////////////////////////
// EOF