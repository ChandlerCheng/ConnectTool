// 回傳值標準
#define G_RETURN_OK			1
#define G_RETURN_ERROR	0
#define G_IMAGE_DOWNLOAD_PATH L"D:\\Project\\GiLab\\CPlusSdk_Receipt\\CPlusDll\\outputDllReszie.bmp"

int GetWindowsFontData(const char* text, const char* fontName, int fontSize, unsigned char** dstAddress, int* widthOut, int* heightOut, int rotationAngle);
int ReturnBitmap(const char* filename, unsigned char** dstAddress, int* widthOut, int* heightOut);
int ReturnBitmap_Scale(const char* filename, unsigned char** dstAddress, int width, int height);
int ReturnBitmap_Scale_Rotate(const char* filename, unsigned char** dstAddress, int widthOut, int heightOut, int rotate);