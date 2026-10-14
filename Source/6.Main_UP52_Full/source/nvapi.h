#pragma once

#ifndef _NVAPI_H
#define _NVAPI_H

#pragma pack(push,8)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#define __cdecl
#endif

#define NVAPI_INTERFACE extern NvAPI_Status __cdecl

#if defined(__GNUC__) || defined(__arm) || defined(__IAR_SYSTEMS_ICC__) || defined(__ghs__) || defined(_WIN64)
    typedef unsigned long long NvU64;
#else
    typedef unsigned __int64   NvU64;
#endif

#if (defined(macintosh) || defined(__APPLE__)) && !defined(__LP64__)
    typedef signed long        NvS32;
#else
    typedef signed int         NvS32;
#endif

    typedef unsigned long    NvU32;
    typedef unsigned short   NvU16;
    typedef unsigned char    NvU8;

#define NV_DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

    NV_DECLARE_HANDLE(NvDisplayHandle);
    NV_DECLARE_HANDLE(NvUnAttachedDisplayHandle);
    NV_DECLARE_HANDLE(NvLogicalGpuHandle);
    NV_DECLARE_HANDLE(NvPhysicalGpuHandle);
    NV_DECLARE_HANDLE(NvEventHandle);

#define NVAPI_DEFAULT_HANDLE        0

#define NVAPI_GENERIC_STRING_MAX    4096
#define NVAPI_LONG_STRING_MAX       256
#define NVAPI_SHORT_STRING_MAX      64

    typedef struct
    {
        NvS32   sX;
        NvS32   sY;
        NvS32   sWidth;
        NvS32   sHeight;
    } NvSBox;

#define NVAPI_MAX_PHYSICAL_GPUS             64
#define NVAPI_MAX_LOGICAL_GPUS              64
#define NVAPI_MAX_AVAILABLE_GPU_TOPOLOGIES  256
#define NVAPI_MAX_GPU_TOPOLOGIES            NVAPI_MAX_PHYSICAL_GPUS
#define NVAPI_MAX_GPU_PER_TOPOLOGY          8
#define NVAPI_MAX_DISPLAY_HEADS             2
#define NVAPI_MAX_DISPLAYS                  NVAPI_MAX_PHYSICAL_GPUS * NVAPI_MAX_DISPLAY_HEADS

#define NV_MAX_HEADS        4
#define NV_MAX_VID_STREAMS  4
#define NV_MAX_VID_PROFILES 4

    typedef char NvAPI_String[NVAPI_GENERIC_STRING_MAX];
    typedef char NvAPI_LongString[NVAPI_LONG_STRING_MAX];
    typedef char NvAPI_ShortString[NVAPI_SHORT_STRING_MAX];

