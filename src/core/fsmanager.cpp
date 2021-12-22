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

/*! Returns the role of the user. */
QString userManager::userRole(int id)
{
	QSettings userIni(fileUtils::configLocation() + "/users/" +
		QString::number(id) + "/user.ini",
		QSettings::IniFormat);
	return userIni.value("main/role","basic").toString();
}

/*! Returns role ID. */
int userManager::roleID(QString roleName)
{
	if(roleName == "basic")
		return 0;
	else if(roleName == "admin")
		return 1;
	else
		return -1;
}

/*! Returns role name. */
QString userManager::roleName(int id)
{
	QString role = "";
	switch(id) {
		case 0:
			// Basic
			role = "basic";
			break;
		case 1:
			// Administrator
			role = "admin";
			break;
	}
	return role;
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

/*! Edits the user. Returns true if successful. */
bool userManager::editUser(int id, QString name, QString role, QString password)
{
	QSettings userIni(fileUtils::configLocation() + "/users/" +
		QString::number(id) + "/user.ini",
		QSettings::IniFormat);
	userIni.setValue("main/name",name);
	userIni.setValue("main/role",role);
	if(password != "")
	{
		// Save password hash
		QCryptographicHash hash(QCryptographicHash::Sha256);
		hash.addData(password.toUtf8());
		QFile passwdFile(fileUtils::configLocation() + "/users/" + QString::number(id) + "/passwd");
		if(passwdFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
			passwdFile.write(hash.result());
		else
			return false;
	}
	return true;
}

/*! Removes a user and all classes the user owns. Returns true if successful. */
bool userManager::removeUser(int id)
{
	// Remove user's classes
	QList<int> classes = classManager::classIDs();
	for(int i=0; i < classes.count(); i++)
	{
		if(classManager::classOwner(classes[i]) == id)
		{
			if(!classManager::removeClass(classes[i]))
				return false;
		}
	}
	// Remove user
	QDir dir(fileUtils::configLocation() + "/users/" + QString::number(id));
	return dir.removeRecursively();
}

/*! Checks user password. */
bool userManager::auth(int userID, QString password)
{
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QFile passwdFile(fileUtils::configLocation() + "/users/" +
		QString::number(userID) +
		"/passwd");
	if(passwdFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
	{
		if(passwdFile.readAll().compare(hash.result()) == 0)
			return true;
		else
		{
			QMessageBox errBox;
			errBox.setWindowTitle(tr("Error"));
			errBox.setText(tr("Incorrect password!"));
			errBox.setStandardButtons(QMessageBox::Ok);
			errBox.setIcon(QMessageBox::Warning);
			errBox.exec();
		}
	}
	return false;
}

/*!
 * Opens authDialog and checks the password.
 * \see authDialog
 */
bool userManager::auth(int userID)
{
	authDialog auth_dialog(userName(userID));
	if(auth_dialog.exec() == QDialog::Accepted)
		return auth(userID,auth_dialog.password);
	else
		return false;
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
	QMultiMap<QDateTime, int> map;
	map.clear();
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir())
		{
			bool ok;
			int id = fileInfo.fileName().toInt(&ok);
			if(ok)
				map.insert(classTimestamp(id),id);
		}
	}
	QList<int> inv = map.values();
	for(int i = inv.count()-1; i >= 0; i--)
		out += inv[i];
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

/*! Returns the class owner ID. */
int classManager::classOwner(int id)
{
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	return classIni.value("main/owner","1").toInt();
}

/*! Returns the class icon ID. */
int classManager::classIcon(int id)
{
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	return classIni.value("main/icon","0").toInt();
}

/*! Returns class timestamp (when the class was used last time). */
QDateTime classManager::classTimestamp(int id)
{
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	return QDateTime::fromString(classIni.value("main/timestamp","0").toString(),Qt::ISODate);
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
bool classManager::addClass(QString name, int owner, bool hasIcon, int icon)
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
		classIni.setValue("main/icon",icon);
	classIni.setValue("main/timestamp",QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
	return true;
}

/*! Edits the class. */
void classManager::editClass(int id, QString name, int owner, bool hasIcon, int icon)
{
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	classIni.setValue("main/name",name);
	classIni.setValue("main/owner",owner);
	if(hasIcon)
		classIni.setValue("main/icon",icon);
	else
		classIni.setValue("main/icon","");
	classIni.setValue("main/timestamp",QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
}

/*! Sets class timestamp (when the class was used last time). */
void classManager::setClassTimestamp(int id, QDateTime time)
{
	QSettings classIni(fileUtils::configLocation() + "/classes/" +
		QString::number(id) + "/class.ini",
		QSettings::IniFormat);
	classIni.setValue("main/timestamp",time.toString(Qt::ISODate));
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
	QStringList items;
	items.clear();
	while(it.hasNext())
		items += it.next();
	items.sort();
	for(int i=0; i < items.count(); i++)
	{
		QFileInfo fileInfo(items[i]);
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

/*! Returns the username of the student. */
QString classManager::studentUsername(int classID, int id)
{
	QSettings studentIni(fileUtils::configLocation() + "/classes/" +
		QString::number(classID) + "/student_" + QString::number(id) + "/student.ini",
		QSettings::IniFormat);
	return studentIni.value("main/username","?").toString();
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

/*! Adds a new student. Returns true if successful. */
bool classManager::addStudent(int classID, QString name, QString username, QString password)
{
	QList<int> IDs = studentIDs(classID);
	int i, max = 0, id;
	for(i=0; i < IDs.count(); i++)
	{
		if(IDs[i] > max)
			max = IDs[i];
	}
	id = max+1;
	QDir dir;
	if(!dir.mkpath(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id)))
		return false;
	QSettings studentIni(fileUtils::configLocation() + "/classes/" +
		QString::number(classID) + "/student_" + QString::number(id) + "/student.ini",
		QSettings::IniFormat);
	studentIni.setValue("main/name",name);
	studentIni.setValue("main/username",username);
	// Save password hash
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QFile passwdFile(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id) + "/passwd");
	if(passwdFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
		passwdFile.write(hash.result());
	else
		return false;
	return true;
}

/*! Edits the student. Returns true if successful. */
bool classManager::editStudent(int classID, int id, QString name, QString username, QString password)
{
	QSettings studentIni(fileUtils::configLocation() + "/classes/" +
		QString::number(classID) + "/student_" + QString::number(id) + "/student.ini",
		QSettings::IniFormat);
	studentIni.setValue("main/name",name);
	studentIni.setValue("main/username",username);
	if(password != "")
	{
		// Save password hash
		QCryptographicHash hash(QCryptographicHash::Sha256);
		hash.addData(password.toUtf8());
		QFile passwdFile(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id) + "/passwd");
		if(passwdFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
			passwdFile.write(hash.result());
		else
			return false;
	}
	return true;
}

/*! Removes a student. Returns true if successful. */
bool classManager::removeStudent(int classID, int id)
{
	QDir dir(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id));
	return dir.removeRecursively();
}

/*! Returns list of lesson packs the student has used. */
QStringList classManager::studentPacks(int classID, int id)
{
	QStringList out;
	out.clear();
	QDirIterator it(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id) + "/stats");
	QString item;
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir() && (fileInfo.fileName() != ".") && (fileInfo.fileName() != ".."))
			out += fileInfo.fileName();
	}
	return out;
}

/*! Returns list of lessons the student has used. */
QList<int> classManager::studentLessons(int classID, int id, QString pack)
{
	QList<int> out;
	out.clear();
	QDirIterator it(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id) + "/stats/" + pack);
	QString item;
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir() && (fileInfo.fileName() != ".") && (fileInfo.fileName() != ".."))
		{
			int lID = exerciseID(fileInfo.fileName(),1);
			if(!out.contains(lID))
				out += lID;
		}
	}
	std::sort(out.begin(),out.end());
	return out;
}

