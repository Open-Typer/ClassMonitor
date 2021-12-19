/*
 * monitorwindow.h
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

#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include "classmenu.h"
#include "initialsetup.h"
#include "usermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MonitorWindow; }
QT_END_NAMESPACE

/*! \brief The MonitorWindow class is the main window. */
class MonitorWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MonitorWindow(QWidget *parent = nullptr);
		~MonitorWindow();

	private:
		Ui::MonitorWindow *ui;
		int classID;

	private slots:
		void openClass(void);
		void openUserManager(void);
		void closeClass(void);
		void updateSchoolName(void);
};
#endif // MONITORWINDOW_H