#define MAKE_NVAPI_VERSION(typeName,ver) (NvU32)(sizeof(typeName) | ((ver)<<16))
#define GET_NVAPI_VERSION(ver) (NvU32)((ver)>>16)
#define GET_NVAPI_SIZE(ver) (NvU32)((ver) & 0xffff)

    typedef enum
    {
        NVAPI_OK = 0,
        NVAPI_ERROR = -1,
        NVAPI_LIBRARY_NOT_FOUND = -2,
        NVAPI_NO_IMPLEMENTATION = -3,
        NVAPI_API_NOT_INTIALIZED = -4,
        NVAPI_INVALID_ARGUMENT = -5,
        NVAPI_NVIDIA_DEVICE_NOT_FOUND = -6,
        NVAPI_END_ENUMERATION = -7,
        NVAPI_INVALID_HANDLE = -8,
        NVAPI_INCOMPATIBLE_STRUCT_VERSION = -9,
        NVAPI_HANDLE_INVALIDATED = -10,
        NVAPI_OPENGL_CONTEXT_NOT_CURRENT = -11,
        NVAPI_NO_GL_EXPERT = -12,
        NVAPI_INSTRUMENTATION_DISABLED = -13,
        NVAPI_EXPECTED_LOGICAL_GPU_HANDLE = -100,
        NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE = -101,
        NVAPI_EXPECTED_DISPLAY_HANDLE = -102,
        NVAPI_INVALID_COMBINATION = -103,
        NVAPI_NOT_SUPPORTED = -104,
        NVAPI_PORTID_NOT_FOUND = -105,
        NVAPI_EXPECTED_UNATTACHED_DISPLAY_HANDLE = -106,
        NVAPI_INVALID_PERF_LEVEL = -107,
        NVAPI_DEVICE_BUSY = -108,
        NVAPI_NV_PERSIST_FILE_NOT_FOUND = -109,
        NVAPI_PERSIST_DATA_NOT_FOUND = -110,
        NVAPI_EXPECTED_TV_DISPLAY = -111,
        NVAPI_EXPECTED_TV_DISPLAY_ON_DCONNECTOR = -112,
        NVAPI_NO_ACTIVE_SLI_TOPOLOGY = -113,
        NVAPI_SLI_RENDERING_MODE_NOTALLOWED = -114,
        NVAPI_EXPECTED_DIGITAL_FLAT_PANEL = -115,
        NVAPI_ARGUMENT_EXCEED_MAX_SIZE = -116,
        NVAPI_DEVICE_SWITCHING_NOT_ALLOWED = -117,
        NVAPI_TESTING_CLOCKS_NOT_SUPPORTED = -118,
        NVAPI_UNKNOWN_UNDERSCAN_CONFIG = -119,
        NVAPI_TIMEOUT_RECONFIGURING_GPU_TOPO = -120,
        NVAPI_DATA_NOT_FOUND = -121,
        NVAPI_EXPECTED_ANALOG_DISPLAY = -122,
        NVAPI_NO_VIDLINK = -123,
        NVAPI_REQUIRES_REBOOT = -124,
        NVAPI_INVALID_HYBRID_MODE = -125,
        NVAPI_MIXED_TARGET_TYPES = -126,
        NVAPI_SYSWOW64_NOT_SUPPORTED = -127,
        NVAPI_IMPLICIT_SET_GPU_TOPOLOGY_CHANGE_NOT_ALLOWED = -128,
        NVAPI_REQUEST_USER_TO_CLOSE_NON_MIGRATABLE_APPS = -129,
        NVAPI_OUT_OF_MEMORY = -130,
        NVAPI_WAS_STILL_DRAWING = -131,
        NVAPI_FILE_NOT_FOUND = -132,
        NVAPI_TOO_MANY_UNIQUE_STATE_OBJECTS = -133,
        NVAPI_INVALID_CALL = -134,
        NVAPI_D3D10_1_LIBRARY_NOT_FOUND = -135,
        NVAPI_FUNCTION_NOT_FOUND = -136,
    } NvAPI_Status;

    NVAPI_INTERFACE NvAPI_Initialize();

    NVAPI_INTERFACE NvAPI_GetErrorMessage(NvAPI_Status nr, NvAPI_ShortString szDesc);

    NVAPI_INTERFACE NvAPI_GetInterfaceVersionString(NvAPI_ShortString szDesc);

    typedef struct
    {
        NvU32              version;
        NvU32              drvVersion;
        NvU32              bldChangeListNum;
        NvAPI_ShortString  szBuildBranchString;
        NvAPI_ShortString  szAdapterString;
    } NV_DISPLAY_DRIVER_VERSION;
