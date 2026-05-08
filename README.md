# General

Latest Version: 1.3.3.0

QMediTools is the new Quazar DICOM-Toolbox. The library supports DICOM-File reading and writing through the old COM-Servers now consolidated in QMTDicom.
But the library also contains the QDICNET-Component (exe) to support DICOM-SCU and DICOM-SCP functionality.

[Documentation](https://docs.quazar.de/qmt)

### Dependencies (Backend)

*Quazar-Dll's:* QMTCore, QMTMedia, QMTDicom, QMTWMF (optional), QJpegNAPI.

*Third party DLL's:* openjp2.dll (Open JPEG), turbojpeg.dll (Turbo JPEG)

### Platform 

Windows

### Examples

Some c++ examples how to use the QMediTools API (QMeditoolsCore)

### QMediTools.exe

Some c# example how to use the QMediTools API (QMeditoolsCore)

### QMediToolsCore (Frontend)

The QMediTools API as dynamic link. This the main project which is intended to be used by other projects.

### QMTTest

The test project is a console application using the QMediToolsCore API to test the functionality of the library. 

Byte the way this application implements a "Enhanced CT" to "Standard CT" conversion.

