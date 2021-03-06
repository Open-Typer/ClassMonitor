/*
 * fsmanager.h
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

#ifndef FSMANAGER_H
#define FSMANAGER_H

#include <QObject>
#include <QList>
#include <QStandardPaths>
#include <QDirIterator>
#include <QSettings>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDateTime>
#include "authdialog.h"

/*! \brief The userManager class provides functions for user management. */
class userManager : public QObject
{
	Q_OBJECT
	public:
		static QList<int> userIDs(void);
		static QString userName(int id);
		static QString userRole(int id);
		static int roleID(QString roleName);
		static QString roleName(int id);
		static QStringList userNames(void);
		static bool addUser(QString name, QString role, QString password);
		static bool editUser(int id, QString name, QString role, QString password = "");
		static bool removeUser(int id);
		static bool auth(int userID, QString password);
		static bool auth(int userID);
};

/*! \brief The classManager class provides functions for class data reading and writing. */
class classManager : public QObject
{
	Q_OBJECT
	public:
		static QList<int> classIDs(void);
		static QString className(int id);
		static int classOwner(int id);
		static int classIcon(int id);
		static QDateTime classTimestamp(int id);
		static QStringList classNames(void);
		static bool addClass(QString name, int owner, bool hasIcon = false, int icon = 0);
		static void editClass(int id, QString name, int owner, bool hasIcon = false, int icon = 0);
		static void setClassTimestamp(int id, QDateTime time);
		static bool removeClass(int id);
		static QList<int> studentIDs(int classID);
		static QString studentName(int classID, int id);
		static QString studentUsername(int classID, int id);
		static QStringList studentNames(int classID);
		static bool addStudent(int classID, QString name, QString username, QString password);
		static bool editStudent(int classID, int id, QString name, QString username, QString password = "");
		static bool removeStudent(int classID, int id);
		static QPair<int,int> findStudent(QString username);
		static bool studentAuth(QString username, QString password);
		static QStringList studentPacks(int classID, int id);
		static QList<int> studentLessons(int classID, int id, QString pack);
		static QList<int> studentSublessons(int classID, int id, QString pack, int lesson);
		static QList<int> studentExercises(int classID, int id, QString pack, int lesson, int sublesson);
		static QStringList lessonList(QList<int> in);
		static QStringList sublessonList(QList<int> in);
		static QStringList exerciseList(QList<int> in);
		static int historySize(int classID, int studentID, QString pack, int lesson, int sublesson, int exercise);
		static QStringList historyEntry(int classID, int studentID, QString pack, int lesson, int sublesson, int exercise, int entry);
		static bool addHistoryEntry(int classID, int studentID, QString pack, int lesson, int sublesson, int exercise, QList<QVariant> entry);
		static int compareWithStudents(int classID, int studentID, QString pack, int lesson, int sublesson, int exercise, bool better);

	private:
		static int exerciseID(const QString name, const int part);
};

/*! \brief The fileUtils class contains functions for file or directory paths. */
class fileUtils : public QObject
{
	Q_OBJECT
	public:
		static QString configLocation(void);
};

#endif // FSMANAGER_H