#define NV_DISPLAY_DRIVER_VERSION_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_DRIVER_VERSION,1)
    NVAPI_INTERFACE NvAPI_GetDisplayDriverVersion(NvDisplayHandle hNvDisplay, NV_DISPLAY_DRIVER_VERSION* pVersion);

    NVAPI_INTERFACE NvAPI_EnumNvidiaDisplayHandle(NvU32 thisEnum, NvDisplayHandle* pNvDispHandle);

    NVAPI_INTERFACE NvAPI_EnumNvidiaUnAttachedDisplayHandle(NvU32 thisEnum, NvUnAttachedDisplayHandle* pNvUnAttachedDispHandle);

    NVAPI_INTERFACE NvAPI_EnumPhysicalGPUs(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount);

    NVAPI_INTERFACE NvAPI_EnumLogicalGPUs(NvLogicalGpuHandle nvGPUHandle[NVAPI_MAX_LOGICAL_GPUS], NvU32* pGpuCount);

    NVAPI_INTERFACE NvAPI_GetPhysicalGPUsFromDisplay(NvDisplayHandle hNvDisp, NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount);

    NVAPI_INTERFACE NvAPI_GetPhysicalGPUFromUnAttachedDisplay(NvUnAttachedDisplayHandle hNvUnAttachedDisp, NvPhysicalGpuHandle* pPhysicalGpu);

    NVAPI_INTERFACE NvAPI_CreateDisplayFromUnAttachedDisplay(NvUnAttachedDisplayHandle hNvUnAttachedDisp, NvDisplayHandle* pNvDisplay);

    NVAPI_INTERFACE NvAPI_GetLogicalGPUFromDisplay(NvDisplayHandle hNvDisp, NvLogicalGpuHandle* pLogicalGPU);

    NVAPI_INTERFACE NvAPI_GetLogicalGPUFromPhysicalGPU(NvPhysicalGpuHandle hPhysicalGPU, NvLogicalGpuHandle* pLogicalGPU);

    NVAPI_INTERFACE NvAPI_GetPhysicalGPUsFromLogicalGPU(NvLogicalGpuHandle hLogicalGPU, NvPhysicalGpuHandle hPhysicalGPU[NVAPI_MAX_PHYSICAL_GPUS], NvU32* pGpuCount);

    NVAPI_INTERFACE NvAPI_GetAssociatedNvidiaDisplayHandle(const char* szDisplayName, NvDisplayHandle* pNvDispHandle);

    NVAPI_INTERFACE NvAPI_GetAssociatedNvidiaDisplayName(NvDisplayHandle NvDispHandle, NvAPI_ShortString szDisplayName);

    NVAPI_INTERFACE NvAPI_GetUnAttachedAssociatedDisplayName(NvUnAttachedDisplayHandle hNvUnAttachedDisp, NvAPI_ShortString szDisplayName);

    NVAPI_INTERFACE NvAPI_EnableHWCursor(NvDisplayHandle hNvDisplay);

    NVAPI_INTERFACE NvAPI_DisableHWCursor(NvDisplayHandle hNvDisplay);

    NVAPI_INTERFACE NvAPI_GetVBlankCounter(NvDisplayHandle hNvDisplay, NvU32* pCounter);

    NVAPI_INTERFACE NvAPI_SetRefreshRateOverride(NvDisplayHandle hNvDisplay, NvU32 outputsMask, float refreshRate, NvU32 bSetDeferred);

    NVAPI_INTERFACE NvAPI_GetAssociatedDisplayOutputId(NvDisplayHandle hNvDisplay, NvU32* pOutputId);

    typedef enum
    {
        NV_DP_1_62GBPS = 6,
        NV_DP_2_70GBPS = 0xA,
    } NV_DP_LINK_RATE;

    typedef enum
    {
        NV_DP_1_LANE = 1,
        NV_DP_2_LANE = 2,
        NV_DP_4_LANE = 4,
    } NV_DP_LANE_COUNT;

    typedef enum
    {
        NV_DP_COLOR_FORMAT_RGB = 0,
        NV_DP_COLOR_FORMAT_YCbCr422,
        NV_DP_COLOR_FORMAT_YCbCr444,
    } NV_DP_COLOR_FORMAT;

    typedef enum
    {
        NV_DP_COLORIMETRY_RGB = 0,
        NV_DP_COLORIMETRY_YCbCr_ITU601,
        NV_DP_COLORIMETRY_YCbCr_ITU709,
    } NV_DP_COLORIMETRY;

    typedef enum
    {
        NV_DP_DYNAMIC_RANGE_VESA = 0,
        NV_DP_DYNAMIC_RANGE_CEA,
    } NV_DP_DYNAMIC_RANGE;

    typedef enum
    {
        NV_DP_BPC_DEFAULT = 0,
        NV_DP_BPC_6,
        NV_DP_BPC_8,
        NV_DP_BPC_10,
        NV_DP_BPC_12,
        NV_DP_BPC_16,
    } NV_DP_BPC;

    typedef struct
    {
        NvU32               version;
        NvU32               dpcd_ver;
        NV_DP_LINK_RATE     maxLinkRate;
        NV_DP_LANE_COUNT    maxLaneCount;
        NV_DP_LINK_RATE     curLinkRate;
        NV_DP_LANE_COUNT    curLaneCount;
        NV_DP_COLOR_FORMAT  colorFormat;
        NV_DP_DYNAMIC_RANGE dynamicRange;
        NV_DP_COLORIMETRY   colorimetry;
        NV_DP_BPC           bpc;
        NvU32               isDp : 1;
        NvU32               isInternalDp : 1;
        NvU32               isColorCtrlSupported : 1;
        NvU32               is6BPCSupported : 1;
        NvU32               is8BPCSupported : 1;
        NvU32               is10BPCSupported : 1;
        NvU32               is12BPCSupported : 1;
        NvU32               is16BPCSupported : 1;
    } NV_DISPLAY_PORT_INFO;

#define NV_DISPLAY_PORT_INFO_VER   MAKE_NVAPI_VERSION(NV_DISPLAY_PORT_INFO,1)

    NVAPI_INTERFACE NvAPI_GetDisplayPortInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_PORT_INFO* pInfo);

    typedef struct
    {
        NvU32               version;
        NV_DP_LINK_RATE     linkRate;
        NV_DP_LANE_COUNT    laneCount;
        NV_DP_COLOR_FORMAT  colorFormat;
        NV_DP_DYNAMIC_RANGE dynamicRange;
        NV_DP_COLORIMETRY   colorimetry;
        NV_DP_BPC           bpc;
        NvU32               isHPD : 1;
        NvU32               isSetDeferred : 1;
        NvU32               isChromaLpfOff : 1;
        NvU32               isDitherOff : 1;
    } NV_DISPLAY_PORT_CONFIG;

