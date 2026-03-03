// SamplesCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <QMediToolsCore.h>

extern void do_connect(void);
extern void do_echo(void);
extern void do_scp(void);
extern void do_dicomdir(void);
extern void do_dicomfile(void);
extern void do_find(void);
extern void do_move(void);
extern void do_store(void);

int main()
{
    QMTSetOption(L"logoutput", L"console");
    QMTSetupLogger(L"d:\\Developer\\git\\QMediTools\\Deploy\\test.log", 6, 0);

    // optional set your license information
    QMTSetOption(L"LicenseKey", L"00000-00000-00000-00000-00000");

    // do something
    do_scp();
    //do_dicomfile();
    //do_dicomdir();
    //do_echo();
    //do_find();
    //do_store(); 
}

