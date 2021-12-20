/*
 * main.cpp
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

#include <QApplication>
#include "monitorwindow.h"
#include "initialsetup.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	classMenu w;
	// Open initialSetup if no users are found
	if(userManager::userIDs().count() == 0)
	{
		initialSetup setupDialog;
		if(setupDialog.exec() == QDialog::Rejected)
			QMetaObject::invokeMethod(&w,"close",Qt::QueuedConnection);
	}
	w.show();
	return a.exec();
}
