TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    Renderer.cpp \
    Camera.cpp

HEADERS += \
    Voxel.h \
    Vector.h \
    RobotModel.h \
    Renderer.h \
    Ray.h \
    PlatformSDL.h \
    mtlList.h \
    Matrix.h \
    MathTypes.h \
    Math3d.h \
    Camera.h

LIBS += \
	-lSDL \
	-lSDLmain \
	-fopenmp

QMAKE_CXXFLAGS += \
	-fopenmp

QMAKE_LFLAGS += \
	-fopenmp
