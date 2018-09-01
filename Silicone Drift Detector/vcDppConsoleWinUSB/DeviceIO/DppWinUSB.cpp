#include "StdAfx.h"
#include "DppWinUSB.h"

CDppWinUSB::CDppWinUSB(void)
{
	WinUsb_Disabled = false;
	InitDeviceInfo();
	NumPipes = -1;
	CurrentDevice = 1;
	NumDevices = 0;
	USB_Default_Timeout = FALSE;     // flag that non-default timeout is in use
}

CDppWinUSB::~CDppWinUSB(void)
{
}

void CDppWinUSB::Disable_WinUsb()
{
	WinUsb_Disabled = true;
}

void CDppWinUSB::InitDeviceInfo()
{
	devInfo.deviceHandle = INVALID_HANDLE_VALUE;
	devInfo.winUSBHandle = NULL;
	devInfo.deviceConnected = FALSE;
}

void CDppWinUSB::CloseDeviceHandle()
{
	if ((devInfo.deviceHandle != NULL) && (devInfo.deviceHandle != INVALID_HANDLE_VALUE)) {
		CloseHandle(devInfo.deviceHandle);
		devInfo.deviceHandle = NULL;
	}
}

void CDppWinUSB::CloseWinUsbDevice()
{
	if (WinUsb_Disabled) return;
    if (devInfo.winUSBHandle) {
        if (!WinUsb_Free(devInfo.winUSBHandle)) {
			//DSYSERR("-WinUsb_Free failed");
			//MessageBox(NULL,"WinUsb_Free failed","Closing WinUSB",MB_OK);
        }
        devInfo.winUSBHandle = NULL;
    } 
	CloseDeviceHandle();
}

BOOL CDppWinUSB::GetDevicePath(LPGUID InterfaceGuid, PCHAR DevicePath, size_t BufLen, DWORD MemberIndex)
{
	BOOL bResult = FALSE;
	HDEVINFO deviceInfo;
	SP_DEVICE_INTERFACE_DATA interfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = NULL;
	ULONG length;
	ULONG requiredLength=0;
	HRESULT hr;

	if (WinUsb_Disabled) return FALSE;

	// get device information handle
	deviceInfo = SetupDiGetClassDevs(InterfaceGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	// ===== Loop here to find multiple devices ====================
	// get interface data for device enumeration (first
	interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	bResult = SetupDiEnumDeviceInterfaces(deviceInfo, NULL, InterfaceGuid, MemberIndex, &interfaceData);
	//bResult = SetupDiEnumDeviceInterfaces(deviceInfo, NULL, InterfaceGuid, 0, &interfaceData);
	if(FALSE == bResult)  { return FALSE; }

	// get the details for the current device interface
	bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, NULL, 0, &requiredLength, NULL);
	detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, requiredLength);
	if(NULL == detailData) {
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return FALSE;
	}
	detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	length = requiredLength;
	bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, detailData, length, &requiredLength, NULL);
	if(FALSE == bResult)   {
		LocalFree(detailData);
		return FALSE;
	}
	// copy the device path
	hr = StringCchCopy(DevicePath, BufLen, detailData->DevicePath);
	if(FAILED(hr)) {
		SetupDiDestroyDeviceInfoList(deviceInfo);
		LocalFree(detailData);
	}
	LocalFree(detailData);
	return bResult;
}

