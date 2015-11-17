#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include "registrator.h"

const QString appName = "morse-login";

int main(int argc, char *argv[])
{
    QGuiApplication* app = SailfishApp::application(argc, argv) ;
    app->setOrganizationName(appName);
    app->setApplicationName(appName);

    QQuickView* view = SailfishApp::createView();

    Registrator r;

    view->rootContext()->setContextProperty("Registrator", &r);

    view->setSource(SailfishApp::pathTo("qml/MorseLogin.qml"));

    view->show();
    return app->exec();
}

