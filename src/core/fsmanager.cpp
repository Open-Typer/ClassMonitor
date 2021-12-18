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
