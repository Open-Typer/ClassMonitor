/*
 * addclassdialog.h
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

#ifndef ADDCLASSDIALOG_H
#define ADDCLASSDIALOG_H

#include <QDialog>
#include "core/fsmanager.h"

namespace Ui {
	class classEdit;
}

/*! \brief The classEdit class is a dialog for class adding and editing. */
class classEdit : public QDialog
{
	Q_OBJECT
	public:
		explicit classEdit(bool newClass = false, QWidget *parent = nullptr);
		~classEdit();
		QString className;

	private:
		Ui::classEdit *ui;

	private slots:
		void verify(void);
};

#endif // ADDCLASSDIALOG_H