/*
 * server.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include "core/fsmanager.h"

/*! \brief The monitorServer class uses QTcpServer to communicate with Open-Typer clients. */
class monitorServer : public QObject
{
	Q_OBJECT
	public:
		explicit monitorServer(QObject *parent = nullptr);
		~monitorServer();
		static quint16 port(void);
		static QHostAddress address(void);
		bool isListening(void);

	private slots:
		void readRequest(void);
		void sendResponse(void);

	private:
		QTcpServer *server;
		QTcpSocket *clientSocket;
		QByteArray convertData(bool *ok, QList<QByteArray> input);
		QByteArray convertData(QList<QByteArray> input);
		QList<QByteArray> readData(QByteArray input);
		QMap<QString,QString> sessions;
};

#endif // SERVER_H
