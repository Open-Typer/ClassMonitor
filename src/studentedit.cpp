/*
 * studentedit.cpp
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

#include "studentedit.h"
#include "ui_studentedit.h"

/*! Constructs studentEdit. */
studentEdit::studentEdit(bool newStudent, int class_id, int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::studentEdit)
{
	ui->setupUi(this);
	if(newStudent)
	{
		setWindowTitle(tr("New student"));
		ui->resetPasswordButton->hide();
	}
	else
	{
		setWindowTitle(tr("Edit student"));
		ui->passwordEdit->hide();
		ui->repeatPasswordLabel->hide();
		ui->repeatPasswordEdit->hide();
		ui->nameEdit->setText(classManager::studentName(class_id,id));
	}
	creatingNewStudent = newStudent;
	passwordReset = false;
	classID = class_id;
	if(creatingNewStudent)
	{
		QList<int> IDs = classManager::studentIDs(classID);
		int i, max = 0;
		for(i=0; i < IDs.count(); i++)
		{
			if(IDs[i] > max)
				max = IDs[i];
		}
		studentID = max+1;
	}
	else
		studentID = id;
	verify();
	// Connections
	connect(ui->nameEdit,&QLineEdit::textChanged,this,&studentEdit::verify);
	connect(ui->passwordEdit,&QLineEdit::textChanged,this,&studentEdit::verify);
	connect(ui->repeatPasswordEdit,&QLineEdit::textChanged,this,&studentEdit::verify);
	connect(ui->resetPasswordButton,SIGNAL(clicked()),this,SLOT(resetPassword()));
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(finish()));
}

/*! Destroys the studentEdit object. */
studentEdit::~studentEdit()
{
	delete ui;
}

/*!
 * Connected from all line edits.\n
 * Checks if everything is correct and enables the OK button.
 */
void studentEdit::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check name
	if(ui->nameEdit->text() == "")
		return;
	// Set username
	QString name = ui->nameEdit->text();
	QString username = QString::number(classID) + "_";
	for(int i=0; i < name.count(); i++)
	{
		if(name[i] == ' ')
			username += ".";
		else if(name[i].toLatin1() > 0)
			username += name[i].toLower();
		else
			username += "-";
	}
	username += QString::number(studentID);
	ui->usernameEdit->setText(username);
	if(creatingNewStudent || passwordReset)
	{
		// Check password
		if(ui->passwordEdit->text() == "")
			return;
		if(ui->passwordEdit->text() != ui->repeatPasswordEdit->text())
			return;
	}
	// Everything is correct
	ui->okButton->setEnabled(true);
}

/*
 * Connected from resetPasswordButton->clicked().\n
 * Shows password boxes.
 */
void studentEdit::resetPassword(void)
{
	ui->resetPasswordButton->hide();
	ui->passwordEdit->show();
	ui->repeatPasswordLabel->show();
	ui->repeatPasswordEdit->show();
	passwordReset = true;
	verify();
}

/*!
 * Connected from okButton->clicked().\n
 * Edits or creates the user and closes the window.
 */
void studentEdit::finish(void)
{
	if(creatingNewStudent)
		classManager::addStudent(classID,ui->nameEdit->text(),ui->usernameEdit->text(),ui->passwordEdit->text());
	else
	{
		if(passwordReset)
			classManager::editStudent(classID,studentID,ui->nameEdit->text(),ui->usernameEdit->text(),ui->passwordEdit->text());
		else
			classManager::editStudent(classID,studentID,ui->nameEdit->text(),ui->usernameEdit->text());
	}
	accept();
}