#define NV_DISPLAY_PORT_CONFIG_VER   MAKE_NVAPI_VERSION(NV_DISPLAY_PORT_CONFIG,2)
#define NV_DISPLAY_PORT_CONFIG_VER_1 MAKE_NVAPI_VERSION(NV_DISPLAY_PORT_CONFIG,1)
#define NV_DISPLAY_PORT_CONFIG_VER_2 MAKE_NVAPI_VERSION(NV_DISPLAY_PORT_CONFIG,2)

    NVAPI_INTERFACE NvAPI_SetDisplayPort(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_PORT_CONFIG* pCfg);

    typedef struct
    {
        NvU32      version;
        NvU32      isGpuHDMICapable : 1;
        NvU32      isMonUnderscanCapable : 1;
        NvU32      isMonBasicAudioCapable : 1;
        NvU32      isMonYCbCr444Capable : 1;
        NvU32      isMonYCbCr422Capable : 1;
        NvU32      isMonxvYCC601Capable : 1;
        NvU32      isMonxvYCC709Capable : 1;
        NvU32      isMonHDMI : 1;
        NvU32      EDID861ExtRev;
    } NV_HDMI_SUPPORT_INFO;

#define NV_HDMI_SUPPORT_INFO_VER  MAKE_NVAPI_VERSION(NV_HDMI_SUPPORT_INFO,1)

    NVAPI_INTERFACE NvAPI_GetHDMISupportInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_HDMI_SUPPORT_INFO* pInfo);

    NVAPI_INTERFACE NvAPI_GPU_GetAllOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pOutputsMask);

    NVAPI_INTERFACE NvAPI_GPU_GetConnectedOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pOutputsMask);

    NVAPI_INTERFACE NvAPI_GPU_GetConnectedSLIOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pOutputsMask);

    NVAPI_INTERFACE NvAPI_GPU_GetConnectedOutputsWithLidState(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pOutputsMask);

    NVAPI_INTERFACE NvAPI_GPU_GetConnectedSLIOutputsWithLidState(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pOutputsMask);

    typedef enum
    {
        NV_SYSTEM_TYPE_UNKNOWN = 0,
        NV_SYSTEM_TYPE_LAPTOP = 1,
        NV_SYSTEM_TYPE_DESKTOP = 2,

    } NV_SYSTEM_TYPE;

    NVAPI_INTERFACE NvAPI_GPU_GetSystemType(NvPhysicalGpuHandle hPhysicalGpu, NV_SYSTEM_TYPE* pSystemType);

    NVAPI_INTERFACE NvAPI_GPU_GetActiveOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pOutputsMask);

#define NV_EDID_V1_DATA_SIZE   256
#define NV_EDID_DATA_SIZE      NV_EDID_V1_DATA_SIZE

    typedef struct
    {
        NvU32   version;
        NvU8    EDID_Data[NV_EDID_DATA_SIZE];
        NvU32   sizeofEDID;
    } NV_EDID;

