/*
 * fsmanager.h
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

#ifndef FSMANAGER_H
#define FSMANAGER_H

#include <QObject>
#include <QList>
#include <QStandardPaths>
#include <QDirIterator>
#include <QSettings>

/*! \brief The classManager class provides functions for class data reading and writing. */
class classManager : public QObject
{
	Q_OBJECT
	public:
		static QList<int> classIDs(void);
		static QString className(int id);
		static QStringList classNames(void);
		static QList<int> studentIDs(int classID);
		static QString studentName(int classID, int id);
		static QStringList studentNames(int classID);
		static void addClass(QString name, bool hasIcon = false, QString iconName = "");
		static bool removeClass(int id);
};

/*! \brief The fileUtils class contains functions for file or directory paths. */
class fileUtils : public QObject
{
	Q_OBJECT
	public:
		static QString configLocation(void);
};

#endif // FSMANAGER_H
