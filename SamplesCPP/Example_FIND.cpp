#include "pch.h"
#include <conio.h>

#include <QMediToolsCore.h>

// Query the SCP and dump some data from response
static HRESULT do_find(HQMTSCU hScu, HQMTMSG hm)
{
	HRESULT hr = QMTSendMessage(hScu, hm);
	if (FAILED(hr)) {
		std::cerr << "Failed to send query message " << std::hex << hr << "\n";
		return E_FAIL;
	}
	bool bContinue = true;
	while (bContinue)
	{
		HQMTMSG hrsp = 0;
		hr = QMTReadMessage(hScu, &hrsp);
		if (SUCCEEDED(hr))
		{
			USHORT nStatus;
			hr = QMTGetMessageStatus(hrsp, &nStatus);
			if (SUCCEEDED(hr))
			{
				std::cout << "some data read ...\n";
				bool bStatusNoError = nStatus == 0 || nStatus == 0xff01 || nStatus == 0xff00;
				if (bStatusNoError)
				{
					bContinue = nStatus == 0xff01 || nStatus == 0xff00;
					wchar_t szDicomValue[256];
					uint32_t cbValueSize = (uint32_t)sizeof(szDicomValue) / sizeof(szDicomValue[0]);
					hr = QMTGetAttributeStr(hrsp, 0x0010, 0x0010, szDicomValue, cbValueSize);
					if (hr == S_OK)
					{
						std::wcout << "Name " << szDicomValue << "\n";
					}
					hr = QMTGetAttributeStr(hrsp, 0x0010, 0x0020, szDicomValue, cbValueSize);
					if (hr == S_OK)
					{
						std::wcout << "Id " << szDicomValue << "\n";
					}
					hr = QMTGetAttributeStr(hrsp, 0x0010, 0x0030, szDicomValue, cbValueSize);
					if (hr == S_OK)
					{
						std::wcout << "DOB " << szDicomValue << "\n";
					}
					hr = QMTGetAttributeStr(hrsp, 0x0010, 0x0040, szDicomValue, cbValueSize);
					if (hr == S_OK)
					{
						std::wcout << "Sex " << szDicomValue << "\n";
					}
					if (nStatus == 0) break;
				}
				else
				{
					bContinue = false;
				}
			}
			else
			{
				std::cerr << "reading response failed hr=" << std::hex << hr << "\n";
			}
			QMTFreeMessage(hrsp);
		}
		else
		{
			std::cerr << "reading message failed hr=" << std::hex << hr << "\n";
			break;
		}
	}
	return hr;
}

// Create a empty C-FIND message (query)
static HRESULT do_find(HQMTSCU hScu)
{
	HRESULT hr = QMTNegotiateAssociation(hScu, L"QRS-FIND-FULL");
	if (SUCCEEDED(hr)) {
		HQMTMSG hm = QMTCreateFindMessage(L"PROOT", L"PATIENT");
		if (hm) {
			do_find(hScu, hm);
			QMTFreeMessage(hm);
		}
		QMTReleaseAssociation(hScu);
	}
	else {
		std::cerr << "ERROR: Failed to negotiate DICOM service class hr=" << std::hex << hr << "\n";
	}
	return hr;
}

// Connect to SCP
void do_find(void)
{
	HQMTSCU hScu;
	hScu = QMTCreateSCU();
	if (hScu == 0) {
		std::cout << "Failed to create SCU class\n";
		return;
	}
	std::cout << "connecting...\n";
	HRESULT hr = QMTConnect(hScu, L"MY-AE", L"REMOTE_AE", L"localhost", 104);
	if (SUCCEEDED(hr)) {
		std::cout << "connected to localhost on 104\n";
		do_find(hScu);
		QMTDisconnect(hScu);
	}
	else
		std::cerr << "ERROR: Failed to connect SCP hr=" << std::hex << hr << "\n";
	QMTDestroySCU(hScu);
}