#define NV_EDID_VER         MAKE_NVAPI_VERSION(NV_EDID,2)

    NVAPI_INTERFACE NvAPI_GPU_GetEDID(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID* pEDID);

    typedef enum _NV_GPU_OUTPUT_TYPE
    {
        NVAPI_GPU_OUTPUT_UNKNOWN = 0,
        NVAPI_GPU_OUTPUT_CRT = 1,
        NVAPI_GPU_OUTPUT_DFP = 2,
        NVAPI_GPU_OUTPUT_TV = 3,
    } NV_GPU_OUTPUT_TYPE;

    NVAPI_INTERFACE NvAPI_GPU_GetOutputType(NvPhysicalGpuHandle hPhysicalGpu, NvU32 outputId, NV_GPU_OUTPUT_TYPE* pOutputType);

    NVAPI_INTERFACE NvAPI_GPU_ValidateOutputCombination(NvPhysicalGpuHandle hPhysicalGpu, NvU32 outputsMask);

    typedef enum _NV_GPU_CONNECTOR_TYPE
    {
        NVAPI_GPU_CONNECTOR_VGA_15_PIN = 0x00000000,
        NVAPI_GPU_CONNECTOR_TV_COMPOSITE = 0x00000010,
        NVAPI_GPU_CONNECTOR_TV_SVIDEO = 0x00000011,
        NVAPI_GPU_CONNECTOR_TV_HDTV_COMPONENT = 0x00000013,
        NVAPI_GPU_CONNECTOR_TV_SCART = 0x00000014,
        NVAPI_GPU_CONNECTOR_TV_COMPOSITE_SCART_ON_EIAJ4120 = 0x00000016,
        NVAPI_GPU_CONNECTOR_TV_HDTV_EIAJ4120 = 0x00000017,
        NVAPI_GPU_CONNECTOR_PC_POD_HDTV_YPRPB = 0x00000018,
        NVAPI_GPU_CONNECTOR_PC_POD_SVIDEO = 0x00000019,
        NVAPI_GPU_CONNECTOR_PC_POD_COMPOSITE = 0x0000001A,
        NVAPI_GPU_CONNECTOR_DVI_I_TV_SVIDEO = 0x00000020,
        NVAPI_GPU_CONNECTOR_DVI_I_TV_COMPOSITE = 0x00000021,
        NVAPI_GPU_CONNECTOR_DVI_I = 0x00000030,
        NVAPI_GPU_CONNECTOR_DVI_D = 0x00000031,
        NVAPI_GPU_CONNECTOR_ADC = 0x00000032,
        NVAPI_GPU_CONNECTOR_LFH_DVI_I_1 = 0x00000038,
        NVAPI_GPU_CONNECTOR_LFH_DVI_I_2 = 0x00000039,
        NVAPI_GPU_CONNECTOR_SPWG = 0x00000040,
        NVAPI_GPU_CONNECTOR_OEM = 0x00000041,
        NVAPI_GPU_CONNECTOR_DISPLAYPORT_EXTERNAL = 0x00000046,
        NVAPI_GPU_CONNECTOR_DISPLAYPORT_INTERNAL = 0x00000047,
        NVAPI_GPU_CONNECTOR_HDMI_A = 0x00000061,
        NVAPI_GPU_CONNECTOR_UNKNOWN = 0xFFFFFFFF,
    } NV_GPU_CONNECTOR_TYPE;

    NVAPI_INTERFACE NvAPI_GPU_GetFullName(NvPhysicalGpuHandle hPhysicalGpu, NvAPI_ShortString szName);

    NVAPI_INTERFACE NvAPI_GPU_GetPCIIdentifiers(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pDeviceId, NvU32* pSubSystemId, NvU32* pRevisionId, NvU32* pExtDeviceId);

    typedef enum _NV_GPU_TYPE
    {
        NV_SYSTEM_TYPE_GPU_UNKNOWN = 0,
        NV_SYSTEM_TYPE_IGPU = 1,
        NV_SYSTEM_TYPE_DGPU = 2,
    } NV_GPU_TYPE;

    NVAPI_INTERFACE NvAPI_GPU_GetGPUType(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_TYPE* pGpuType);

    typedef enum _NV_GPU_BUS_TYPE
    {
        NVAPI_GPU_BUS_TYPE_UNDEFINED = 0,
        NVAPI_GPU_BUS_TYPE_PCI = 1,
        NVAPI_GPU_BUS_TYPE_AGP = 2,
        NVAPI_GPU_BUS_TYPE_PCI_EXPRESS = 3,
        NVAPI_GPU_BUS_TYPE_FPCI = 4,
    } NV_GPU_BUS_TYPE;

    NVAPI_INTERFACE NvAPI_GPU_GetBusType(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_BUS_TYPE* pBusType);

    NVAPI_INTERFACE NvAPI_GPU_GetBusId(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pBusId);

    NVAPI_INTERFACE NvAPI_GPU_GetBusSlotId(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pBusSlotId);

    NVAPI_INTERFACE NvAPI_GPU_GetIRQ(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pIRQ);

    NVAPI_INTERFACE NvAPI_GPU_GetVbiosRevision(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pBiosRevision);

    NVAPI_INTERFACE NvAPI_GPU_GetVbiosOEMRevision(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pBiosRevision);

    NVAPI_INTERFACE NvAPI_GPU_GetVbiosVersionString(NvPhysicalGpuHandle hPhysicalGpu, NvAPI_ShortString szBiosRevision);

    NVAPI_INTERFACE NvAPI_GPU_GetAGPAperture(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pSize);

    NVAPI_INTERFACE NvAPI_GPU_GetCurrentAGPRate(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pRate);

    NVAPI_INTERFACE NvAPI_GPU_GetCurrentPCIEDownstreamWidth(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pWidth);

    NVAPI_INTERFACE NvAPI_GPU_GetPhysicalFrameBufferSize(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pSize);

    NVAPI_INTERFACE NvAPI_GPU_GetVirtualFrameBufferSize(NvPhysicalGpuHandle hPhysicalGpu, NvU32* pSize);

