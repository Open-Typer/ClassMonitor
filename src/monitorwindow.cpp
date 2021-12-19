/*
 * monitorwindow.cpp
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

#include "monitorwindow.h"
#include "ui_monitorwindow.h"

/*! Constructs MonitorWindow. */
MonitorWindow::MonitorWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MonitorWindow)
{
	ui->setupUi(this);
	ui->classControls->hide();
	// Open initialSetup if no users are found
	if(userManager::userIDs().count() == 0)
	{
		initialSetup setupDialog;
		if(setupDialog.exec() == QDialog::Rejected)
		{
			QMetaObject::invokeMethod(this,"close",Qt::QueuedConnection);
			return;
		}
	}
	// Connections
	connect(ui->openClassButton,SIGNAL(clicked()),this,SLOT(openClass()));
	connect(ui->closeClassButton,SIGNAL(clicked()),this,SLOT(closeClass()));
	emit openClass();
}

/*! Destroys the MonitorWindow object. */
MonitorWindow::~MonitorWindow()
{
	delete ui;
}

/*!
 * Connected from openClassButton.\n
 * Opens class menu dialog.
 *
 * \see classMenu
 */
void MonitorWindow::openClass(void)
{
	classMenu menu;
	menu.exec();
}

void MonitorWindow::closeClass(void)
{
	ui->classControls->hide();
}
