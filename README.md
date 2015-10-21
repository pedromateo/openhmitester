OHT: Open HMI Tester
====================

"Open HMI Tester" is a GUI Testing tool following an open architecture that describes a non intrusive capture/replay tool based on GUI Events. It may be adapted to support different windowing systems and operating systems used in the testing environment.


"Open HMI Tester" or OHT is an application framework for the development of GUI testing tools. It uses
GUI introspection to capture and simulate real user interaction, which enhances
robustness and tolerance to changes during testing stage. 

OHT provides a cross-platform, open design to support major event-based GUI platforms. Furthermore,
it can be integrated into ongoing and legacy developments due to it being not code-intrusive. 
As a result, the framework provides an adaptable, extensible, scalable,
and robust basis to support the automation of GUI testing processes. 

OHT is open-source and ready to use.


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


## Further information at...

http://www.catedrasaes.org/wiki/OHT
