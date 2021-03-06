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

#include "widgets/usermanager.h"
#include "ui_usermanager.h"

/*! Constructs userManagerWidget. */
userManagerWidget::userManagerWidget(int userLoginID, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::userManagerWidget)
{
	ui->setupUi(this);
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	ui->schoolNameEdit->setText(settings.value("main/schoolname","?").toString());
	loginID = userLoginID;
	setupList();
	// Connections
	connect(ui->backButton,SIGNAL(clicked()),this,SLOT(finish()));
	connect(ui->userList,&QListWidget::itemSelectionChanged,this,&userManagerWidget::verify);
	connect(ui->schoolNameEdit,&QLineEdit::textChanged,this,&userManagerWidget::verify);
	connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addUser()));
	connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(removeUser()));
	connect(ui->editButton,SIGNAL(clicked()),this,SLOT(editUser()));
}

/*! Destroys the userManagerWidget object. */
userManagerWidget::~userManagerWidget()
{
	delete ui;
}

/*! Loads the users. */
void userManagerWidget::setupList(void)
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
void userManagerWidget::verify(void)
{
	ui->backButton->setEnabled(false);
	// Check list of users
	bool enable = (ui->userList->currentRow() != -1);
	ui->editButton->setEnabled(enable);
	if((userManager::userIDs().value(ui->userList->currentRow()) != loginID) && enable)
		ui->removeButton->setEnabled(true);
	else
		ui->removeButton->setEnabled(false);
	// Check school name
	if(ui->schoolNameEdit->text() == "")
		return;
	// Everything is correct
	ui->backButton->setEnabled(true);
}

/*
 * Connected from backButton->clicked().\n
 * Saves school name and emits backClicked().
 */
void userManagerWidget::finish(void)
{
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	settings.setValue("main/schoolname",ui->schoolNameEdit->text());
	emit backClicked();
}

/*!
 * Overrides QDialog#closeEvent().\n
 * Emits finish().
 *
 * \see finish()
 */
void userManagerWidget::closeEvent(QCloseEvent *event)
{
	emit finish();
	event->accept();
}

/*!
 * Connected from addButton->clicked().\n
 * Opens userEdit and creates a new user.
 *
 * \see userEdit
 */
void userManagerWidget::addUser(void)
{
	userEdit dialog(true);
	dialog.exec();
	setupList();
}

/*!
 * Connected from removeButton->clicked().\n
 * Removes selected user.
 */
void userManagerWidget::removeUser(void)
{
	if(ui->userList->currentRow() == -1)
		return;
	int userID = userManager::userIDs().value(ui->userList->currentRow());
	QMessageBox confirmDialog;
	confirmDialog.setWindowTitle(tr("Confirm"));
	confirmDialog.setText(tr("Are you sure you want to remove user") + " " + userManager::userName(userID) + "?");
	confirmDialog.setInformativeText(tr("This will remove all classes the user owns!"));
	QPushButton *yesButton = confirmDialog.addButton(tr("Yes"),QMessageBox::YesRole);
	QPushButton *noButton = confirmDialog.addButton(tr("No"),QMessageBox::NoRole);
	confirmDialog.setIcon(QMessageBox::Question);
	confirmDialog.exec();
	if(confirmDialog.clickedButton() == yesButton)
	{
		if(userManager::auth(loginID))
			userManager::removeUser(userID);
	}
	else if(confirmDialog.clickedButton() == noButton)
		return;
	setupList();
}

/*!
 * Connected from editButton->clicked().\n
 * Opens userEdit and edits selected user.
 *
 * \see userEdit
 */
void userManagerWidget::editUser(void)
{
	userEdit dialog(false,userManager::userIDs().value(ui->userList->currentRow()));
	dialog.exec();
	setupList();
}
