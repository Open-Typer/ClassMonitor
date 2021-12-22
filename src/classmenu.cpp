/*
 * classmenu.cpp
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

#include "classmenu.h"
#include "ui_classmenu.h"

/*! Constructs classMenu. */
classMenu::classMenu(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::classMenu)
{
	ui->setupUi(this);
	ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
	setWindowFlag(Qt::WindowMinimizeButtonHint,true);
	setupList();
	verify();
	// Connections
	connect(ui->openButton,SIGNAL(clicked()),this,SLOT(open()));
	connect(ui->usersButton,SIGNAL(clicked()),this,SLOT(openUserManager()));
	connect(ui->classList,&QListWidget::itemDoubleClicked,this,&classMenu::open);
	connect(ui->classList,SIGNAL(itemSelectionChanged()),this,SLOT(verify()));
	connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addClass()));
	connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(removeClass()));
	connect(ui->editButton,SIGNAL(clicked()),this,SLOT(editClass()));
}

/*! Destroys the classMenu object. */
classMenu::~classMenu()
{
	delete ui;
}

/*! Loads the classes. */
void classMenu::setupList(void)
{
	QDirIterator icons(":res/images/class-icons",QDirIterator::NoIteratorFlags);
	QStringList iconNames;
	while(icons.hasNext())
		iconNames += icons.next();
	iconNames.sort();
	ui->classList->clear();
	QList<int> classes = classManager::classIDs();
	for(int i=0; i < classes.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem(QIcon(iconNames[classManager::classIcon(classes[i])]),classManager::className(classes[i]));
		ui->classList->addItem(item);
	}
	verify();
}

/*!
 * Connected from classList->itemSelectionChanged().\n
 * Checks if there's a selected class and enables the open and remove buttons.
 */
void classMenu::verify(void)
{
	bool enable = (ui->classList->currentRow() != -1);
	ui->openButton->setEnabled(enable);
	ui->removeButton->setEnabled(enable);
	ui->editButton->setEnabled(enable);
}

/*!
 * Connected from openButton->clicked() and classList->itemDoubleClicked().\n
 * Opens selected class.
 */
void classMenu::open(void)
{
	classID = classManager::classIDs().value(ui->classList->currentRow());
	if((ui->classList->currentRow() != -1) && userManager::auth(classManager::classOwner(classID)))
	{
		classManager::setClassTimestamp(classID,QDateTime::currentDateTimeUtc());
		MonitorWindow *monitor = new MonitorWindow(classID);
		monitor->show();
		accept();
	}
}

/*!
 * Connected from addButton->clicked().\n
 * Opens classEdit and adds a new class.
 * \see classEdit
 */
void classMenu::addClass(void)
{
	classEdit dialog(true);
	if(dialog.exec() == QDialog::Accepted)
		setupList();
}

/*!
 * Connected from removeButton->clicked().\n
 * Removes selected class.
 */
void classMenu::removeClass(void)
{
	if(ui->classList->currentRow() == -1)
		return;
	int classID = classManager::classIDs().value(ui->classList->currentRow());
	QMessageBox confirmDialog;
	confirmDialog.setWindowTitle(tr("Confirm"));
	confirmDialog.setText(tr("Are you sure you want to remove class ") + classManager::className(classID) + "?");
	QPushButton *yesButton = confirmDialog.addButton(tr("Yes"),QMessageBox::YesRole);
	QPushButton *noButton = confirmDialog.addButton(tr("No"),QMessageBox::NoRole);
	confirmDialog.setIcon(QMessageBox::Question);
	confirmDialog.exec();
	if(confirmDialog.clickedButton() == yesButton)
	{
		if(userManager::auth(classManager::classOwner(classID)))
			classManager::removeClass(classID);
	}
	else if(confirmDialog.clickedButton() == noButton)
		return;
	setupList();
}

/*!
 * Connected from editButton->clicked().\n
 * Opens classEdit and edits selected class.
 *
 * \see classEdit
 */
void classMenu::editClass(void)
{
	if(ui->classList->currentRow() == -1)
		return;
	classEdit dialog(false,classManager::classIDs().value(ui->classList->currentRow()));
	dialog.exec();
	setupList();
}

/*!
 * Connected from usersButton->clicked().\n
 * Asks for administrator authentication and opens userManagerDialog.
 *
 * \see adminSelector
 * \see userManagerDialog
 */
void classMenu::openUserManager(void)
{
	adminSelector adminDialog;
	if(adminDialog.exec() == QDialog::Rejected)
		return;
	if(userManager::auth(adminDialog.userID))
	{
		userManagerDialog dialog(adminDialog.userID);
		dialog.exec();
	}
}
