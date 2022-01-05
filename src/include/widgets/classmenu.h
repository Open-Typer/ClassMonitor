/*
 * classmenu.h
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

#ifndef CLASSMENU_H
#define CLASSMENU_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include "monitorwindow.h"
#include "core/fsmanager.h"
#include "classedit.h"
#include "adminselector.h"

namespace Ui {
	class classMenu;
}

/*!
 * \brief The classMenu class is a class menu widget used by MonitorWindow.
 *
 * \image html classMenu.png
 */
class classMenu : public QWidget
{
	Q_OBJECT
	public:
		explicit classMenu(QWidget *parent = nullptr);
		~classMenu();
		int classID;

	private:
		Ui::classMenu *ui;
		void setupList(void);

	signals:
		/*! A signal, which is emitted when a class is opened. The parameter is class ID. */
		void classOpened(int);
		/*! A signal, which is emitted when user manager button is clicked. */
		void userManagerOpened();

	private slots:
		void verify(void);
		void open(void);
		void openUserManager(void);
		void addClass(void);
		void removeClass(void);
		void editClass(void);
};

#endif // CLASSMENU_H
