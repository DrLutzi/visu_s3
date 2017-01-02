#--------------------------
#
# Project TP OpenGL DTMI 1
#
#--------------------------

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

QT += core gui opengl xml widgets
TARGET = project
TEMPLATE = app
TEMPLATE += subdirs

#for QGLViewer & glm
INCLUDEPATH += ../

DESTDIR = ../bin/

QMAKE_CXXFLAGS += -DSHADERPATH=$$_PRO_FILE_PWD_

# Linux
unix:!macx {
        #QMAKE_LFLAGS += -Wl,-rpath,$$_PRO_FILE_PWD_/../QGLViewer
        #LIBS += -L$$_PRO_FILE_PWD_/../QGLViewer -lQGLViewer -lGLEW -lGLU
        LIBS += -lGLEW -lGLU
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
        #LIBS += -L$$_PRO_FILE_PWD_/../QGLViewer -L$$_PRO_FILE_PWD_/../ -lQGLViewer -lglew32 -lopengl32
        LIBS += -L$$_PRO_FILE_PWD_/../Glew/lib/Release/x64 -lglew32 -lopengl32
        INCLUDEPATH += $$_PRO_FILE_PWD_/../Glew/include
}

#SOURCES +=        viewer.cpp
SOURCES += main.cpp \
        Shader.cpp \
        ShaderProgram.cpp \
        ShaderProgram_RayTracer.cpp \
        sceneobject.cpp \
        sceneface.cpp \
        ray.cpp \
        scenemanager.cpp \
        scenecamera.cpp \
        dialog_renderedimage.cpp

#HEADERS  += viewer.h
HEADERS  += ShaderProgram.h \
            Shader.h \
            errorsHandler.hpp \
            ShaderProgram_RayTracer.h \
            sceneobject.h \
            sceneface.h \
            ray.h \
            scenemanager.h \
            scenecamera.h \
            dialog_renderedimage.h

OTHER_FILES += \
    shader.frag \
    shader.vert \
    shader.geom \

FORMS += \
    dialog_renderedimage.ui
