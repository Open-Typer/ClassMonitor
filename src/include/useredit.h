/*
 * useredit.h
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

#ifndef USEREDIT_H
#define USEREDIT_H

#include <QDialog>
#include "core/fsmanager.h"

namespace Ui {
	class userEdit;
}

/*! \brief the userEdit class is a dialog used to add or edit users. */
class userEdit : public QDialog
{
	Q_OBJECT
	public:
		explicit userEdit(bool newUser = false, int id = 1, QWidget *parent = nullptr);
		~userEdit();

	private:
		Ui::userEdit *ui;
		bool creatingNewUser;
		int userID;

	private slots:
		void verify(void);
		void resetPassword(void);
		void finish(void);
};

#endif // USEREDIT_H
