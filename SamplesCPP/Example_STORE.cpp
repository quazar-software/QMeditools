#include "pch.h"
#include <conio.h>

#include <QMediToolsCore.h>

// Send the DICOM message and wait for SCP response
HRESULT do_store(HQMTSCU hSCU, HQMTMSG hm)
{
	DWORD dwTick = GetTickCount();
	std::cout << "sending...";
	HRESULT hr = QMTSendMessage(hSCU, hm);
	if (FAILED(hr)) {
		std::cerr << "Failed to send store message hr=" << std::hex << hr << "\n";
		return E_FAIL;
	}
	std::cout << GetTickCount() - dwTick << " ms" << "\n";
	HQMTMSG hrsp = 0;
	std::cout << "waiting response...\n";
	hr = QMTReadMessage(hSCU, &hrsp);
	if (SUCCEEDED(hr))
	{
		USHORT nStatus;
		hr = QMTGetMessageStatus(hrsp, &nStatus);
		if (SUCCEEDED(hr)) {
			if(nStatus==0) std::cout << "File sent (Status=" << std::hex << nStatus << ")\n";
			else std::cout << "Warning: Status=" << std::hex << nStatus << "\n";
		}
		else {
			std::cerr << "Reading response failed hr=" << std::hex << hr << "\n";
		}
		QMTFreeMessage(hrsp);
	}
	else
	{
		std::cerr << "Reading message failed hr=" << std::hex << hr << "\n";
	}
	return true;
}

// Create a DICOM message from file (C-STORE)
HRESULT do_store(HQMTSCU hScu)
{
	HRESULT hr = QMTNegotiateAssociation(hScu, L"STORE-XA");
	if (SUCCEEDED(hr)) {
		std::wstring sFilename = L"d:\\buf\\test-images\\2Pat2Std2Ser2Img\\img00001.dcm";
		HQMTMSG hm = QMTCreateStoreMessageFromFile(sFilename.c_str());
		if (hm) {
			do_store(hScu, hm);
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
void do_store()
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
		do_store(hScu);
		QMTDisconnect(hScu);
	}
	else
		std::cerr << "ERROR: Failed to connect SCP hr=" << std::hex << hr << "\n";
	QMTDestroySCU(hScu);
}