#define NVAPI_MAX_THERMAL_SENSORS_PER_GPU 3

    typedef enum
    {
        NVAPI_THERMAL_TARGET_NONE = 0,
        NVAPI_THERMAL_TARGET_GPU = 1,
        NVAPI_THERMAL_TARGET_MEMORY = 2,
        NVAPI_THERMAL_TARGET_POWER_SUPPLY = 4,
        NVAPI_THERMAL_TARGET_BOARD = 8,
        NVAPI_THERMAL_TARGET_ALL = 15,
        NVAPI_THERMAL_TARGET_UNKNOWN = -1,
    } NV_THERMAL_TARGET;

    typedef enum
    {
        NVAPI_THERMAL_CONTROLLER_NONE = 0,
        NVAPI_THERMAL_CONTROLLER_GPU_INTERNAL,
        NVAPI_THERMAL_CONTROLLER_ADM1032,
        NVAPI_THERMAL_CONTROLLER_MAX6649,
        NVAPI_THERMAL_CONTROLLER_MAX1617,
        NVAPI_THERMAL_CONTROLLER_LM99,
        NVAPI_THERMAL_CONTROLLER_LM89,
        NVAPI_THERMAL_CONTROLLER_LM64,
        NVAPI_THERMAL_CONTROLLER_ADT7473,
        NVAPI_THERMAL_CONTROLLER_SBMAX6649,
        NVAPI_THERMAL_CONTROLLER_VBIOSEVT,
        NVAPI_THERMAL_CONTROLLER_OS,
        NVAPI_THERMAL_CONTROLLER_UNKNOWN = -1,
    } NV_THERMAL_CONTROLLER;

    typedef struct
    {
        NvU32   version;
        NvU32   count;
        struct
        {
            NV_THERMAL_CONTROLLER       controller;
            NvU32                       defaultMinTemp;
            NvU32                       defaultMaxTemp;
            NvU32                       currentTemp;
            NV_THERMAL_TARGET           target;
        } sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU];

    } NV_GPU_THERMAL_SETTINGS;

#define NV_GPU_THERMAL_SETTINGS_VER  MAKE_NVAPI_VERSION(NV_GPU_THERMAL_SETTINGS,1)

    NVAPI_INTERFACE NvAPI_GPU_GetThermalSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 sensorIndex, NV_GPU_THERMAL_SETTINGS* pThermalSettings);

    typedef enum _NV_DISPLAY_TV_FORMAT
    {
        NV_DISPLAY_TV_FORMAT_NONE = 0,
        NV_DISPLAY_TV_FORMAT_SD_NTSCM = 0x00000001,
        NV_DISPLAY_TV_FORMAT_SD_NTSCJ = 0x00000002,
        NV_DISPLAY_TV_FORMAT_SD_PALM = 0x00000004,
        NV_DISPLAY_TV_FORMAT_SD_PALBDGH = 0x00000008,
        NV_DISPLAY_TV_FORMAT_SD_PALN = 0x00000010,
        NV_DISPLAY_TV_FORMAT_SD_PALNC = 0x00000020,
        NV_DISPLAY_TV_FORMAT_SD_576i = 0x00000100,
        NV_DISPLAY_TV_FORMAT_SD_480i = 0x00000200,
        NV_DISPLAY_TV_FORMAT_ED_480p = 0x00000400,
        NV_DISPLAY_TV_FORMAT_ED_576p = 0x00000800,
        NV_DISPLAY_TV_FORMAT_HD_720p = 0x00001000,
        NV_DISPLAY_TV_FORMAT_HD_1080i = 0x00002000,
        NV_DISPLAY_TV_FORMAT_HD_1080p = 0x00004000,
        NV_DISPLAY_TV_FORMAT_HD_720p50 = 0x00008000,
        NV_DISPLAY_TV_FORMAT_HD_1080p24 = 0x00010000,
        NV_DISPLAY_TV_FORMAT_HD_1080i50 = 0x00020000,
        NV_DISPLAY_TV_FORMAT_HD_1080p50 = 0x00040000,

    } NV_DISPLAY_TV_FORMAT;

#define NVAPI_MAX_SIZEOF_I2C_DATA_BUFFER 256
#define NVAPI_NO_PORTID_FOUND 5
#define NVAPI_DISPLAY_DEVICE_MASK_MAX 24

    typedef struct
    {
        NvU32                   version;
        NvU32                   displayMask;
        NvU8                    bIsDDCPort;
        NvU8                    i2cDevAddress;
        NvU8* pbI2cRegAddress;
        NvU32                   regAddrSize;
        NvU8* pbData;
        NvU32                   cbSize;
        NvU32                   i2cSpeed;
    } NV_I2C_INFO;

#define NV_I2C_INFO_VER  MAKE_NVAPI_VERSION(NV_I2C_INFO,1)

    NVAPI_INTERFACE NvAPI_I2CRead(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO* pI2cInfo);

    NVAPI_INTERFACE NvAPI_I2CWrite(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO* pI2cInfo);

    typedef struct
    {
        NvU32               version;
        NvU32               vendorId;
        NvU32               deviceId;
        NvAPI_ShortString   szVendorName;
        NvAPI_ShortString   szChipsetName;
        NvU32               flags;
        NvU32               subSysVendorId;
        NvU32               subSysDeviceId;
        NvAPI_ShortString   szSubSysVendorName;
    } NV_CHIPSET_INFO;

#define NV_CHIPSET_INFO_VER     MAKE_NVAPI_VERSION(NV_CHIPSET_INFO,3)

    typedef enum
    {
        NV_CHIPSET_INFO_HYBRID = 0x00000001,
    } NV_CHIPSET_INFO_FLAGS;

    typedef struct
    {
        NvU32               version;
        NvU32               vendorId;
        NvU32               deviceId;
        NvAPI_ShortString   szVendorName;
        NvAPI_ShortString   szChipsetName;
        NvU32               flags;
    } NV_CHIPSET_INFO_v2;

