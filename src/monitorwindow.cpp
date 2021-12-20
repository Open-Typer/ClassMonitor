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
MonitorWindow::MonitorWindow(int openClassID, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MonitorWindow)
{
	ui->setupUi(this);
	classID = openClassID;
	updateSchoolName();
}

/*! Destroys the MonitorWindow object. */
MonitorWindow::~MonitorWindow()
{
	delete ui;
}

/*! Updates schoolNameLabel. */
void MonitorWindow::updateSchoolName(void)
{
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	ui->schoolNameLabel->setText(settings.value("main/schoolname","?").toString() + " - " + classManager::className(classID));
}
