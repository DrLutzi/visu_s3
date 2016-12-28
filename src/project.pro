#--------------------------
#
# Project TP OpenGL DTMI 1
#
#--------------------------

CONFIG += c++11

QT += core gui opengl xml widgets
TARGET = tp_ogl_basic
TEMPLATE = app
TEMPLATE += subdirs

#for QGLViewer & glm
INCLUDEPATH += ../

DESTDIR = ../bin/

QMAKE_CXXFLAGS += -DSHADERPATH=$$_PRO_FILE_PWD_

# Linux
unix:!macx {
	QMAKE_LFLAGS += -Wl,-rpath,$$_PRO_FILE_PWD_/../QGLViewer
	LIBS += -L$$_PRO_FILE_PWD_/../QGLViewer -lQGLViewer -lGLEW -lGLU
}

# macOS/X
macx {
	# installer homebrew puis glew (brew install glew)
	# installer Qt5.x
	QMAKE_LFLAGS += -Wl,-rpath,$$_PRO_FILE_PWD_/../QGLViewer
	LIBS += -L$$_PRO_FILE_PWD_/../QGLViewer -lQGLViewer -L/usr/local/lib -lGLEW
	INCLUDEPATH += /usr/local/include
}

# Windows (64b)
win32 {
	# installer VS2015 community
	# installer Qt5.x pour VS2015_x64
	# unzip glew.zip (recherche google)
	# copier lib/Release/x64/glew32.lib dans TP_OGL
	# copier bin/Release/x64/glew32.dll dans TP_OGL/bin
	# copier GL (de include) dans TP_OGL
	LIBS += -L$$_PRO_FILE_PWD_/../QGLViewer -L$$_PRO_FILE_PWD_/../ -lQGLViewer -lglew32 -lopengl32
}



SOURCES += main.cpp \
        viewer.cpp \
        Shader.cpp \
        ShaderProgram.cpp \
    ShaderProgram_RayTracer.cpp \
    sceneobject.cpp \
    sceneface.cpp \
    ray.cpp \
    scenemanager.cpp

HEADERS  += viewer.h \
        ShaderProgram.h \
        Shader.h \
        errorsHandler.hpp \
    ShaderProgram_RayTracer.h \
    sceneobject.h \
    sceneface.h \
    ray.h \
    scenemanager.h

OTHER_FILES += \
    simpleShader.vert \
    shaderBezier.frag \
    shaderBezier.vert \
    shaderBezier.geom
