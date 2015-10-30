OHT: Open HMI Tester
====================

"Open HMI Tester" or OHT is an application framework for the development of GUI testing tools. It uses
GUI introspection to capture and simulate real user interaction, which enhances
robustness and tolerance to changes during testing stage. 

OHT provides a cross-platform, open design to support major event-based GUI platforms. Furthermore,
it can be integrated into ongoing and legacy developments due to it being not code-intrusive. 
As a result, the framework provides an adaptable, extensible, scalable,
and robust basis to support the automation of GUI testing processes. 

OHT is open-source and ready to use.

OHT can be adapted to support different GUI platforms and operating systems. An adaptation to Qt-Linux
testing environments is provided in this repository.

# Any question? Any bug?

Please, contact me at pedrolmn@gmail.com

# OHT in action

## Recording and playing desktop test cases

https://www.youtube.com/watch?v=PiwPB8uwZOk

<iframe width="420" height="315" src="https://www.youtube.com/embed/PiwPB8uwZOk" frameborder="0" allowfullscreen></iframe>

## Recording and playing web test cases

Note that this might produce unexpected results, as in the current implementation of the OHT, the test is recorded in the browser, not in the web page. We are working hard to provide a robust web-testing experience.

https://www.youtube.com/watch?v=Smcj3WmdPdQ

<iframe width="420" height="315" src="https://www.youtube.com/embed/Smcj3WmdPdQ" frameborder="0" allowfullscreen></iframe>




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


## Further information

Webpage: http://www.catedrasaes.org/wiki/OHT

Videos: http://www.youtube.com/user/CatedraSaesUmu


