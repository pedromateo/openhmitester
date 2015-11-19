OHT: Open HMI Tester
====================

"Open HMI Tester" or OHT is an application framework for the development of **GUI testing tools**. It uses
GUI **introspection** to capture and simulate **real user interaction**, which enhances
robustness and tolerance to changes during testing stage. 

OHT provides a **cross-platform, open design** to support major event-based GUI platforms. Furthermore,
it can be integrated into ongoing and legacy developments due to it being **not code-intrusive**. 
As a result, the framework provides an adaptable, extensible, scalable,
and **robust basis** to support the automation of GUI testing processes. 

OHT is open-source and ready to use.

OHT can be adapted to **support different GUI platforms** and operating systems. An adaptation to Qt-Linux
testing environments is provided in this repository.

# Any question? Any bug?

Please, contact me at pedrolmn@gmail.com

# OHT in action

## Recording and playing desktop test cases

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/PiwPB8uwZOk/0.jpg)](http://www.youtube.com/watch?v=PiwPB8uwZOk)

https://www.youtube.com/watch?v=PiwPB8uwZOk

## Recording and playing web test cases

Note that this might produce unexpected results, as in the current implementation of the OHT, the test is recorded in the browser, not in the web page. We are working hard to provide a robust web-testing experience.

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/Smcj3WmdPdQ/0.jpg)](http://www.youtube.com/watch?v=Smcj3WmdPdQ)

https://www.youtube.com/watch?v=Smcj3WmdPdQ


# Content of this repository

## OHT base architecture

* common: includes common sources.
* hmi_tester: includes implementation of the OHT controller
* lib_preload: includes implementation of the library injected in the application to test.
* build_oht_base: Qt Creator project to build the base architecture.



## Qt-Linux OHT Adaptation

* qt_linux_hmi_tester: implementation of the OHT controller for a Qt-Linux testing environment.
* qt_linux_lib_preload: implementation of the injected library for a Qt-Linux testing environment.
* build_oht_qt_linux: Qt Creator project to build the Qt-Linux GUI testing tool.

# FAQ

### How to adapt OHT to Windows environments?

1. In qt_linux_hmi_tester, find the class linuxpreloadingaction.h. 
2. Create a similar class to support library preloading in windows.
3. In qt_linux_lib_preload, find qtx11preloadingcontrol class.
4. Create a similar class to "wake up" the OHT at application startup and start OHT installation process.

### How does the injection + preloading process works?

1. The class doing DLL injection before application launching is called LinuxPreloadingAction (extends PreloadingAction) and it is in the HMI Tester. In Linux, it uses the environment variable LD_PRELOAD to set the library to be preloaded before target application launching.

2. The class deploying the OHT services into the target application is QtX11PreloadingControl (extends PreloadingControl) and it is in the Lib Preload. In Linux, it uses QWidget::x11Event in Qt4, or QWidget::nativeEvent in Qt5 to "automatically wake up" and start deploying event consumer and executor.

### I am running an application using Qt 4.8 + Embedded Linux without X server (there is QWS from QtEmbedded built in my application instead). Does OHT support this kind of setup?

OHT will support this kind of setup provided that you can "wake up" OHT within your application. With waking up I mean to find an event that is executed at your application launching, so you can handle it and start deploying OHT services.

In the Qt-Linux implementation, the class deploying the OHT services into the target application is QtX11PreloadingControl (extends PreloadingControl) and it is in the Lib Preload. In Linux, it uses QWidget::x11Event in Qt4, or QWidget::nativeEvent in Qt5 to "automatically wake up" and start deploying event consumer and executor.

So, you need to create and adaptation of this class (e.g., QWSPreloadingControl) and:
* either you find a generic Qt4 event that executes at application startup, similar to QWidget::nativeEvent in Qt5
* or you find a QWS event equivalent to QWidget::x11Event in Qt4

# Further information

Webpage: http://www.catedrasaes.org/wiki/OHT

Videos: http://www.youtube.com/user/CatedraSaesUmu




