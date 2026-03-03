#include "pch.h"
#include <conio.h>

#include <QMediToolsCore.h>

// dump response helper
static void dump_move_info(HQMTMSG hrsp)
{
	wchar_t szDicomValue[256];
	uint32_t cbValueSize = (uint32_t)sizeof(szDicomValue) / sizeof(szDicomValue[0]);
	HRESULT hr = QMTGetAttributeStr(hrsp, 0x0000, 0x1020, szDicomValue, cbValueSize);
	std::wstring sRemaing = L"?", sCompleted = L"?", sFailed = L"?", sWarning = L"?";
	hr = QMTGetAttributeStr(hrsp, 0x0000, 0x1020, szDicomValue, cbValueSize);
	if (SUCCEEDED(hr)) sRemaing = szDicomValue;
	hr = QMTGetAttributeStr(hrsp, 0x0000, 0x1021, szDicomValue, cbValueSize);
	if (SUCCEEDED(hr)) sCompleted = szDicomValue;
	hr = QMTGetAttributeStr(hrsp, 0x0000, 0x1022, szDicomValue, cbValueSize);
	if (SUCCEEDED(hr)) sFailed = szDicomValue;
	hr = QMTGetAttributeStr(hrsp, 0x0000, 0x1023, szDicomValue, cbValueSize);
	if (SUCCEEDED(hr)) sWarning = szDicomValue;

	std::wcout << L"R:" << sRemaing << L", C:" << sCompleted << L", F:" << sFailed << L", W:" << sWarning << L"\n";
}

// Query the SCP and dump some data from response
static HRESULT do_move(HQMTSCU hScu, HQMTMSG hm)
{
	HRESULT hr = QMTSendMessage(hScu, hm);
	if (FAILED(hr)) {
		std::cerr << "Failed to send move message " << std::hex << hr << "\n";
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
				std::cout << "Status=" << std::hex << nStatus;
				bool bStatusNoError = nStatus == 0 || nStatus == 0xff01 || nStatus == 0xff00;
				if (bStatusNoError)
				{
					bContinue = nStatus == 0xff01 || nStatus == 0xff00;
					dump_move_info(hrsp);
					if (nStatus == 0) break;
				}
				else
				{
					bContinue = false;
				}
			}
			else
			{
				std::cerr << "reading response failed hr" << std::hex << hr;
			}
			QMTFreeMessage(hrsp);
		}
		else
		{
			std::cerr << "reading message failed hr" << std::hex << hr;
			break;
		}
	}
	return true;
}

// Create a empty C-MOVE message (query)
static HRESULT do_move(HQMTSCU hScu)
{
	HRESULT hr = QMTNegotiateAssociation(hScu, L"QRS-MOVE-FULL");
	if (SUCCEEDED(hr)) {
		HQMTMSG hm = QMTCreateMoveMessage(L"PROOT", L"PATIENT", L"DEST-AE");
		if (hm) {
			do_move(hScu, hm);
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
void do_move()
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
		do_move(hScu);
		QMTDisconnect(hScu);
	}
	else
		std::cerr << "ERROR: Failed to connect SCP hr=" << std::hex << hr << "\n";
	QMTDestroySCU(hScu);
}