/*! Returns list of sublessons the student has used. */
QList<int> classManager::studentSublessons(int classID, int id, QString pack, int lesson)
{
	QList<int> out;
	out.clear();
	QDirIterator it(fileUtils::configLocation() + "/classes/" + QString::number(classID) + "/student_" + QString::number(id) + "/stats/" + pack);
	QString item;
	while(it.hasNext())
	{
		item = it.next();
		QFileInfo fileInfo(item);
		if(fileInfo.isDir() && (fileInfo.fileName() != ".") && (fileInfo.fileName() != ".."))
		{
			int lID = exerciseID(fileInfo.fileName(),1);
			if(lID == lesson)
			{
				int sID = exerciseID(fileInfo.fileName(),2);
				if(!out.contains(sID))
					out += sID;
			}
		}
	}
	std::sort(out.begin(),out.end());
	return out;
}

/*! Returns a part of the exercise ID (from a directory name).
 *
 * Part 1 - lesson ID\n
 * Part 2 - sublesson ID\n
 * Part 3 - exercise ID\n
 */
int classManager::exerciseID(const QString name, const int part)
{
	QString out = "";
	int i, currentPart = 0;
	for(i=0; i < name.count(); i++)
	{
		if(name[i] == '.')
		{
			currentPart++;
			/*
			 * 1: lesson ID
			 * 2: sublesson ID
			 * 3: exercise ID
			 */
			if(currentPart == part)
				return out.toInt();
			out = "";
			i++;
		}
		out += name[i];
	}
	if(currentPart+1 == part)
		return out.toInt();
	else
		return 0;
}

/*! Converts integer list of lessons to a human-readable string list. */
QStringList classManager::lessonList(QList<int> in)
{
	QStringList out;
	out.clear();
	for(int i=0; i < in.count(); i++)
		out += tr("Lesson") + " " + QString::number(in[i]);
	return out;
}

/*! Converts integer list of sublessons to a human-readable string list. */
QStringList classManager::sublessonList(QList<int> in)
{
	QStringList out;
	out.clear();
	for(int i=0; i < in.count(); i++)
	{
		switch(in[i]) {
			case 1:
				out += tr("Touch");
				break;
			case 2:
				out += tr("Words");
				break;
			case 3:
				out += tr("Sentences");
				break;
			case 4:
				out += tr("Text");
				break;
			default:
				out += tr("Sublesson") + " " + QString::number(in[i]);
				break;
		}
	}
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
