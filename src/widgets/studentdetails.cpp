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
	ui->titleLabel->setText(classManager::studentName(classID,studentID));
	// Connections
	connect(ui->backButton,SIGNAL(clicked()),this,SLOT(goBack()));
	connect(ui->packBox,SIGNAL(activated(int)),this,SLOT(refresh()));
	connect(ui->lessonBox,SIGNAL(activated(int)),this,SLOT(refresh()));
	connect(ui->sublessonBox,SIGNAL(activated(int)),this,SLOT(refresh()));
	connect(ui->exerciseBox,SIGNAL(activated(int)),this,SLOT(refresh()));
	refresh();
}

/*! Destroys the studentDetails object. */
studentDetails::~studentDetails()
{
	delete ui;
}

/*! Connected from backButton->clicked().\n
 * Emits backClicked().
 */
void studentDetails::goBack(void)
{
	emit backClicked();
}

/*! Refreshes comboboxes and other widgets. */
void studentDetails::refresh(void)
{
	// Save old indexes
	int oldP, oldL, oldS, oldE;
	oldP = ui->packBox->currentIndex();
	oldL = ui->lessonBox->currentIndex();
	oldS = ui->sublessonBox->currentIndex();
	oldE = ui->exerciseBox->currentIndex();
	// Packs
	ui->packBox->clear();
	ui->packBox->addItems(classManager::studentPacks(classID,studentID));
	if(ui->packBox->count() == 0)
	{
		ui->exerciseFrame->hide();
		ui->chartFrame->hide();
		ui->noInfoFrame->show();
	}
	else
	{
		ui->exerciseFrame->show();
		ui->chartFrame->show();
		ui->noInfoFrame->hide();
	}
	if(oldP == -1)
		oldP = 0;
	ui->packBox->setCurrentIndex(oldP);
	// Lessons
	ui->lessonBox->clear();
	QList<int> lessons = classManager::studentLessons(classID,studentID,ui->packBox->currentText());
	ui->lessonBox->addItems(classManager::lessonList(lessons));
	if(oldL == -1)
		oldL = 0;
	ui->lessonBox->setCurrentIndex(oldL);
	// Sublessons
	ui->sublessonBox->clear();
	QList<int> sublessons = classManager::studentSublessons(classID,studentID,ui->packBox->currentText(),
		lessons[ui->lessonBox->currentIndex()]);
	ui->sublessonBox->addItems(classManager::sublessonList(sublessons));
	if(oldS == -1)
		oldS = 0;
	ui->sublessonBox->setCurrentIndex(oldS);
	// Exercise
	ui->exerciseBox->clear();
	QList<int> exercises = classManager::studentExercises(classID,studentID,ui->packBox->currentText(),
		lessons[ui->lessonBox->currentIndex()],
		sublessons[ui->sublessonBox->currentIndex()]);
	ui->exerciseBox->addItems(classManager::exerciseList(exercises));
	if(oldE == -1)
		oldE = 0;
	ui->sublessonBox->setCurrentIndex(oldE);
}
