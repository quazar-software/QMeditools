#include "pch.h"

#include <QMediToolsCore.h>

// Just connect to a TCP peer. No DICOM negotiation was performed here
void do_connect(void) 
{
	HQMTSCU hScu;
	hScu = QMTCreateSCU();
	if (hScu == 0) {
		std::cerr << "Failed to create SCU class\n";
		return;
	}
	std::cout << "connecting...\n";
	HRESULT hr = QMTConnect(hScu, L"MY-AE", L"REMOTE_AE", L"localhost", 104);
	if (SUCCEEDED(hr)) {
		std::cout << "connected to localhost on 104\n";
		QMTDisconnect(hScu);
	} else 
		std::cerr << "ERROR: Failed to connect SCP hr=" << std::hex << hr << "\n";
	QMTDestroySCU(hScu);
}
