#include "pch.h"

#include <QMediToolsCore.h>

// Negotiate ECHO-Service and send a message. Waiting on SCP response and check the status code.
void do_echo(HQMTSCU hScu)
{
	HRESULT hr = QMTNegotiateAssociation(hScu, L"ECHO");
	if (SUCCEEDED(hr)) {
		// send a C-ECHO Message
		HQMTMSG hm = QMTCreateEchoMessage();
		if (hm) {
			hr = QMTSendMessage(hScu, hm);
			if (SUCCEEDED(hr)) {
				// read response 
				HQMTMSG hrsp = 0;
				hr = QMTReadMessage(hScu, &hrsp);
				if (SUCCEEDED(hr))
				{
					USHORT nStatus;
					hr = QMTGetMessageStatus(hrsp, &nStatus);
					if (SUCCEEDED(hr)) {
						std::cout << "ping nStatus(" << nStatus << ")";
					}
					else {
						std::cerr << "reading response failed hr=" << std::hex << hr << "\n";
					}
					QMTFreeMessage(hrsp);
				}
				else {
					std::cerr << "reading response failed hr=" << std::hex << hr << "\n";
				}
			} else
				std::cerr << "Failed to send message hr=" << std::hex << hr << "\n";
			QMTFreeMessage(hm);
		}
		QMTReleaseAssociation(hScu);
	}
	else {
		std::cerr << "ERROR: Failed to negotiate DICOM service class hr=" << std::hex << hr << "\n";
	}
}

// Connect to SCP
void do_echo(void)
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
		do_echo(hScu);
		QMTDisconnect(hScu);
	}
	else
		std::cerr << "ERROR: Failed to connect SCP hr=" << std::hex << hr << "\n";
	QMTDestroySCU(hScu);
}
