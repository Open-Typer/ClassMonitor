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
	setupList();
	verify();
	// Connections
	connect(ui->openButton,SIGNAL(clicked()),this,SLOT(open()));
	connect(ui->classList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(open(QListWidgetItem*)));
	connect(ui->classList,SIGNAL(itemSelectionChanged()),this,SLOT(verify()));
	connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addClass()));
}

/*! Destroys the classMenu object. */
classMenu::~classMenu()
{
	delete ui;
}

/*! Loads the classes. */
void classMenu::setupList(void)
{
	ui->classList->clear();
	QList<int> classes = classManager::classIDs();
	for(int i=0; i < classes.count(); i++)
	{
		// TODO: Use icon in the QListWidgetItem constructor.
		QListWidgetItem *item = new QListWidgetItem(classManager::className(classes[i]));
		ui->classList->addItem(item);
	}
	verify();
}

/*!
 * Connected from classList->itemSelectionChanged().\n
 * Checks if there's a selected class and enables the open button.
 */
void classMenu::verify(void)
{
	ui->openButton->setEnabled(ui->classList->currentRow() != -1);
}

/*!
 * Connected from openButton->clicked().\n
 * Opens selected class.
 */
void classMenu::open(void)
{
	if(ui->classList->currentRow() != -1)
	{
		classID = ui->classList->currentRow();
		accept();
	}
}

/*!
 * Connected from classList->itemDoubleClicked().\n
 * Opens double-clicked class.
 */
void classMenu::open(QListWidgetItem* item)
{
	classID = ui->classList->row(item);
	accept();
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
		classManager::addClass(dialog.className);
	setupList();
}
