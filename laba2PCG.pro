QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Путь к заголовочным файлам библиотеки Magick++
INCLUDEPATH += $$PWD/include

# Путь к библиотекам Magick++
LIBS += -L$$PWD/lib

# Указываем правильные имена библиотек
LIBS += -lCORE_RL_Magick++_
LIBS += -lCORE_RL_MagickCore_
LIBS += -lCORE_RL_MagickWand_

SOURCES += \
    main.cpp

HEADERS +=

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
