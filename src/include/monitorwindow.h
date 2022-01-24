/*
 * monitorwindow.h
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

#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QPropertyAnimation>
#include "updater/updater.h"
#include "options/optionswindow.h"
#include "widgets/classmenu.h"
#include "widgets/classcontrols.h"
#include "widgets/usermanager.h"
#include "widgets/studentdetails.h"
#include "core/fsmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MonitorWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MonitorWindow class is the main window.
 *
 * \image html MonitorWindow.png
 */
class MonitorWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MonitorWindow(QWidget *parent = nullptr);
		~MonitorWindow();

	private:
		Ui::MonitorWindow *ui;
		int classID;
		QSettings *settings;
		QList<QWidget*> controlWidgets;
		QPropertyAnimation *moveAnim;
		void setTheme(void);
		void updateControlWidget(void);
		void transition(void);

	protected:
		void changeEvent(QEvent *event);

	signals:
		/*! A signal, which is emitted when the theme changes. \see setTheme() */
		void themeChanged(bool);

	private slots:
		void updateSchoolName(void);
		void openOptions(void);
		void openClass(int id);
		void openUserManager(void);
		void openDetails(int id);
		void goBack(void);
};
#endif // MONITORWINDOW_H
