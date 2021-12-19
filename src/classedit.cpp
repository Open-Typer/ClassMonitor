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
	ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
	ui->okButton->setEnabled(false);
	if(newClass)
		setWindowTitle(tr("New class"));
	else
		setWindowTitle(tr("Edit class"));
	// Set up list of users
	ui->ownerBox->clear();
	ui->ownerBox->addItems(userManager::userNames());
	updateOwner(ui->ownerBox->currentText());
	// Connections
	connect(ui->nameEdit,&QLineEdit::textChanged,this,&classEdit::verify);
	connect(ui->ownerBox,SIGNAL(currentTextChanged(const QString)),this,SLOT(updateOwner(const QString)));
	connect(ui->passwordEdit,&QLineEdit::textChanged,this,&classEdit::verify);
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(finish()));
}

/*! Destroys the classEdit object. */
classEdit::~classEdit()
{
	delete ui;
}

/*!
 * Connected from all line edits' textChanged() signal.\n
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
	// Check password
	if(ui->passwordEdit->text() == "")
		return;
	// Everything is correct
	className = ui->nameEdit->text();
	classOwner = userManager::userIDs().value(ui->ownerBox->currentIndex());
	ui->okButton->setEnabled(true);
}

/*!
 * Connected from passwordEdit->currentTextChanged().\n
 * Updates passwordLabel.
 */
void classEdit::updateOwner(const QString name)
{
	ui->passwordLabel->setText("Password for " + name + ":");
	verify();
}

/*!
 * Connected from okButton->clicked().\n
 * Checks owner password and closes the dialog.
 */
void classEdit::finish(void)
{
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(ui->passwordEdit->text().toUtf8());
	QFile passwdFile(fileUtils::configLocation() + "/users/" + QString::number(userManager::userIDs().value(ui->ownerBox->currentIndex())) + "/passwd");
	if(passwdFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
	{
		if(passwdFile.readAll().compare(hash.result()) == 0)
			accept();
		else
		{
			QMessageBox errBox;
			errBox.setText(tr("Incorrect password!"));
			errBox.setStandardButtons(QMessageBox::Ok);
			errBox.setIcon(QMessageBox::Warning);
			errBox.exec();
		}
	}
}
