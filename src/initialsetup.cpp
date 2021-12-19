/*
 * initialsetup.cpp
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

#include "initialsetup.h"
#include "ui_initialsetup.h"

/*! Constructs initialSetup. */
initialSetup::initialSetup(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::initialSetup)
{
	ui->setupUi(this);
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	ui->schoolNameEdit->setText(settings.value("main/schoolname","").toString());
	verify();
	// Connections
	connect(ui->schoolNameEdit,&QLineEdit::textChanged,this,&initialSetup::verify);
	connect(ui->adminNameEdit,&QLineEdit::textChanged,this,&initialSetup::verify);
	connect(ui->adminPasswordEdit,&QLineEdit::textChanged,this,&initialSetup::verify);
	connect(ui->repeatPasswordEdit,&QLineEdit::textChanged,this,&initialSetup::verify);
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(save()));
}

/*! Destroys the initialSetup object. */
initialSetup::~initialSetup()
{
	delete ui;
}

/*! Connected from all line edits' %textChanged() signal.\n
 * Checks if everything is correct and enables the OK button.
 */
void initialSetup::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check school name
	if(ui->schoolNameEdit->text() == "")
		return;
	// Check administrator name
	if(ui->adminNameEdit->text() == "")
		return;
	// Check administrator password
	if(ui->adminPasswordEdit->text() == "")
		return;
	if(ui->adminPasswordEdit->text() != ui->repeatPasswordEdit->text())
		return;
	// Everything is correct
	ui->okButton->setEnabled(true);
}

/*!
 * Connected from okButton->clicked().\n
 * Saves settings and closes the dialog.
 */
void initialSetup::save(void)
{
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	settings.setValue("main/schoolname",ui->schoolNameEdit->text());
	userManager::addUser(ui->adminNameEdit->text(),"admin",ui->adminPasswordEdit->text());
	accept();
}