HANDLE CDppWinUSB::OpenDevice(BOOL bSync, DWORD MemberIndex)
{
	HANDLE hDev = NULL;
	char devicePath[MAX_DEVPATH_LENGTH];
	BOOL retVal;

	if (WinUsb_Disabled) return INVALID_HANDLE_VALUE;

	retVal = GetDevicePath((LPGUID) &GUID_DP5_INTERFACE_CLASS, devicePath, sizeof(devicePath), MemberIndex);
	//Error-handling code omitted.
	if (retVal) {
		HRESULT hr;
		hr = StringCchCopy(devInfo.devicePath, sizeof(devicePath), devicePath);
	}
	hDev = CreateFile(devicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//Error-handling code omitted.
	return hDev;
}

BOOL CDppWinUSB::Initialize_Device(DWORD MemberIndex)
{
	BOOL bResult;
	WINUSB_INTERFACE_HANDLE usbHandle;
	USB_INTERFACE_DESCRIPTOR ifaceDescriptor;
	WINUSB_PIPE_INFORMATION pipeInfo;
	UCHAR speed;
	ULONG length;
	BYTE bFalse = 0;
	BYTE bTrue = 1;
	ULONG pipeTimeout;

	if (WinUsb_Disabled) return false;

	pipeTimeout = 500;   // 2/4/10 - seems like 500mS should be plenty?
	//Change timeout for Erase FPGA (4 Sec), and Diagnostic Data (2.5 Sec) commands

	InitDeviceInfo();
	devInfo.deviceHandle = OpenDevice(TRUE, MemberIndex);
	bResult = WinUsb_Initialize(devInfo.deviceHandle, &usbHandle);
	//[1]
	if(bResult) {
		devInfo.winUSBHandle = usbHandle;
		length = sizeof(UCHAR);
		bResult = WinUsb_QueryDeviceInformation(devInfo.winUSBHandle, DEVICE_SPEED, &length, &speed);
	}
	//[2]
	if(bResult)
	{
		devInfo.deviceSpeed = speed;
		bResult = WinUsb_QueryInterfaceSettings(devInfo.winUSBHandle, 0, &ifaceDescriptor);
	}
	if(bResult) {
		NumPipes = ifaceDescriptor.bNumEndpoints;
		for(int i=0;i<ifaceDescriptor.bNumEndpoints;i++) {
			PipeId[i] = INVALID_HANDLE_VALUE;
			PipeTimeout[i] = 500;
			//[3]
			bResult = WinUsb_QueryPipe(devInfo.winUSBHandle, 0, (UCHAR) i, &pipeInfo);
			PipeId[i] = (HANDLE)pipeInfo.PipeId;
			PipeTimeout[i] = pipeTimeout;
			//[4]
			if(pipeInfo.PipeType == UsbdPipeTypeBulk && USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId)) {
				devInfo.bulkInPipe = pipeInfo.PipeId;
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkInPipe, IGNORE_SHORT_PACKETS, sizeof(BYTE), &bFalse);  
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkInPipe, AUTO_CLEAR_STALL, sizeof(BYTE), &bTrue);    
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkInPipe, ALLOW_PARTIAL_READS, sizeof(BYTE), &bFalse);    
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkInPipe, PIPE_TRANSFER_TIMEOUT, sizeof(ULONG), &pipeTimeout);
			} else if(pipeInfo.PipeType == UsbdPipeTypeBulk && USB_ENDPOINT_DIRECTION_OUT(pipeInfo.PipeId)) {
				devInfo.bulkOutPipe = pipeInfo.PipeId;
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkInPipe, IGNORE_SHORT_PACKETS, sizeof(BYTE), &bFalse);  
				//WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkOutPipe, SHORT_PACKET_TERMINATE, sizeof(BYTE), &bTrue);
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkOutPipe, SHORT_PACKET_TERMINATE, sizeof(BYTE), &bFalse);
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkOutPipe, AUTO_CLEAR_STALL, sizeof(BYTE), &bTrue);
				WinUsb_SetPipePolicy(devInfo.winUSBHandle, devInfo.bulkOutPipe, PIPE_TRANSFER_TIMEOUT, sizeof(ULONG), &pipeTimeout);
			} else if(pipeInfo.PipeType == UsbdPipeTypeInterrupt) {
				devInfo.interruptPipe = pipeInfo.PipeId;
			} else {
				bResult = FALSE;
				break;
			}
		}
	}
	return bResult;
}

DPP_USB_STATUS CDppWinUSB::FindUSBDevice(DWORD MemberIndex)
{
	BOOL bResult;
	DPP_USB_STATUS USBStatus;	//0=do nothing, 1=new connect, 2=cannot connect

	if (WinUsb_Disabled) return dusbStatConnectFail;

	USBStatus = dusbStatNoAction;
	if (devInfo.deviceConnected) {		// already connected,do nothing
		return (USBStatus);
	}
	USBStatus = dusbStatConnectFail;
	bResult = Initialize_Device(MemberIndex);		// begin connect sequence
	if (! bResult) {								// failed to initialize
		devInfo.deviceConnected = FALSE;
		return (USBStatus);							// return ConnectFail
	}
	if (devInfo.deviceHandle == NULL) {	// NULL device handle,do nothing
		USBStatus = dusbStatNoAction;
		return (USBStatus); 
	}
	if (devInfo.deviceHandle != INVALID_HANDLE_VALUE) {
		devInfo.deviceConnected = true;
		USBStatus = dusbStatNewConnect;				// return NewConnect
	} else {										// return ConnectFail
		devInfo.deviceConnected = false;
		CloseWinUsbDevice();
	}
	//if (devInfo.deviceConnected) {
	//	RegDeviceNotification();
	//}
	return (USBStatus);
}

