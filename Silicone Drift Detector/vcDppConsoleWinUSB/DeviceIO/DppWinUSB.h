/** CDppWinUSB CDppWinUSB */
#pragma once
#pragma comment(lib, ".\\DeviceIO\\winusb.lib")
#pragma comment(lib, "setupapi.lib")

// MS var redef bug fix
#ifdef _MP
	#undef _MP
#endif
// ====================
// Include Windows headers
#include <Windows.h>
#include <Strsafe.h>
#include "stringex.h"
// Include WinUSB headers
#include <Winusb.h>
#include <Setupapi.h>
#include "guid.h"
#include <Dbt.h>
#include "DP5Protocol.h"

#define MAX_DEVPATH_LENGTH 256
#define MAXDP5S 128				// max number of devices
#define MAXREGBUFFER 128
#define WinUSBService "SYSTEM\\CurrentControlSet\\Services\\WinUSB\\Enum"
#define WinUsbDP5 "USB\\Vid_10c4&Pid_842a"
#define WinUsbDP5Size 21
#define DRV_INFO_OPTION_NONE 0x00
#define DRV_INFO_OPTION_MFG_NAME 0x01
#define DRV_INFO_OPTION_PROV_NAME 0x02
#define DRV_INFO_OPTION_DESC 0x04
#define DRV_INFO_OPTION_DATE 0x08
#define DRV_INFO_OPTION_VER 0x10
#define DRV_INFO_OPTION_MS_TERM 0x20
#define DRV_INFO_OPTION_UX_TERM 0x40
#define DRV_INFO_OPTION_LABEL 0x80
#define DRV_INFO_OPTION_ALL 0xFF
#define MAXPIPES 10

typedef enum _DPP_USB_STATUS
{
	dusbStatNoAction,
	dusbStatNewConnect,
	dusbStatConnectFail
} DPP_USB_STATUS;

/// Holds WinUSB device information.
typedef struct WINUSB_DEVICE_INFO
{
	/// System device handle opened with CreateFile using a device path. 
	HANDLE deviceHandle;
	/// WinUSB device interface handle opened with WinUsb_Initialize using a CreateFile device handle. 
	WINUSB_INTERFACE_HANDLE winUSBHandle;
	/// USB device speed indicator.
	UCHAR deviceSpeed;
	/// Bulk in pipe identifier.
	UCHAR bulkInPipe;
	/// Bulk out pipe identifier.
	UCHAR bulkOutPipe;
	/// Interrupt pipe identifier.
	UCHAR interruptPipe;
	/// Device path identifier string used in opening a connection to a USB device.
	char devicePath[MAX_DEVPATH_LENGTH];
	/// True if a USB deevice is connected.
	BOOL deviceConnected;
} DEVICE_INFO, *PDEVICE_INFO;

class CDppWinUSB
{
public:
	CDppWinUSB(void);
	~CDppWinUSB(void);

	bool WinUsb_Disabled;
	void Disable_WinUsb();

	/// Holds the current USB device information.
	DEVICE_INFO devInfo;
	/// Number of endpoints.
	int NumPipes;

	/// Pipe ID array for the current pipes.
	HANDLE PipeId[MAXPIPES];
	/// Pipe timeout value array for the current pipes.
	ULONG PipeTimeout[MAXPIPES];
	/// Initializes the DEVICE_INFO data.
	void InitDeviceInfo();
	/// Closes the system device handle
	void CloseDeviceHandle();
	/// Closes the WinUSB device interface handle and frees the resources.
	void CloseWinUsbDevice();
	/// Returns the device path string for a selcted device.
	BOOL GetDevicePath(LPGUID InterfaceGuid, PCHAR DevicePath, size_t BufLen, DWORD MemberIndex);
	/// Opens a connection to a selected DPP device.
	HANDLE OpenDevice(BOOL bSync, DWORD MemberIndex);
	/// Initializes the selected DPP device.
	BOOL Initialize_Device(DWORD MemberIndex);

	// dialog helper functions

	/// Finds a USB DPP device and sets DEVICE_INFO status indicators.  DPP_USB_STATUS is not supported for console applications.
	DPP_USB_STATUS FindUSBDevice(DWORD MemberIndex);
	/// Returns the number of USB devices found.
	int CountDP5WinusbDevices(void);
	/// USB device index.  CurrentDevice index starts at 1 for display purposes.  WinUSB device indexes (MemberIndex) start at 0.
	int CurrentDevice;
	/// Number of devices found.
	int NumDevices;
	/// Indicates a DPP device has been found. 
	bool isDppFound;
		
	string DisplayBufferArray(unsigned char buffer[], unsigned long bufSizeIn);
	void SaveStringDataToFile(string strData);

	string USBStatusString(DPP_USB_STATUS USBStatus);

	/// Sends a USB buffer and returns any data read back.
	bool SendPacketUSB(unsigned char Buffer[], DEVICE_INFO TestDevInfo, unsigned char PacketIn[]);
	bool USB_Default_Timeout;

};
