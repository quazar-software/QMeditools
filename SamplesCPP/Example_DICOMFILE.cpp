#include "pch.h"

#include <QMediToolsCore.h>

// Get DICOM tag from message object
static std::wstring getAttributeStr(HQMTMSG h, USHORT g, USHORT e)
{
	wchar_t szData[512];

	szData[0] = L'\0';
	HRESULT err = QMTGetAttributeStr(h, g, e, szData, sizeof(szData)/sizeof(wchar_t));
	if (err == S_FALSE) {
		std::cout << "Tag not found (" << std::hex << g << "," << std::hex << e << ")\n";
	}
	else if (err != 0) {
		std::cerr << "ERROR: Failed to read tag (" << std::hex << g << "," << std::hex << e << ")\n";
	}
	return std::wstring(szData);
}

// Parse a DICOM file and output some data
void do_dicomfile(void)
{
	std::wstring sFilename = L"d:\\buf\\test-images\\2Pat2Std2Ser2Img\\img00001.dcm";
	HQMTMSG hFile = QMTReadMessageFromFile(sFilename.c_str(),FALSE);

	if (hFile) {
		std::cout <<  "--DICOM Instance----------------------------------------------------\n";
		std::wcout << "Name  : " << getAttributeStr(hFile, 0x10, 0x10) << " ID: " << getAttributeStr(hFile, 0x10, 0x20) << std::endl;
		std::wcout << "Study : " << getAttributeStr(hFile, 0x20, 0x10) << " from " << getAttributeStr(hFile, 0x08, 0x20) << std::endl;
		std::wcout << "Series: " << getAttributeStr(hFile, 0x20, 0x11) << " from " << getAttributeStr(hFile, 0x08, 0x21) << std::endl;
		std::wcout << "SOP   : " << getAttributeStr(hFile, 0x08, 0x18) << " " << getAttributeStr(hFile, 0x08, 0x16) << std::endl;

		QMTFreeMessage(hFile);
	}
	else
		std::cerr << "ERROR: Failed to read DICOM file\n";
}

