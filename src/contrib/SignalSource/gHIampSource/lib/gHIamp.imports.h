// Import declarations for gHIamp.h, generated by dylib_imports.sh
#ifndef gHIamp_IMPORTS_H
#define gHIamp_IMPORTS_H

#include "Win32Defs.h"

#if DYNAMIC_IMPORTS
#define GT_GetDriverVersion GT_GetDriverVersion_89e5
#define GT_GetHWVersion GT_GetHWVersion_f8ee
#define GT_OpenDevice GT_OpenDevice_306c
#define GT_OpenDeviceEx GT_OpenDeviceEx_6e94
#define GT_CloseDevice GT_CloseDevice_e5f9
#define GT_GetData GT_GetData_c54e
#define GT_GetOverlappedResult GT_GetOverlappedResult_a039
#define GT_Start GT_Start_f497
#define GT_Stop GT_Stop_cce8
#define GT_GetLastError GT_GetLastError_bc18
#define GT_ResetTransfer GT_ResetTransfer_cadc
#define GT_GetSerial GT_GetSerial_4e25
#define GT_VR GT_VR_b5ed
#define GT_Calibrate GT_Calibrate_fd17
#define GT_SetScale GT_SetScale_1ae0
#define GT_GetScale GT_GetScale_209a
#define GT_GetFactoryScaleSettings GT_GetFactoryScaleSettings_4c60
#define GT_GetImpedance GT_GetImpedance_04f1
#define GT_GetAvailableChannels GT_GetAvailableChannels_8e08
#define GT_GetNumberOfSupportedSampleRates GT_GetNumberOfSupportedSampleRates_ad89
#define GT_GetSupportedSampleRates GT_GetSupportedSampleRates_2404
#define GT_SetConfiguration GT_SetConfiguration_4b5a
#define GT_GetConfiguration GT_GetConfiguration_1e77
#define GT_GetFilterSpec GT_GetFilterSpec_08ec
#define GT_GetNumberOfFilter GT_GetNumberOfFilter_97f8
#define GT_GetNotchSpec GT_GetNotchSpec_9cb1
#define GT_GetNumberOfNotch GT_GetNumberOfNotch_14d0
#endif // DYNAMIC_IMPORTS

#include "gHIamp.h"

#if DYNAMIC_IMPORTS
#undef GT_GetDriverVersion
#undef GT_GetHWVersion
#undef GT_OpenDevice
#undef GT_OpenDeviceEx
#undef GT_CloseDevice
#undef GT_GetData
#undef GT_GetOverlappedResult
#undef GT_Start
#undef GT_Stop
#undef GT_GetLastError
#undef GT_ResetTransfer
#undef GT_GetSerial
#undef GT_VR
#undef GT_Calibrate
#undef GT_SetScale
#undef GT_GetScale
#undef GT_GetFactoryScaleSettings
#undef GT_GetImpedance
#undef GT_GetAvailableChannels
#undef GT_GetNumberOfSupportedSampleRates
#undef GT_GetSupportedSampleRates
#undef GT_SetConfiguration
#undef GT_GetConfiguration
#undef GT_GetFilterSpec
#undef GT_GetNumberOfFilter
#undef GT_GetNotchSpec
#undef GT_GetNumberOfNotch

extern "C" {
extern FLOAT (__stdcall *GT_GetDriverVersion)(void);
extern FLOAT (__stdcall *GT_GetHWVersion)(HANDLE hDevice);
extern HANDLE (__stdcall *GT_OpenDevice)(int iPortNumber);
extern HANDLE (__stdcall *GT_OpenDeviceEx)(LPSTR lpSerial);
extern BOOL (__stdcall *GT_CloseDevice)(HANDLE *hDevice);
extern BOOL (__stdcall *GT_GetData)(HANDLE hDevice, BYTE *pData, DWORD dwSzBuffer, OVERLAPPED *ov);
extern BOOL (__stdcall *GT_GetOverlappedResult)(HANDLE hDevice, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
extern BOOL (__stdcall *GT_Start)(HANDLE hDevice);
extern BOOL (__stdcall *GT_Stop)(HANDLE hDevice);
extern BOOL (__stdcall *GT_GetLastError)(WORD *wErrorCode, char *pLastError);
extern BOOL (__stdcall *GT_ResetTransfer)(HANDLE hDevice);
extern BOOL (__stdcall *GT_GetSerial)(HANDLE hDevice, LPSTR lpstrSerial, UINT uiSize);
extern BOOL (__stdcall *GT_VR)(int nargin, void *varargin[], int nargout, void *varargout[]);
extern BOOL (__stdcall *GT_Calibrate)(HANDLE hDevice, PSCALE scaling);
extern BOOL (__stdcall *GT_SetScale)(HANDLE hDevice, PSCALE scaling);
extern BOOL (__stdcall *GT_GetScale)(HANDLE hDevice, PSCALE scaling);
extern BOOL (__stdcall *GT_GetFactoryScaleSettings)(HANDLE hDevice, PSCALE factoryScaling);
extern BOOL (__stdcall *GT_GetImpedance)(HANDLE hDevice, GT_HIAMP_CHANNEL_IMPEDANCES *channelImpedances);
extern BOOL (__stdcall *GT_GetAvailableChannels)(HANDLE hDevice, UCHAR *availableChannels, WORD availableChannelsLength);
extern BOOL (__stdcall *GT_GetNumberOfSupportedSampleRates)(HANDLE hDevice, int *numberOfSupportedSampleRates);
extern BOOL (__stdcall *GT_GetSupportedSampleRates)(HANDLE hDevice, float* supportedSampleRates);
extern BOOL (__stdcall *GT_SetConfiguration)(HANDLE hDevice, GT_HIAMP_CONFIGURATION configuration);
extern BOOL (__stdcall *GT_GetConfiguration)(HANDLE hDevice, GT_HIAMP_CONFIGURATION *configuration);
extern BOOL (__stdcall *GT_GetFilterSpec)(FILT *filterSpec);
extern BOOL (__stdcall *GT_GetNumberOfFilter)(int* nof);
extern BOOL (__stdcall *GT_GetNotchSpec)(FILT *filterSpec);
extern BOOL (__stdcall *GT_GetNumberOfNotch)(int* nof);
}
#endif // DYNAMIC_IMPORTS

namespace Dylib { bool gHIamp_Loaded(); }

#endif // gHIamp_IMPORTS_H
