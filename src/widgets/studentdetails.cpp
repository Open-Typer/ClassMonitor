/*
 * studentdetails.cpp
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

#include "widgets/studentdetails.h"
#include "ui_studentdetails.h"

/*! Constructs studentDetails. */
studentDetails::studentDetails(int openClassID, int id, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::studentDetails)
{
	ui->setupUi(this);
	classID = openClassID;
	studentID = id;
	// Connections
	connect(ui->backButton,SIGNAL(clicked()),this,SLOT(goBack()));
}

/*! Destroys the studentDetails object. */
studentDetails::~studentDetails()
{
	delete ui;
}

/*! Connected from backButton->clicked().\n
 * Emits goBack().
 */
void studentDetails::goBack(void)
{
	emit backClicked();
}
