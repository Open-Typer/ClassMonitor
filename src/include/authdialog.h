/*
 * authdialog.h
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

#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>

namespace Ui {
	class authDialog;
}

/*! \brief The authDialog class is an authentication dialog with a password input box. */
class authDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit authDialog(QString userName = "", QWidget *parent = nullptr);
		~authDialog();
		QString password;

	private:
		Ui::authDialog *ui;

	private slots:
		void updateText(QString text);
};

#endif // AUTHDIALOG_H
