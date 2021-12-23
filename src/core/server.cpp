/*
 * server.cpp
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

#include "core/server.h"

/*! Constructs monitorServer. */
monitorServer::monitorServer(QObject *parent) :
	QObject(parent)
{
	server = new QTcpServer(this);
	if(!server->listen(QHostAddress::Any,port()))
	{
		QMessageBox errBox;
		errBox.setWindowTitle("Error");
		errBox.setText(tr("Unable to start server on port %1.").arg(port()));
		errBox.setInformativeText(server->errorString());
		errBox.setIcon(QMessageBox::Critical);
		errBox.exec();
		return;
	}
	sessions.clear();
	// Connections
	connect(server,&QTcpServer::newConnection,this,&monitorServer::readRequest);
}

/*! Destroys the monitorServer object. */
monitorServer::~monitorServer()
{
	server->close();
}

/*! Returns the IP address of the network interface. */
QHostAddress monitorServer::address(void)
{
	QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
	for(int i=0; i < addressList.count(); i++)
	{
		if((addressList[i] != QHostAddress::LocalHost) && (addressList[i].toIPv4Address()))
			return addressList[i];
	}
	return QHostAddress(QHostAddress::LocalHost);
}

/*! Returns the port, which was set by the user (or the default port). */
quint16 monitorServer::port(void)
{
	QSettings settings(fileUtils::configLocation() + "/settings.ini");
	return (quint16) settings.value("server/port",57100).toInt();
}

/*! Returns true if the server is listening. */
bool monitorServer::isListening(void)
{
	return server->isListening();
}

/*!
 * Connected from server->newConnection().\n
 * Reads the request and sends a response.
 *
 * \see sendResponse()
 */
void monitorServer::readRequest(void)
{
	clientSocket = server->nextPendingConnection();
	connect(clientSocket,&QIODevice::readyRead,this,&monitorServer::sendResponse);
	connect(clientSocket,&QAbstractSocket::disconnected,clientSocket,&QObject::deleteLater);
}

/*! Sends a response back to the client. */
void monitorServer::sendResponse(void)
{
	QByteArray request = clientSocket->readAll();
	QList<QByteArray> requestList = readData(request);
	if(requestList[0] == "auth")
	{
		if(classManager::studentAuth(requestList[1],requestList[2]))
		{
			sessions.insert(QHostAddress(clientSocket->peerAddress().toIPv4Address()).toString(),
				QPair<QString,QDateTime>(requestList[1],QDateTime::currentDateTimeUtc()));
			clientSocket->write(convertData({"ok"}));
		}
		else
			clientSocket->write(convertData({"fail"}));
	}
}

/*! Converts list of QByteArrays to a single QByteArray, which can be used for a response. */
QByteArray monitorServer::convertData(bool *ok, QList<QByteArray> input)
{
	QByteArray out;
	out.clear();
	for(int i = 0; i < input.count(); i++)
	{
		QByteArray sizeNum, dataSize;
		// Data size
		sizeNum.setNum(input[i].size(),16);
		dataSize = QByteArray::fromHex(sizeNum);
		if(sizeNum.size() <= 2)
			dataSize.prepend(QByteArray::fromHex("0"));
		else if(sizeNum.size() > 4)
		{
			if(ok != nullptr)
				*ok = false;
			return QByteArray();
		}
		out += dataSize;
		// Data
		out += input[i];
	}
	if(ok != nullptr)
		*ok = true;
	return out;
}

/*! Implementation of convertData() without status boolean. */
QByteArray monitorServer::convertData(QList<QByteArray> input)
{
	return convertData(nullptr,input);
}

/*! Returns a list of QByteArrays from the input QByteArray. */
QList<QByteArray> monitorServer::readData(QByteArray input)
{
	QList<QByteArray> out;
	out.clear();
	quint16 dataSize, i2;
	QByteArray dataSizeArr, data;
	int i = 0;
	while(i < input.count())
	{
		// Read data size
		dataSizeArr.clear();
		dataSizeArr += input[i];
		dataSizeArr += input[i+1];
		i += 2;
		dataSize = dataSizeArr.toHex().toUInt(nullptr,16);
		// Read data
		data.clear();
		for(i2=0; i2 < dataSize; i2++)
		{
			data += input[i];
			i++;
		}
		out += data;
	}
	return out;
}
