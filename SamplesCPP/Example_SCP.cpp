#include "pch.h"
#include <conio.h>

#include <QMediToolsCore.h>

// Create a SCP instance an start listing for incoming messages
void do_scp()
{
	HQMTSCP hScp = QMTCreateSCP();
	if (!hScp) return;

	QMTSetOption(L"outfolder", L"D:\\buf\\in-coming");
	QMTSetOption(L"serverlocalae", L"MYSCP");
	QMTSetOption(L"scpservicelist",L"all");
	QMTSetOption(L"logoutput", L"console");
	HRESULT hr;
	hr = QMTDoListen(hScp);
	if (SUCCEEDED(hr)) {
		for (;;) {
			if (_kbhit()) {
				int c = _getch();
				if (c == 'q' || c == 'Q' || c == 0x1b)
					break;
				else
					std::cout << "hit <q> or <esc> to terminate server.\n";
			}
			QMT_SCPEVENT info;
			hr = QMTWaitOnIncoming(hScp, &info);
			if (FAILED(hr)) break;
			if (hr == S_OK) {
				if (info.nEvent == QMT_EVENT_CSTORE_REQ) {
					std::wcout << "Event: File Stored: " << info.szFileName << " PTX: "
						<< info.szAbstractSyntax << "/" << info.szSOPInstanceUID  << "\n";
				}
				else if (info.nEvent == QMT_EVENT_NEW_ASSOCIATION) {
					std::wcout << "Event: New Client connected [" << info.szFileName << "]\n";
				}
				else  if (info.nEvent == QMT_EVENT_ECHO_REQ) {
					std::wcout << "Event: Got some ping\n";
				}
				else  if (info.nEvent == QMT_EVENT_ASSOCIATION_RELEASED) {
					std::wcout << "Event: Client disconnected [" << info.szFileName << "]\n";
				}
			}
		}
		QMTStopListen(hScp);
	}
	else
		std::cerr << "ERROR: Failed to listen on tcp port";
	QMTDestroySCP(hScp);
}

