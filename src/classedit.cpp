/*
 * addclassdialog.cpp
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

#include "classedit.h"
#include "ui_classedit.h"

/*! Constructs classEdit. */
classEdit::classEdit(bool newClass, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::classEdit)
{
	ui->setupUi(this);
	ui->okButton->setEnabled(false);
	if(newClass)
		setWindowTitle(tr("New class"));
	else
		setWindowTitle(tr("Edit class"));
	// Connections
	connect(ui->nameEdit,&QLineEdit::textChanged,this,&classEdit::verify);
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

/*! Destroys the classEdit object. */
classEdit::~classEdit()
{
	delete ui;
}

/*!
 * Connected from nameEdit->textChanged().\n
 * Checks if all info is correct and enables the OK button.
 */
void classEdit::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check class name
	if(ui->nameEdit->text().count() == 0)
		return;
	QStringList classes = classManager::classNames();
	for(int i=0; i < classes.count(); i++)
	{
		if(classes[i] == ui->nameEdit->text())
			return;
	}
	// Everything is correct
	className = ui->nameEdit->text();
	ui->okButton->setEnabled(true);
}
