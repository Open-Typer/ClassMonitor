/*
 * usermanager.cpp
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

#include "usermanager.h"
#include "ui_usermanager.h"

/*! Constructs userManagerDialog. */
userManagerDialog::userManagerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::userManagerDialog)
{
	ui->setupUi(this);
	ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	ui->schoolNameEdit->setText(settings.value("main/schoolname","?").toString());
	setupList();
	// Connections
	connect(ui->userList,&QListWidget::itemSelectionChanged,this,&userManagerDialog::verify);
	connect(ui->schoolNameEdit,&QLineEdit::textChanged,this,&userManagerDialog::verify);
	connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(finish()));
}

/*! Destroys the userManagerDialog object. */
userManagerDialog::~userManagerDialog()
{
	delete ui;
}

/*! Loads the users. */
void userManagerDialog::setupList(void)
{
	ui->userList->clear();
	QList<int> users = userManager::userIDs();
	for(int i=0; i < users.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem(userManager::userName(users[i]));
		ui->userList->addItem(item);
	}
	verify();
}

/*!
 * Connected from userList->itemSelectionChanged() and schoolNameEdit->textChanged().\n
 * Checks if there's a selected user and enables the open and remove buttons,
 * checks if the school name is correct and enables the close button.
 */
void userManagerDialog::verify(void)
{
	ui->closeButton->setEnabled(false);
	// Check list of users
	bool enable = (ui->userList->currentRow() != -1);
	ui->removeButton->setEnabled(enable);
	ui->editButton->setEnabled(enable);
	// Check school name
	if(ui->schoolNameEdit->text() == "")
		return;
	// Everything is correct
	ui->closeButton->setEnabled(true);
}

/*
 * Connected from closeButton->clicked().\n
 * Saves school name and closes the window.
 */
void userManagerDialog::finish(void)
{
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	settings.setValue("main/schoolname",ui->schoolNameEdit->text());
	accept();
}
