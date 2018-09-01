//
// $History: guid.h $
//
//Amptek DP5 WinUSB    Date: 03/08/2011    Time: 02:49:49 PM

#ifndef GUID_INC
#define GUID_INC

#pragma once

#ifndef DEFINE_GUID
#include <objbase.h>
#endif

#include <initguid.h>

/// Device Interface ID {5A8ED6A1-7FC3-4b6a-A536-95DF35D03448}
DEFINE_GUID(GUID_DP5_INTERFACE_CLASS, 0x5a8ed6a1, 0x7fc3, 0x4b6a, 0xa5, 0x36, 0x95, 0xdf, 0x35, 0xd0, 0x34, 0x48);

/// Setup Class ID {6A4E9A2D-9368-4f01-8E60-B3F9CDBAB5E8}
DEFINE_GUID(GUID_DP5_SETUP_CLASS, 0x6a4e9a2d, 0x9368, 0x4f01, 0x8e, 0x60, 0xb3, 0xf9, 0xcd, 0xba, 0xb5, 0xe8);


#endif // end, #ifndef GUID_INC
