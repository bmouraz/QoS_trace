# QoS Trace

A software that analyzes data from NS-3 output (.tr - tracer file).

## Starting

### Precondition

The next repositories are needed:

```
sudo apt install build-essential
sudo apt install qtbase5-dev
sudo apt install qtdeclarative5-dev
sudo apt install qtmultimedia5-dev
sudo apt install libqt5charts5-dev
sudo apt install qml-module-qtcharts
sudo apt install qtpositioning5-dev
sudo apt install qtbase5-private-dev
```

### Installing

On the folder that .pro files were extracted, execute:

```
qmake QoS_trace
make
./QoS_trace
``` 

## How to Use:

Input a valid .tr file with trace output data.

## Build with:

* [QMake 3.0](https://doc.qt.io/archives/3.3/qmake-manual-2.html) - Compiler
* [Qt5](http://doc.qt.io/qt-5/linux.html) - Framework for GUI
* [QCustomPlot](http://www.qcustomplot.com/) - Graphics
* [QXlsx] (https://github.com/QtExcel/QXlsx) - XLSX export

### Autor

* **Bruno Moura dos Santos**
