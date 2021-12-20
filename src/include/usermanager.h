/*
 * usermanager.h
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

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include "classmenu.h"
#include "useredit.h"
#include "core/fsmanager.h"

namespace Ui {
	class userManagerDialog;
}

/*! \brief the userManagerDialog class is a dialog for user management. */
class userManagerDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit userManagerDialog(int userLoginID, QWidget *parent = nullptr);
		~userManagerDialog();

	private:
		Ui::userManagerDialog *ui;
		void setupList(void);
		int loginID;

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
