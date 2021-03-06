/*
 * server.cpp
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

#include "core/server.h"

monitorServer *serverPtr = nullptr;

/*! Constructs monitorServer. */
monitorServer::monitorServer(bool silent, QObject *parent) :
	QTcpServer(parent),
	clientSockets(),
	sessions(),
	m_sslLocalCertificate(),
	m_sslPrivateKey(),
	m_sslProtocol(QSsl::UnknownProtocol)
{
	setSslLocalCertificate(":certs/server.pem");
	setSslPrivateKey(":certs/server.key");
	setSslProtocol(QSsl::TlsV1_2);
	if(!listen(QHostAddress::Any,port()))
	{
		if(!silent)
		{
			QMessageBox errBox;
			errBox.setWindowTitle("Error");
			errBox.setText(tr("Unable to start server on port %1.").arg(port()));
			errBox.setInformativeText(errorString());
			errBox.setIcon(QMessageBox::Critical);
			errBox.exec();
		}
		return;
	}
}

/*! Destroys the monitorServer object. */
monitorServer::~monitorServer()
{
	close();
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
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	return settings.value("server/port",57100).toUInt();
}

/*!
 * Connected from QSslSocket::encrypted().\n
 * Accepts a new connection.
 *
 * \see sendResponse()
 */
void monitorServer::acceptConnection(void)
{
	QSslSocket *clientSocket = dynamic_cast<QSslSocket*>(nextPendingConnection());
	connect(clientSocket,&QIODevice::readyRead,this,&monitorServer::sendResponse);
	connect(clientSocket,&QAbstractSocket::disconnected,this,&monitorServer::disconnectClient);
	clientSockets += clientSocket;
}

/*! Connected from QAbstractSocket::disconnected(). */
void monitorServer::disconnectClient(void)
{
	QSslSocket *clientSocket = (QSslSocket*) sender();
	clientSockets.removeAll(clientSocket);
	clientSocket->deleteLater();
}

/*!
 * Connected from QIODevice::readyRead().\n
 * Sends a response back to the client.
 */
void monitorServer::sendResponse(void)
{
	QSslSocket *clientSocket = (QSslSocket*) sender();
	QByteArray request = clientSocket->readAll();
	QList<QByteArray> requestList = readData(request);
	if((requestList[0] == "auth") && (requestList.count() >= 3))
	{
		if(classManager::studentAuth(requestList[1],requestList[2]))
		{
			sessions.insert(clientSocket,requestList[1]);
			clientSocket->write(convertData({"ok"}));
		}
		else
			clientSocket->write(convertData({"fail"}));
	}
	else if(requestList[0] == "logout")
	{
		sessions.remove(clientSocket);
		clientSocket->write(convertData({"ok"}));
	}
	else if(requestList[0] == "check")
		clientSocket->write(convertData({"ok"}));
	else if((requestList[0] == "get") && (requestList.count() >= 2))
	{
		if(requestList[1] == "username")
		{
			if(sessions.contains(clientSocket))
			{
				QString username = sessions.value(clientSocket);
				clientSocket->write(convertData({"ok",username.toUtf8()}));
			}
			else
				clientSocket->write(convertData({"fail"}));
		}
		else if((requestList[1] == "resultcount") && (requestList.count() >= 6))
		{
			if(sessions.contains(clientSocket))
			{
				QString username = sessions.value(clientSocket);
				QPair<int,int> studentLoc = classManager::findStudent(username);
				clientSocket->write(convertData({"ok",QByteArray::number(classManager::historySize(studentLoc.first,studentLoc.second,QString(requestList[2]),requestList[3].toInt(),requestList[4].toInt(),requestList[5].toInt()))}));
			}
			else
				clientSocket->write(convertData({"fail"}));
		}
		else if((requestList[1] == "result") && (requestList.count() >= 7))
		{
			if(sessions.contains(clientSocket))
			{
				QString username = sessions.value(clientSocket);
				QPair<int,int> studentLoc = classManager::findStudent(username);
				QStringList entry = classManager::historyEntry(studentLoc.first,studentLoc.second,QString(requestList[2]),requestList[3].toInt(),requestList[4].toInt(),requestList[5].toInt(),requestList[6].toInt());
				QList<QByteArray> responseList;
				responseList.clear();
				responseList += "ok";
				for(int i=0; i < entry.count(); i++)
					responseList += entry[i].toUtf8();
				clientSocket->write(convertData(responseList));
			}
			else
				clientSocket->write(convertData({"fail"}));
		}
		else if((requestList[1] == "betterstudents") && (requestList.count() >= 6))
		{
			if(sessions.contains(clientSocket))
			{
				QString username = sessions.value(clientSocket);
				QPair<int,int> studentLoc = classManager::findStudent(username);
				int betterStudents = classManager::compareWithStudents(studentLoc.first,studentLoc.second,QString(requestList[2]),requestList[3].toInt(),requestList[4].toInt(),requestList[5].toInt(),true);
				clientSocket->write(convertData({"ok",QByteArray::number(betterStudents)}));
			}
			else
				clientSocket->write(convertData({"fail"}));
		}
		else if((requestList[1] == "worsestudents") && (requestList.count() >= 6))
		{
			if(sessions.contains(clientSocket))
			{
				QString username = sessions.value(clientSocket);
				QPair<int,int> studentLoc = classManager::findStudent(username);
				int worseStudents = classManager::compareWithStudents(studentLoc.first,studentLoc.second,QString(requestList[2]),requestList[3].toInt(),requestList[4].toInt(),requestList[5].toInt(),false);
				clientSocket->write(convertData({"ok",QByteArray::number(worseStudents)}));
			}
			else
				clientSocket->write(convertData({"fail"}));
		}
		else
			clientSocket->write(convertData({"fail"}));
	}
	else if((requestList[0] == "put") && (requestList.count() >= 2))
	{
		if((requestList[1] == "result") && (requestList.count() >= 9))
		{
			if(sessions.contains(clientSocket))
			{
				QString username = sessions.value(clientSocket);
				QPair<int,int> studentLoc = classManager::findStudent(username);
				QList<QVariant> resultData = {requestList[6],requestList[7],requestList[8]};
				if(classManager::addHistoryEntry(studentLoc.first,studentLoc.second,QString(requestList[2]),requestList[3].toInt(),requestList[4].toInt(),requestList[5].toInt(),resultData))
					clientSocket->write(convertData({"ok"}));
				else
					clientSocket->write(convertData({"fail"}));
			}
			else
				clientSocket->write(convertData({"fail"}));
		}
		else
			clientSocket->write(convertData({"fail"}));
	}
	else
		clientSocket->write(convertData({"fail"}));
}

