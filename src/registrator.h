#ifndef REGISTRATOR_H
#define REGISTRATOR_H

#include <QObject>
#include <QtQuick>

#include <TelegramQt/TelegramNamespace>

class CTelegramCore;

class Registrator : public QObject
{
    Q_OBJECT
public:
    explicit Registrator(QObject *parent = 0);

    Q_INVOKABLE void start(QString phoneNumber);
    Q_INVOKABLE void logIn(QString code);

signals:
    void codeNeeded();
    void loggedIn();
    void failed();


public slots:

private slots:
    void whenConnectionStateChanged(TelegramNamespace::ConnectionState state);
    void whenAuthenticated();
    void whenAuthErrorReceived();
    void whenPhoneCodeRequired();
    void whenAuthSignErrorReceived(TelegramNamespace::AuthSignError errorCode, const QString &errorMessage);
    void whenConnectionReady();
//    void whenContactListChanged();
//    void whenDisconnected();

private:

    static bool saveSessionData(const QString &phone, const QByteArray &data);

    CTelegramCore *m_core;

    int m_authReconnectionsCount;

    QString m_selfPhone;

};

#endif // REGISTRATOR_H