#define NV_CHIPSET_INFO_VER_2   MAKE_NVAPI_VERSION(NV_CHIPSET_INFO_v2,2)

    typedef struct
    {
        NvU32               version;
        NvU32               vendorId;
        NvU32               deviceId;
        NvAPI_ShortString   szVendorName;
        NvAPI_ShortString   szChipsetName;
    } NV_CHIPSET_INFO_v1;

#define NV_CHIPSET_INFO_VER_1  MAKE_NVAPI_VERSION(NV_CHIPSET_INFO_v1,1)

    NVAPI_INTERFACE NvAPI_SYS_GetChipSetInfo(NV_CHIPSET_INFO* pChipSetInfo);

    typedef struct
    {
        NvU32 version;
        NvU32 currentLidState;
        NvU32 currentDockState;
        NvU32 currentLidPolicy;
        NvU32 currentDockPolicy;
        NvU32 forcedLidMechanismPresent;
        NvU32 forcedDockMechanismPresent;
    }NV_LID_DOCK_PARAMS;

#define NV_LID_DOCK_PARAMS_VER  MAKE_NVAPI_VERSION(NV_LID_DOCK_PARAMS,1)

    NVAPI_INTERFACE NvAPI_SYS_GetLidAndDockInfo(NV_LID_DOCK_PARAMS* pLidAndDock);

#define NVAPI_OGLEXPERT_DETAIL_NONE                 0x00000000
#define NVAPI_OGLEXPERT_DETAIL_ERROR                0x00000001
#define NVAPI_OGLEXPERT_DETAIL_SWFALLBACK           0x00000002
#define NVAPI_OGLEXPERT_DETAIL_BASIC_INFO           0x00000004
#define NVAPI_OGLEXPERT_DETAIL_DETAILED_INFO        0x00000008
#define NVAPI_OGLEXPERT_DETAIL_PERFORMANCE_WARNING  0x00000010
#define NVAPI_OGLEXPERT_DETAIL_QUALITY_WARNING      0x00000020
#define NVAPI_OGLEXPERT_DETAIL_USAGE_WARNING        0x00000040
#define NVAPI_OGLEXPERT_DETAIL_ALL                  0xFFFFFFFF

#define NVAPI_OGLEXPERT_REPORT_NONE                 0x00000000
#define NVAPI_OGLEXPERT_REPORT_ERROR                0x00000001
#define NVAPI_OGLEXPERT_REPORT_SWFALLBACK           0x00000002
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_VERTEX      0x00000004
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_GEOMETRY    0x00000008
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_XFB         0x00000010
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_RASTER      0x00000020
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_FRAGMENT    0x00000040
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_ROP         0x00000080
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_FRAMEBUFFER 0x00000100
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_PIXEL       0x00000200
#define NVAPI_OGLEXPERT_REPORT_PIPELINE_TEXTURE     0x00000400
#define NVAPI_OGLEXPERT_REPORT_OBJECT_BUFFEROBJECT  0x00000800
#define NVAPI_OGLEXPERT_REPORT_OBJECT_TEXTURE       0x00001000
#define NVAPI_OGLEXPERT_REPORT_OBJECT_PROGRAM       0x00002000
#define NVAPI_OGLEXPERT_REPORT_OBJECT_FBO           0x00004000
#define NVAPI_OGLEXPERT_REPORT_FEATURE_SLI          0x00008000
#define NVAPI_OGLEXPERT_REPORT_ALL                  0xFFFFFFFF

#define NVAPI_OGLEXPERT_OUTPUT_TO_NONE              0x00000000
#define NVAPI_OGLEXPERT_OUTPUT_TO_CONSOLE           0x00000001
#define NVAPI_OGLEXPERT_OUTPUT_TO_DEBUGGER          0x00000004
#define NVAPI_OGLEXPERT_OUTPUT_TO_CALLBACK          0x00000008
#define NVAPI_OGLEXPERT_OUTPUT_TO_ALL               0xFFFFFFFF

    typedef void (*NVAPI_OGLEXPERT_CALLBACK) (unsigned int categoryId, unsigned int messageId, unsigned int detailLevel, int objectId, const char* messageStr);

    NVAPI_INTERFACE NvAPI_OGL_ExpertModeSet(NvU32 expertDetailLevel,
        NvU32 expertReportMask,
        NvU32 expertOutputMask,
        NVAPI_OGLEXPERT_CALLBACK expertCallback);

    NVAPI_INTERFACE NvAPI_OGL_ExpertModeGet(NvU32* pExpertDetailLevel,
        NvU32* pExpertReportMask,
        NvU32* pExpertOutputMask,
        NVAPI_OGLEXPERT_CALLBACK* pExpertCallback);

    NVAPI_INTERFACE NvAPI_OGL_ExpertModeDefaultsSet(NvU32 expertDetailLevel,
        NvU32 expertReportMask,
        NvU32 expertOutputMask);

    NVAPI_INTERFACE NvAPI_OGL_ExpertModeDefaultsGet(NvU32* pExpertDetailLevel,
        NvU32* pExpertReportMask,
        NvU32* pExpertOutputMask);

