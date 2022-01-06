/*
 * main.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QSslSocket>
#include <QFile>
#include "monitorwindow.h"
#include "initialsetup.h"
#include "core/server.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// Set window icon
	a.setWindowIcon(QIcon(":/res/images/icon.ico"));
	// Start server
	serverPtr = new monitorServer(true);
	if(!serverPtr->isListening())
	{
		// Check if there's an existing server instance
		QSslSocket *socket = new QSslSocket;
		QFile *certFile = new QFile(":certs/server.pem");
		certFile->open(QIODevice::ReadOnly | QIODevice::Text);
		QSslCertificate cert(certFile);
		socket->addCaCertificate(cert);
		socket->setProtocol(QSsl::TlsV1_2);
		socket->ignoreSslErrors({QSslError(QSslError::HostNameMismatch,cert)});
		socket->connectToHostEncrypted("localhost",serverPtr->port());
		if(socket->waitForEncrypted())
			return 0;
		// Run server again, but allow error messages
		serverPtr = new monitorServer(false);
		// Let the user change server settings
		optionsWindow options;
		if(options.exec() == QDialog::Rejected)
			return 0;
	}
	MonitorWindow w;
	// Open initialSetup if no users are found
	if(userManager::userIDs().count() == 0)
	{
		initialSetup setupDialog;
		if(setupDialog.exec() == QDialog::Rejected)
			QMetaObject::invokeMethod(&w,"close",Qt::QueuedConnection);
	}
	w.show();
	return a.exec();
}