int CDppWinUSB::CountDP5WinusbDevices(void) 
{
	int NumDevices = 0;
    HKEY hKey;
    DWORD retCode;
	BYTE DevicePath[MAXREGBUFFER];
    DWORD cbDevicePath;
    char KeyName[MAX_PATH];
	DWORD cbKeyName;
	char ErrMsg[MAXREGBUFFER];
	DWORD cbErrMsg;

	char* lpDevicePath;
	
	retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, WinUSBService, 0, KEY_QUERY_VALUE, &hKey);
    if (retCode != ERROR_SUCCESS) {
        return 0;
    }
	// Test ALL Keys (0,1,... are device paths, Count,NextInstance,(Default) have other info)
	for (DWORD idxDP5 = 0; idxDP5 < MAXDP5S + 3; idxDP5++) {	//devs + 3 other keys
		cbKeyName = MAX_PATH;
		cbDevicePath = MAXREGBUFFER;
		retCode = RegEnumValue(hKey, idxDP5, KeyName, &cbKeyName, NULL, NULL, &DevicePath[0], &cbDevicePath);
		if (retCode == ERROR_SUCCESS) {
			lpDevicePath = (char*)DevicePath;
			if (!_strnicmp(KeyName, "Count", 5)) {
				continue;
			} else if (!_strnicmp(lpDevicePath, WinUsbDP5, WinUsbDP5Size)) {	// DP5 device path found
				//TRACE("DP5 device [%d]: %s=%s\r\n", (NumDevices + 1), KeyName, DevicePath);
				NumDevices++;
				continue;
			}
		} else if (retCode == ERROR_NO_MORE_ITEMS) {	// no more values to read
			break;
		} else {										// error reading values
			cbErrMsg = MAXREGBUFFER;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, retCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), ErrMsg, cbErrMsg, NULL);
			break;
		}
	}
	RegCloseKey(hKey);
    return NumDevices;		/* return number of devices */
}

string CDppWinUSB::USBStatusString(DPP_USB_STATUS USBStatus)
{
	string strStatus;
	strStatus = "";
	switch (USBStatus) {
		case dusbStatNewConnect:
			strStatus = "USB DPP device connected.";
			break;
		case dusbStatConnectFail:
			strStatus = "Cannot connect to USB DPP device.";
			break;
	}
	return (strStatus);
}

string CDppWinUSB::DisplayBufferArray(unsigned char buffer[], unsigned long bufSizeIn)
{
    unsigned long i;
	string strVal("");
	string strMsg("");
	stringex strfn;
	for(i=0;i<bufSizeIn;i++) {
		strVal = strfn.Format("%.2X ",buffer[i]);
		strMsg += strVal;
		if (((i+1) % 16) == 0 ) { 
			strMsg += "\r\n";
		} else if (((i+1) % 8) == 0 ) { 
			strMsg += "   ";
		}
	}
	strMsg += "\r\n";
	return strMsg;
}

void CDppWinUSB::SaveStringDataToFile(string strData)
{
   FILE  *out;
   string strFilename;
   string strError;
   stringex strfn;

   strFilename = "CfgHex.txt";

   if ( (out = fopen(strFilename.c_str(),"w")) == (FILE *) NULL)
      strError = strfn.Format("Couldn't open %s for writing.\n", strFilename.c_str());
   else
   {
      fprintf(out,"%s\n",strData.c_str());
   }
   fclose(out);
}

bool CDppWinUSB::SendPacketUSB(unsigned char Buffer[], DEVICE_INFO TestDevInfo, unsigned char PacketIn[])
{
    unsigned long bytesWritten; 
	unsigned long bytesRead;
    int success;
    long PLen;
	unsigned long pipeTimeout;

    if ((Buffer[2] == PID1_REQ_SCOPE_MISC) && Buffer[3] == PID2_SEND_DIAGNOSTIC_DATA) {
		// Request Diagnostic Packet delay 'USB_DiagDataDelayMS
		pipeTimeout = USB_DiagDataDelayMS;
		success = WinUsb_SetPipePolicy(TestDevInfo.winUSBHandle, TestDevInfo.bulkInPipe, PIPE_TRANSFER_TIMEOUT, sizeof(unsigned long), &pipeTimeout);
        USB_Default_Timeout = false;     // flag that non-default timeout is in use
	} else {
        if (USB_Default_Timeout == false) {  // change pipetimeout to default if it isn't already set that way
			pipeTimeout = USB_TIMEOUT;
			success = WinUsb_SetPipePolicy(TestDevInfo.winUSBHandle, TestDevInfo.bulkInPipe, PIPE_TRANSFER_TIMEOUT, sizeof(unsigned long), &pipeTimeout);
            USB_Default_Timeout = true;      // flag that default timeout is in use
        }
	}

	bytesWritten = NULL;
	bytesRead = NULL;
	PLen = (Buffer[4] * 256) + Buffer[5] + 8;
	success = WinUsb_WritePipe(TestDevInfo.winUSBHandle, TestDevInfo.bulkOutPipe, Buffer, PLen, &bytesWritten, NULL);
	Sleep(50);				// 20110907 Added to improve usb communications while running other processes
	if (success) {
		success = WinUsb_ReadPipe(TestDevInfo.winUSBHandle, TestDevInfo.bulkInPipe, PacketIn, 32767, &bytesRead, NULL);
		if (success) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
