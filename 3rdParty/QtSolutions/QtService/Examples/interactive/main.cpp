/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info.com)
**
** This file is part of a Qt Solutions component.
**
** Commercial Usage
** Licensees holding valid Qt Solutions licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales.com.
**
****************************************************************************/

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLabel>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include "qtservice.h"

class InteractiveService : public QtService<QApplication>
{
public:
    InteractiveService(int argc, char **argv);
    ~InteractiveService();

protected:

    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);

private:
    QLabel *gui;
};

InteractiveService::InteractiveService(int argc, char **argv)
    : QtService<QApplication>(argc, argv, "Qt Interactive Service"), gui(0)
{
    setServiceDescription("A Qt service with user interface.");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

InteractiveService::~InteractiveService()
{
}

void InteractiveService::start()
{
#if defined(Q_OS_WIN)
    if ((QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) &&
        (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)) {
        logMessage( "Service GUI not allowed on Windows Vista. See the documentation for this example for more information.", QtServiceBase::Error );
        return;
    }
#endif

    qApp->setQuitOnLastWindowClosed(false);

    gui = new QLabel("Service", 0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    gui->move(QApplication::desktop()->availableGeometry().topLeft());
    gui->show();
}

void InteractiveService::stop()
{
    delete gui;
}

void InteractiveService::pause()
{
    if (gui)
	gui->hide();
}

void InteractiveService::resume()
{
    if (gui)
	gui->show();
}

void InteractiveService::processCommand(int code)
{
    gui->setText("Command code " + QString::number(code));
    gui->adjustSize();
}

int main(int argc, char **argv)
{
#if !defined(Q_WS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/Trolltech.conf)", QDir::tempPath().toLatin1().constData());
#endif
    InteractiveService service(argc, argv);
    return service.exec();
}
