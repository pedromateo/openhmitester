
**10/06/2016 Working on a Windows version. Please, be patient for a working version, and if you want to contribute, just email me or check out his TODO list:**

- *TODO_1 (already done)*: Try to integrate the "preload library" into the workbench desktop
app (called Simusaes) manually. By manually I mean to include a line
in "Simusaes" main function in which the preload action is called
(this should be called automatically, read below). This is almost
implemented.

  To do this, please, take a look at: openhmitester/testbench/desktop/main.cpp, line 12
 

- *TODO_2*: Once everything is working as expected (i.e., the OHT records and
replays without any problem), I should try to execute the preload
action automatically. In linux it is implemented using the LD_PRELOAD
environment variable. In windows I read about including the path of
the library to preload into a registry key.

  For this, openhmitester/src/preloaders/winpreloadingaction.cpp should be properly implemented. Right now it holds the implementation for linux systems, but I will clean and prepare this class asap.

  **PreloadModule** in the library used to extract events from the
application under test (AUT), and to execute actions on it as well.
This module (encapsulated into a DLL library) has to:

  1. be preloaded into the AUT before the AUT is launched.
  2. detect any event (in linux we use an event called something like Qt nativeEvent) to automatically execute at startup and deploy the OHT services, thus the OHT controller will be able to communicate to and control the AUT.


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

With OHT you can create test cases for your application. Then, test cases can be replayed one by one, or all in a row.

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/PiwPB8uwZOk/0.jpg)](http://www.youtube.com/watch?v=PiwPB8uwZOk)

https://www.youtube.com/watch?v=PiwPB8uwZOk

## Recording and playing web test cases

With OHT you can create test cases for your webapp. Then, test cases can be replayed one by one, or all in a row.

OHT provides a free, easy and fast way to test your web, as well as to automate actions.

Note that this might produce unexpected results, as in the current implementation of the OHT, the test is recorded in the browser, not in the web page. We are working hard to provide a robust web-testing experience.

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/Smcj3WmdPdQ/0.jpg)](http://www.youtube.com/watch?v=Smcj3WmdPdQ)

https://www.youtube.com/watch?v=Smcj3WmdPdQ

## Logging in tested application

Two logs are generated with the standard and error output of the Application Under Test (AUT): 
* oht_aut_stdout.log
* oht_aut_stderr.log

They are located in the same directory where the OHT binary is located. This decision was taken in order to use a cross-platform location.

Please, note that lib_preload output is also included in these log files.


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

### The AUT is crashing and I don't know why. Are there any logs to see what's happening?

Two logs are generated with the standard and error output of the Application Under Test (AUT): 
* oht_aut_stdout.log
* oht_aut_stderr.log

They are located in the same directory where the OHT binary is located. 

# Further information

Webpage: http://catedrasaes.org/html/projects/OHT/OHT.html

More videos: http://www.youtube.com/user/CatedraSaesUmu




