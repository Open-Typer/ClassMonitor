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
	QDirIterator it(fileUtils::configLocation(),QDirIterator::NoIteratorFlags);
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
	QSettings classIni(fileUtils::configLocation() + "/" +
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

/*!
 * Returns the path to the program configuration directory.\n
 * For example: <tt>/home/user/.config/Open-Typer-CM</tt>
 */
QString fileUtils::configLocation(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer-CM";
}
