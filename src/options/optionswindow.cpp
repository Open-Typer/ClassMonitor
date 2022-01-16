/*
 * optionswindow.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "options/optionswindow.h"
#include "ui_optionswindow.h"

/*! Constructs optionsWindow. */
optionsWindow::optionsWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::optionsWindow)
{
	ui->setupUi(this);
	settings = new QSettings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	ui->portEdit->setValue(monitorServer::port());
	ui->darkThemeCheckBox->setChecked(settings->value("customization/darktheme","false").toBool());
	// Connections
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(closeOptions()));
}

/*! Destroys the optionsWindow object. */
optionsWindow::~optionsWindow()
{
	delete ui;
}

/*!
 * Connected from okButton->clicked().\n
 * Saves settings and closes the dialog.
 */
void optionsWindow::closeOptions(void)
{
	settings->setValue("server/port",ui->portEdit->value());
	settings->setValue("customization/darktheme",ui->darkThemeCheckBox->isChecked());
	delete serverPtr;
	serverPtr = new monitorServer;
	if(serverPtr->isListening())
		accept();
}
