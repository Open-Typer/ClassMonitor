/*
 * classcontrols.cpp
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

#include "widgets/classcontrols.h"
#include "ui_classcontrols.h"

/*! Constructs classControls. */
classControls::classControls(int openClassID, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::classControls)
{
	ui->setupUi(this);
	classID = openClassID;
	setupTable();
}

/*! Destroys the classControls object. */
classControls::~classControls()
{
	delete ui;
}

/*! Loads the students. */
void classControls::setupTable(void)
{
	ui->studentsTable->clear();
	ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Columns
	ui->studentsTable->setColumnCount(1);
	QStringList columns;
	columns += tr("Name");
	ui->studentsTable->setHorizontalHeaderLabels({ tr("Name") });
	// Rows
	QStringList students = classManager::studentNames(classID);
	ui->studentsTable->setRowCount(students.count());
	for(int i=0; i < students.count(); i++)
	{
		QTableWidgetItem *item = new QTableWidgetItem(students[i]);
		ui->studentsTable->setItem(i,0,item);
	}
}
