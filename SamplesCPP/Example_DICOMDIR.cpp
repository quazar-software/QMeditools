#include "pch.h"

#include <QMediToolsCore.h>

// Get DICOM tag from message object
static std::wstring getAttributeStr(HQMTMSG h, USHORT g, USHORT e)
{
	wchar_t szData[512];

	szData[0] = L'\0';
	HRESULT err = QMTGetAttributeStr(h, g, e, szData, sizeof(szData) / sizeof(wchar_t));
	if (err == S_FALSE) {
		std::cout << "Tag not found (" << std::hex << g << "," << std::hex << e << ")\n";
	}
	else if (err != 0) {
		std::cerr << "ERROR: Failed to read tag (" << std::hex << g << "," << std::hex << e << ")\n";
	}
	return std::wstring(szData);
}

// Enumerate instance/image records from DICOM catalog
static bool enumFiles(HQMTMSG h, const std::wstring &sCatalogPath)
{
	HQMTMSG hRec;
	for (hRec = h; hRec;) {
		if (getAttributeStr(hRec, 0x4, 0x1430) == L"IMAGE") {
			std::wcout << L"found '" << getAttributeStr(hRec,0x4,0x1500) << L"'\n";
		}
		HQMTMSG hLowerLevel;
		if (hLowerLevel = QMTGetLowerLevelEntity(hRec)) {
			enumFiles(hLowerLevel, sCatalogPath);
		}
		HQMTMSG hNxt = QMTGetNextDirRecord(hRec);
		QMTFreeMessage(hRec);
		hRec = hNxt;
	}
	return true;
}

// Load DICOMDIR from folder
void do_dicomdir()
{
	std::wstring sPath = L"d:\\buf\\test-images\\2Pat2Std2Ser2Img";
	HQMTMSG hDir = QMTDicomDirReader((sPath+L"\\DICOMDIR").c_str());

	if (hDir) {
		HQMTMSG hRec = QMTGetFirstDirRecord(hDir);
		if (hRec != 0) enumFiles(hRec, sPath);
		QMTFreeMessage(hDir);
		return;
	} else
		std::cerr << "ERROR: Failed to read DICOMDIR\n";
}
