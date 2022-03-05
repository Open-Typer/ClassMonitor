/*
 * usermanager.h
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

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include <QCloseEvent>
#include "useredit.h"
#include "core/fsmanager.h"

namespace Ui {
	class userManagerWidget;
}

/*!
 * \brief the userManagerWidget class is a widget for user management, used by MonitorWindow.
 *
 * \image html userManagerWidget.png
 */
class userManagerWidget : public QWidget
{
	Q_OBJECT
	public:
		explicit userManagerWidget(int userLoginID, QWidget *parent = nullptr);
		~userManagerWidget();

	private:
		Ui::userManagerWidget *ui;
		void setupList(void);
		int loginID;

	signals:
		/*! A signal, which is emitted when the back button is clicked. */
		void backClicked();

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void verify(void);
		void finish(void);
		void addUser(void);
		void removeUser(void);
		void editUser(void);
};

#endif // USERMANAGER_H
