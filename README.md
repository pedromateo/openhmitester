
**July, the 1st -> OHT is now cross-platform and works both in Linux (automatic preload) and Windows (using manual preload). There is one feature to implement: make "preload" to work in Windows. If you want to contribute, just email me or [continue reading here](#todo-windows-adaptation)**

---

"Open HMI Tester" or OHT is an application framework for the development of **GUI testing tools**. It uses real-time
GUI **introspection** to capture and simulate **real user interaction**, which enhances
robustness and tolerance to changes during testing stage. 

OHT provides a **cross-platform, open design** to support major event-based GUI platforms. Furthermore,
it can be integrated into ongoing and legacy developments due to it being **not code-intrusive**. 
As a result, the framework provides an adaptable, extensible, scalable,
and **robust basis** to support the automation of GUI testing processes. 

OHT is open-source and ready to use. It is cross-platform as well. Versions working in **Qt-Linux** and **Qt-Windows** environments are provided in this repository.

# Requirements / Compile & run OHT

OHT compiles with many different combinations of Qt and Boost libraries. Anyway, the authors suggest using:
- Qt v5.x (we are using Qt 5.5)

        sudo apt-get install qt5-default qt5-qmake

- Boost 1.60 or higher

        sudo apt-get install libboost-system-dev libboost-thread-dev libboost-serialization-dev

To build the project manually from Qt Creator (http://www.qt.io/ide/, recommended), follow these steps:

1. Open the `build_all_*` project.
2. Build `qt_*_hmi_tester` project.
3. Build `qt_*_lib_preload` project.

`*` means the operating system we are building OHT for.

Moreover, an Ansible script (https://www.ansible.com/) is provided to compile and run OHT automatically. It automates:

- installing requirements/dependencies
- downloading this repository
- compiling sources
- run HMI Tester (the testing app of OHT)

To execute it, just copy the file `download_compile_run.ansible.yml` to an empty directory and execute the following command:

    ansible-playbook -i "localhost," -c local ansible_ubuntu.yml -K

# OHT in action

## Recording and playing desktop test cases

With OHT you can create test cases for your application. Then, test cases can be replayed one by one, or all in a row.

<a href="https://www.youtube.com/watch?v=PiwPB8uwZOk"><img src="http://img.youtube.com/vi/PiwPB8uwZOk/0.jpg" width="250"></a>

https://www.youtube.com/watch?v=PiwPB8uwZOk

## Recording and playing web test cases

With OHT you can create test cases for your webapp. Then, test cases can be replayed one by one, or all in a row.

OHT provides a free, easy and fast way to test your web, as well as to automate actions.

Note that this might produce unexpected results, as in the current implementation of the OHT, the test is recorded in the browser, not in the web page. We are working hard to provide a robust web-testing experience.

<a href="https://www.youtube.com/watch?v=Smcj3WmdPdQ"><img src="http://img.youtube.com/vi/Smcj3WmdPdQ/0.jpg" width="250"></a>

https://www.youtube.com/watch?v=Smcj3WmdPdQ

## Cross-platform experiment

With OHT you can create test cases in Windows and play them into the same application compiled in Linux, and vice versa. Cross-platform testing is one of the strengths of OHT.

<a href="https://www.youtube.com/watch?v=3WYmRFk7r7E"><img src="http://img.youtube.com/vi/3WYmRFk7r7E/0.jpg" width="250"></a>

https://www.youtube.com/watch?v=3WYmRFk7r7E

## Robustness experiment

OHT can foresee missing GUI widgets, thus no actions will be executed for them.

<a href="https://www.youtube.com/watch?v=4rXnRWQ9dts"><img src="http://img.youtube.com/vi/4rXnRWQ9dts/0.jpg" width="250"></a>

https://www.youtube.com/watch?v=4rXnRWQ9dts

# Logs in the AUT for debug actions

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

### If I try to open and build the build_oht_qt_win.pro I get these messages:

    cannot find -lboost_serialization-mgw49-mt-d-1_60
    cannot find -lboost_thread-mgw49-mt-d-1_60
    cannot find -lboost_system-mgw49-mt-d-1_60
    
You need to correctly reference Boost libraries:

1. in `common/common.pri`, add correct Boost includes at the end of this file.
2. change the name of all includes from `-lboost_system-mgw49-mt-d-1_60` to `-lboost_system-<your-compiler-version>-1_60`. Remember that Boost libraries have to be compiled with the same compiler used by Qt (`mingw` in this case).

### I cannot compile `build_all_*.pro` project

In this case, try to compile projects independently:

1. Compile `qt_*_hmi_tester` project.
2. Compile `qt_*_lib_preload` project.

`*` means the operating system we are building OHT for.


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

# TODO: Windows adaptation

*TODO*: Do the preload automatically in Windows. While in linux it is implemented using the LD_PRELOAD
environment variable, in windows I found several options:

  - Option 1: Use registry keys. The problem is that security in most recent Windows versions do not allow us to do it. You can read this: https://en.wikipedia.org/wiki/DLL_injection

  - Option 2: Create an application launcher (AppL) to launch the application under testing (AUT). The process would be as follows:
     1. The HMI Tester starts recording a test case, thus the AUT must be launched and the PreloadLibrary loaded into it.
     2. The HMI Tester calls the AppL with the path to the AUT as parameter.
     3. The AppL is a binary including the PreloadLibrary. Therefore, in its main function, it first calls to `QtPreloadingControl::Do_preload();`. Please, see `win_oht_launcher` subproject into the `build_all_win` project.
     4. The AppL loads the exe file of the AUT as a DLL. Some links supporting this approach:
        - http://www.codeproject.com/Articles/1045674/Load-EXE-as-DLL-Mission-Possible
        - https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175%28v=vs.85%29.aspx
        - http://stackoverflow.com/questions/8696653/dynamically-load-a-function-from-a-dll
     5. The AppL calls the main function of the AUT to launch it, including the PreloadLibrary previously loaded.

  For this, `openhmitester/src/preloaders/winpreloadingaction.cpp` should be properly implemented to (option 1) change the registry and launch the AUT as a `QProcess` or (option 2) to call the AppL with the AUT path as parameter. Right now it holds the basic implementation with a lot of TODOs.

Additional info: **PreloadModule** is the library used to extract events from the application under test (AUT), and to execute actions on it as well. This module (encapsulated into a DLL library) has to:

  1. be preloaded into the AUT before the AUT is launched.
  2. detect any event (in linux we use an event called something like Qt nativeEvent) to automatically execute at startup and deploy the OHT services, thus the OHT controller will be able to communicate to and control the AUT.


# Any question? Any bug?

Please, contact me at pedrolmn@gmail.com

# Further information

Webpage: http://catedrasaes.org/html/projects/OHT/OHT.html

More videos: http://www.youtube.com/user/CatedraSaesUmu