/*! Sends a signal to clients with username from a list. */
void monitorServer::sendSignal(QByteArray name, QList<QByteArray> data, QList<QByteArray> usernames)
{
	QList<QByteArray> rawData;
	rawData.clear();
	rawData += name;
	for(int i=0; i < data.count(); i++)
		rawData += data[i];
	QByteArray finalData = convertData(rawData);
	for(int i=0; i < clientSockets.count(); i++)
	{
		if(sessions.contains(clientSockets[i]))
		{
			if(usernames.contains(sessions[clientSockets[i]].toUtf8()))
				clientSockets[i]->write(finalData);
		}
	}
}

/*! Converts list of QByteArrays to a single QByteArray, which can be used for a response or signal. */
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

/*! Overrides QTcpServer#incomingConnection(). */
void monitorServer::incomingConnection(qintptr socketDescriptor)
{
	QSslSocket *sslSocket = new QSslSocket(this);
	sslSocket->setSocketDescriptor(socketDescriptor);
	sslSocket->setLocalCertificate(m_sslLocalCertificate);
	sslSocket->setPrivateKey(m_sslPrivateKey);
	sslSocket->setProtocol(m_sslProtocol);
	addPendingConnection(sslSocket);
	sslSocket->startServerEncryption();
	connect(sslSocket,&QSslSocket::encrypted,this,&monitorServer::acceptConnection);
}

/*! Returns local SSL certificate. */
const QSslCertificate &monitorServer::getSslLocalCertificate() const
{
	return m_sslLocalCertificate;
}

/*! Returns SSL private key. */
const QSslKey &monitorServer::getSslPrivateKey() const
{
	return m_sslPrivateKey;
}

/*! Returns the SSL protocol that is currently set. */
QSsl::SslProtocol monitorServer::getSslProtocol() const
{
	return m_sslProtocol;
}

/*! Sets local SSL certificate. */
void monitorServer::setSslLocalCertificate(const QSslCertificate &certificate)
{
	m_sslLocalCertificate = certificate;
}

/*! Sets local SSL certificate from path. */
bool monitorServer::setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format)
{
	QFile certificateFile(path);
	if(!certificateFile.open(QIODevice::ReadOnly))
		return false;
	m_sslLocalCertificate = QSslCertificate(certificateFile.readAll(),format);
	return true;
}

/*! Sets SSL private key. */
void monitorServer::setSslPrivateKey(const QSslKey &key)
{
	m_sslPrivateKey = key;
}

/*! Sets SSL private key from a file. */
bool monitorServer::setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format, const QByteArray &passPhrase)
{
	QFile keyFile(fileName);
	if(!keyFile.open(QIODevice::ReadOnly))
		return false;
	m_sslPrivateKey = QSslKey(keyFile.readAll(),algorithm,format,QSsl::PrivateKey,passPhrase);
	return true;
}

/*! Sets SSL protocol. */
void monitorServer::setSslProtocol(QSsl::SslProtocol protocol)
{
	m_sslProtocol = protocol;
}
