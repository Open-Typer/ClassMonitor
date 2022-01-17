/*
 * authdialog.cpp
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

#include "authdialog.h"
#include "ui_authdialog.h"

/*! Constructs authDialog. */
authDialog::authDialog(QString userName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::authDialog)
{
	ui->setupUi(this);
	setStyleSheet(globalStyleSheet);
	ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
	password = ui->passwordEdit->text();
	if(userName != "")
		ui->passwordLabel->setText(tr("Password for ") + userName + ":");
	// Connections
	connect(ui->passwordEdit,SIGNAL(textChanged(QString)),this,SLOT(updateText(QString)));
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

/*! Destroys the authDialog object. */
authDialog::~authDialog()
{
	delete ui;
}

/*!
 * Connected from passwordEdit->textChanged().\n
 * Updates password attribute.
 */
void authDialog::updateText(QString text)
{
	password = text;
}