#define NVAPI_MAX_VIEW_TARGET  2

    typedef enum _NV_TARGET_VIEW_MODE
    {
        NV_VIEW_MODE_STANDARD = 0,
        NV_VIEW_MODE_CLONE = 1,
        NV_VIEW_MODE_HSPAN = 2,
        NV_VIEW_MODE_VSPAN = 3,
        NV_VIEW_MODE_DUALVIEW = 4,
        NV_VIEW_MODE_MULTIVIEW = 5,
    } NV_TARGET_VIEW_MODE;

    typedef enum _NV_SCALING
    {
        NV_SCALING_DEFAULT = 0,
        NV_SCALING_MONITOR_SCALING = 1,
        NV_SCALING_ADAPTER_SCALING = 2,
        NV_SCALING_CENTERED = 3,
        NV_SCALING_ASPECT_SCALING = 5,
        NV_SCALING_CUSTOMIZED = 255
    } NV_SCALING;

    typedef enum _NV_ROTATE
    {
        NV_ROTATE_0 = 0,
        NV_ROTATE_90 = 1,
        NV_ROTATE_180 = 2,
        NV_ROTATE_270 = 3
    } NV_ROTATE;

    typedef enum _NV_FORMAT
    {
        NV_FORMAT_UNKNOWN = 0,
        NV_FORMAT_P8 = 41,
        NV_FORMAT_R5G6B5 = 23,
        NV_FORMAT_A8R8G8B8 = 21,
        NV_FORMAT_A16B16G16R16F = 113
    } NV_FORMAT;

    typedef struct
    {
        NvU32 version;
        NvU32 count;
        struct
        {
            NvU32 deviceMask;
            NvU32 sourceId;
            NvU32 bPrimary : 1;

            NvU32 bInterlaced : 1;
            NvU32 bGDIPrimary : 1;
        } target[NVAPI_MAX_VIEW_TARGET];
    } NV_VIEW_TARGET_INFO;

#define NV_VIEW_TARGET_INFO_VER  MAKE_NVAPI_VERSION(NV_VIEW_TARGET_INFO,2)

    NVAPI_INTERFACE NvAPI_SetView(NvDisplayHandle hNvDisplay, NV_VIEW_TARGET_INFO* pTargetInfo, NV_TARGET_VIEW_MODE targetView);

    NVAPI_INTERFACE NvAPI_GetView(NvDisplayHandle hNvDisplay, NV_VIEW_TARGET_INFO* pTargets, NvU32* pTargetMaskCount, NV_TARGET_VIEW_MODE* pTargetView);

#define NVAPI_MAX_DISPLAY_PATH  NVAPI_MAX_VIEW_TARGET

    typedef struct
    {
        NvU32 version;
        NvU32 count;
        struct
        {
            NvU32                   deviceMask;
            NvU32                   sourceId;
            NvU32                   bPrimary : 1;

            NV_GPU_CONNECTOR_TYPE   connector;

            NvU32                   width;
            NvU32                   height;
            NvU32                   depth;
            NV_FORMAT               colorFormat;

            NV_ROTATE               rotation;

            NV_SCALING              scaling;

            NvU32                   refreshRate;
            NvU32                   interlaced : 1;

            NV_DISPLAY_TV_FORMAT    tvFormat;

            NvU32                   posx;
            NvU32                   posy;
            NvU32                   bGDIPrimary : 1;

        } path[NVAPI_MAX_DISPLAY_PATH];
    } NV_DISPLAY_PATH_INFO;

#define NV_DISPLAY_PATH_INFO_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_PATH_INFO,2)

    NVAPI_INTERFACE NvAPI_SetViewEx(NvDisplayHandle hNvDisplay, NV_DISPLAY_PATH_INFO* pPathInfo, NV_TARGET_VIEW_MODE displayView);

    NVAPI_INTERFACE NvAPI_GetViewEx(NvDisplayHandle hNvDisplay, NV_DISPLAY_PATH_INFO* pPathInfo, NvU32* pPathCount, NV_TARGET_VIEW_MODE* pTargetViewMode);

    NVAPI_INTERFACE NvAPI_GetSupportedViews(NvDisplayHandle hNvDisplay, NV_TARGET_VIEW_MODE* pTargetViews, NvU32* pViewCount);

#ifdef __cplusplus
};
#endif

#pragma pack(pop)

#endif
