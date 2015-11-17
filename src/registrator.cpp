#include "registrator.h"

#include <TelegramQt/CAppInformation>
#include <TelegramQt/CTelegramCore>

#include <QDebug>


#define INSECURE_SAVE


#ifdef INSECURE_SAVE
#include <QStandardPaths>
#include <QDir>
#include <QFile>
static const QString secretsDirPath = QLatin1String("/home/nemo/.cache/telepathy-morse/secrets/");
#endif // INSECURE_SAVE


Registrator::Registrator(QObject *parent) :
    QObject(parent),
    m_core(0),
    m_authReconnectionsCount(0)
{
    qDebug() << Q_FUNC_INFO;
}

void Registrator::start(QString phoneNumber)
{
    qDebug() << Q_FUNC_INFO;

    m_selfPhone = phoneNumber;

    CAppInformation appInfo;
    appInfo.setAppId(14617);
    appInfo.setAppHash(QLatin1String("e17ac360fd072f83d5d08db45ce9a121"));
    appInfo.setAppVersion(QLatin1String("0.1"));
    appInfo.setDeviceInfo(QLatin1String("pc"));
    appInfo.setOsInfo(QLatin1String("GNU/Linux"));
    appInfo.setLanguageCode(QLatin1String("en"));

    m_authReconnectionsCount = 0;
    m_core = new CTelegramCore(0);
    m_core->setAppInformation(&appInfo);
    m_core->setMessageReceivingFilterFlags(TelegramNamespace::MessageFlagOut|TelegramNamespace::MessageFlagRead);
    m_core->setAcceptableMessageTypes(TelegramNamespace::MessageTypeText);

    connect(m_core, SIGNAL(connectionStateChanged(TelegramNamespace::ConnectionState)),
            this, SLOT(whenConnectionStateChanged(TelegramNamespace::ConnectionState)));
    connect(m_core, SIGNAL(authorizationErrorReceived()),
            this, SLOT(whenAuthErrorReceived()));
    connect(m_core, SIGNAL(phoneCodeRequired()),
            this, SLOT(whenPhoneCodeRequired()));
    connect(m_core, SIGNAL(authSignErrorReceived(TelegramNamespace::AuthSignError,QString)),
            this, SLOT(whenAuthSignErrorReceived(TelegramNamespace::AuthSignError,QString)));

    m_core->initConnection(QLatin1String("149.154.175.50"), 443);
}

void Registrator::logIn(QString code)
{
    m_core->signIn(m_selfPhone, code);
}

void Registrator::whenConnectionStateChanged(TelegramNamespace::ConnectionState state)
{
    qDebug() << Q_FUNC_INFO << state;
    switch (state) {
    case TelegramNamespace::ConnectionStateAuthRequired:
        m_core->requestPhoneCode(m_selfPhone);
        break;
    case TelegramNamespace::ConnectionStateAuthenticated:
        whenAuthenticated();
        break;
    case TelegramNamespace::ConnectionStateReady:
        whenConnectionReady();
        break;
    case TelegramNamespace::ConnectionStateDisconnected:
//            saveSessionData(m_selfPhone, m_core->connectionSecretInfo());
        break;
    default:
        break;
    }
}

void Registrator::whenAuthenticated()
{
    qDebug() << Q_FUNC_INFO;
}

void Registrator::whenAuthErrorReceived()
{
    qDebug() << Q_FUNC_INFO;

    emit failed();
}

void Registrator::whenPhoneCodeRequired()
{
    qDebug() << Q_FUNC_INFO;

    emit codeNeeded();
}

void Registrator::whenAuthSignErrorReceived(TelegramNamespace::AuthSignError errorCode, const QString &errorMessage)
{
    qDebug() << Q_FUNC_INFO << errorCode << errorMessage;

    emit failed();
}

void Registrator::whenConnectionReady()
{
    qDebug() << Q_FUNC_INFO;

    saveSessionData(m_selfPhone, m_core->connectionSecretInfo());

    emit loggedIn();
    m_core->closeConnection();
}

bool Registrator::saveSessionData(const QString &phone, const QByteArray &data)
{
#ifdef INSECURE_SAVE
    QDir dir;

    dir.mkpath(secretsDirPath);
    QFile secretFile(secretsDirPath + phone);

    if (secretFile.open(QIODevice::WriteOnly)) {
        return secretFile.write(data) == data.size();
    }
#endif // INSECURE_SAVE

    return false;
}
