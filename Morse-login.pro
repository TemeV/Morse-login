# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = Morse-login

CONFIG += sailfishapp

SOURCES += \
    src/registrator.cpp \
    src/main.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    rpm/Morse-login.spec \
    translations/*.ts \
    Morse-login.desktop \
    qml/MorseLogin.qml

LIBS += -ltelegram-qt5
INCLUDEPATH += /usr/include/telegram-qt5

HEADERS += \
    src/registrator.h

DISTFILES += \
    qml/pages/LogInPage.qml \
    Morse-login.png

