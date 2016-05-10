INCLUDEPATH += $$PWD
LIBS += -lQtOpenCL
macx:!opencl_configure {
    LIBS += -framework OpenCL
}
win32|unix {
    !isEmpty(QMAKE_INCDIR_OPENCL) {
		INCLUDEPATH += $$QMAKE_INCDIR_OPENCL
    }
    !isEmpty(QMAKE_LIBDIR_OPENCL) {
        LIBS += -L$$QMAKE_LIBDIR_OPENCL
    }
    !isEmpty(QMAKE_LIBS_OPENCL) {
        LIBS += $$QMAKE_LIBS_OPENCL
    } else {
        LIBS += -lOpenCL
    }
}
unix:{
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/../lib\'"
}
