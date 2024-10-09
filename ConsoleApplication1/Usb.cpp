#include "Usb.h"


#include "Usb.h"
#include <iostream>
#include <SetupAPI.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>
#include <initguid.h>
#include <Usbiodef.h>
#include <winerror.h>
#include <regex>
#include <atlstr.h>

#pragma comment(lib, "setupapi.lib")

HANDLE hDevice = INVALID_HANDLE_VALUE;
int sendData_Usb(const unsigned char* data);
bool sendDataSz_Usb(const unsigned char* data, int length);
int receiveData_Usb(unsigned char* buffer, int bufferLength);

 const char* getDllVersion_Usb(void)
{
	return 0;
}
// function
#pragma region SDK_FUNCTION

#pragma endregion
// connect , send , receive 
#pragma region SDK_IO

 bool openPort_Usb()
{
	 std::vector<std::wstring> devicePaths;
	 HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVINTERFACE_USB_DEVICE, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	 if (deviceInfoSet == INVALID_HANDLE_VALUE) {
		 std::cerr << "獲取設備信息集合失敗。錯誤: " << GetLastError() << std::endl;
		 return false;
	 }

	 SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
	 deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	 DWORD i = 0;
	 std::wstring devicePath;
	 bool foundDevice = false;

	 while (SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &GUID_DEVINTERFACE_USB_DEVICE, i, &deviceInterfaceData)) {
		 DWORD requiredSize = 0;
		 if (!SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, NULL)) {
			 if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
				 std::cerr << "SetupDiGetDeviceInterfaceDetail失敗（初始）。錯誤: " << GetLastError() << std::endl;
				 ++i;
				 continue;
			 }
		 }

		 std::vector<BYTE> buffer(requiredSize);
		 SP_DEVICE_INTERFACE_DETAIL_DATA* deviceInterfaceDetailData = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(buffer.data());
		 deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		 SP_DEVINFO_DATA deviceInfoData;
		 deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		 if (!SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceInterfaceDetailData, requiredSize, NULL, &deviceInfoData)) {
			 std::cerr << "SetupDiGetDeviceInterfaceDetail失敗。錯誤: " << GetLastError() << std::endl;
			 ++i;
			 continue;
		 }

		 devicePath = deviceInterfaceDetailData->DevicePath;

		 // 檢查設備路徑中的VID
		 std::wregex vidRegex_1(L"vid_0471");
		 std::wregex vidRegex_2(L"vid_1203");
		 std::wregex vidRegex_3(L"vid_0512");
		 if (std::regex_search(devicePath, vidRegex_1) || std::regex_search(devicePath, vidRegex_2) || std::regex_search(devicePath, vidRegex_3)) {
			 foundDevice = true;
			 break;
		 }

		 ++i;
	 }

	 SetupDiDestroyDeviceInfoList(deviceInfoSet);

	 if (!foundDevice) {
		 std::cerr << "未找到指定VID的USB設備。" << std::endl;
		 return false;
	 }

	 hDevice = CreateFile(devicePath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	 if (hDevice == INVALID_HANDLE_VALUE) {
		 std::cerr << "打開USB端口失敗。錯誤: " << GetLastError() << std::endl;
		 return false;
	 }

	 return true;
}

 void closePort_Usb()
{
	if (hDevice != INVALID_HANDLE_VALUE) {
		CloseHandle(hDevice);
		hDevice = INVALID_HANDLE_VALUE;
	}
}

int sendData_Usb(const unsigned char* data)
{
	if (hDevice == INVALID_HANDLE_VALUE) {
		std::cerr << "Device not connected." << std::endl;
		return G_RETURN_ERROR;
	}

	DWORD bytesWritten;
	if (!WriteFile(hDevice, data, strlen(reinterpret_cast<const char*>(data)), &bytesWritten, NULL)) {
		std::cerr << "Failed to write data. Error: " << GetLastError() << std::endl;
		return G_RETURN_ERROR;
	}

	return G_RETURN_OK;
}

bool sendDataSz_Usb(const unsigned char* data, int length)
{

	if (hDevice == INVALID_HANDLE_VALUE) {
		std::cerr << "Device not connected." << std::endl;
		return false;
	}

	DWORD bytesWritten;
	if (!WriteFile(hDevice, data, length, &bytesWritten, NULL)) {
		std::cerr << "Failed to write data. Error: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

int receiveData_Usb(unsigned char* buffer, int bufferLength)
{
	if (hDevice == INVALID_HANDLE_VALUE) {
		std::cerr << "Device not connected." << std::endl;
		return -1;
	}

	DWORD bytesRead;
	if (!ReadFile(hDevice, buffer, bufferLength, &bytesRead, NULL)) {
		std::cerr << "Failed to read data. Error: " << GetLastError() << std::endl;
		return -1;
	}

	return bytesRead;
}
#pragma endregion