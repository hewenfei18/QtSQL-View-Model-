QT += core gui sql widgets
CONFIG += c++17

TARGET = Lab3_SQL
TEMPLATE = app

# 头文件
HEADERS += \
    idatabase.h \
    loginview.h \
    welcomeview.h \
    masterview.h \
    patientview.h \
    patienteditview.h \
    doctorview.h \
    departmentview.h

# 源文件
SOURCES += \
    main.cpp \
    idatabase.cpp \
    loginview.cpp \
    welcomeview.cpp \
    masterview.cpp \
    patientview.cpp \
    patienteditview.cpp \
    doctorview.cpp \
    departmentview.cpp

# UI文件
FORMS += \
    loginview.ui \
    welcomeview.ui \
    masterview.ui \
    patientview.ui \
    patienteditview.ui \
    doctorview.ui \
    departmentview.ui

# 输出路径配置（无需修改）
DESTDIR = ./build
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
UI_DIR = $$DESTDIR/ui
RESOURCES += \
    resources.qrc

# 自动连接 UI
QT += xml
QT += sql
# 不让 qmake 清理数据库
QMAKE_CLEAN += $$quote(-exclude Qt_Lab3a.db)
