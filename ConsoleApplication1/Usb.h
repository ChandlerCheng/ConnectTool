#include <Windows.h>
#include <string>
#include <vector>

#define G_RETURN_ERROR 0
#define G_RETURN_OK 1

 const char* getDllVersion_Usb(void);
 bool openPort_Usb(void);
 void closePort_Usb(void);
