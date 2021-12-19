/*
 * fsmanager.cpp
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

#include "core/fsmanager.h"

/*!
 * Returns list of user IDs (user directory names).
 * \see userNames()
 */
QList<int> userManager::userIDs(void)
{
	QList<int> out;
	out.clear();
	QDirIterator it(fileUtils::configLocation()+"/users",QDirIterator::NoIteratorFlags);
	QString item;
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir())
		{
			bool ok;
			int id = fileInfo.fileName().toInt(&ok);
			if(ok)
				out += id;
		}
	}
	return out;
}

/*! Returns the name of the user. */
QString userManager::userName(int id)
{
	QSettings userIni(fileUtils::configLocation() + "/users/" +
		QString::number(id) + "/user.ini",
		QSettings::IniFormat);
	return userIni.value("main/name","?").toString();
}

/*!
 * Returns list of users (with their real names).
 * \see userIDs()
 * \see userName()
 */
QStringList userManager::userNames(void)
{
	QStringList out;
	out.clear();
	QList<int> idList = userIDs();
	for(int i=0; i < idList.count(); i++)
		out += userName(idList[i]);
	return out;
}

/*! Adds a new user. Returns true if successful. */
bool userManager::addUser(QString name, QString role, QString password)
{
	QList<int> IDs = userIDs();
	int i, max = 0, id;
	for(i=0; i < IDs.count(); i++)
	{
		if(IDs[i] > max)
			max = IDs[i];
	}
	id = max+1;
	QDir dir;
	if(!dir.mkpath(fileUtils::configLocation() + "/users/" + QString::number(id)))
		return false;
	QSettings userIni(fileUtils::configLocation() + "/users/" +
		QString::number(id) + "/user.ini",
		QSettings::IniFormat);
	userIni.setValue("main/name",name);
	userIni.setValue("main/role",role);
	// Save password hash
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QFile passwdFile(fileUtils::configLocation() + "/users/" + QString::number(id) + "/passwd");
	if(passwdFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
		passwdFile.write(hash.result());
	else
		return false;
	return true;
}

/*!
 * Returns list of class IDs (class directory names).
 * \see classNames()
 */
QList<int> classManager::classIDs(void)
{
	QList<int> out;
	out.clear();
	QDirIterator it(fileUtils::configLocation()+"/classes",QDirIterator::NoIteratorFlags);
	QString item;
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir())
		{
			bool ok;
			int id = fileInfo.fileName().toInt(&ok);
			if(ok)
				out += id;
		}
	}
	return out;
}

/*! Returns the name of the class. */
QString classManager::className(int id)
{
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	return classIni.value("main/name","?").toString();
}

/*!
 * Returns list of classes (with their real names).
 * \see classIDs()
 * \see className()
 */
QStringList classManager::classNames(void)
{
	QStringList out;
	out.clear();
	QList<int> idList = classIDs();
	for(int i=0; i < idList.count(); i++)
		out += className(idList[i]);
	return out;
}

/*! Adds a new class. Returns true if successful. */
bool classManager::addClass(QString name, QString owner, bool hasIcon, QString iconName)
{
	QList<int> IDs = classIDs();
	int i, max = 0, id;
	for(i=0; i < IDs.count(); i++)
	{
		if(IDs[i] > max)
			max = IDs[i];
	}
	id = max+1;
	QDir dir;
	if(!dir.mkpath(fileUtils::configLocation() + "/classes/" + QString::number(id)))
		return false;
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	classIni.setValue("main/name",name);
	classIni.setValue("main/owner",owner);
	if(hasIcon)
		classIni.setValue("main/icon",iconName);
	return true;
}

/*! Removes a class. Returns true if successful. */
bool classManager::removeClass(int id)
{
	QDir dir(fileUtils::configLocation() + "/classes/" + QString::number(id));
	return dir.removeRecursively();
}

/*
 * Returns list of student IDs.
 * \see studentNames()
 */
QList<int> classManager::studentIDs(int classID)
{
	QList<int> out;
	out.clear();
	QDirIterator it(fileUtils::configLocation() + "/classes/" +
		QString::number(classID),
		QDirIterator::NoIteratorFlags);
	QString item;
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir())
		{
			QString name = fileInfo.fileName();
			if(name.left(8) == "student_")
			{
				bool ok;
				int id = name.mid(8).toInt(&ok);
				if(ok)
					out += id;
			}
		}
	}
	return out;
}

/*! Returns the name of the student. */
QString classManager::studentName(int classID, int id)
{
	QSettings studentIni(fileUtils::configLocation() + "/classes/" +
		QString::number(classID) + "/student_" + QString::number(id) + "/student.ini",
		QSettings::IniFormat);
	return studentIni.value("main/name","?").toString();
}

/*!
 * Returns list of students (with their real names).
 * \see studentIDs()
 * \see studentName()
 */
QStringList classManager::studentNames(int classID)
{
	QStringList out;
	out.clear();
	QList<int> idList = studentIDs(classID);
	for(int i=0; i < idList.count(); i++)
		out += studentName(classID,idList[i]);
	return out;
}

/*!
 * Returns the path to the program configuration directory.\n
 * For example: <tt>/home/user/.config/Open-Typer-CM</tt>
 */
QString fileUtils::configLocation(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer-CM";
}
