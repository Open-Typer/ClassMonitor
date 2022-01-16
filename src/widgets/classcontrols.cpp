/*
 * classcontrols.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#include "widgets/classcontrols.h"
#include "ui_classcontrols.h"

/*! Constructs classControls. */
classControls::classControls(int openClassID, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::classControls)
{
	ui->setupUi(this);
	classID = openClassID;
	setupTable();
	verify();
	// Set up charts
	speedChart = new QChart;
	mistakesChart = new QChart;
	timeChart = new QChart;
	speedChart->setTheme(QChart::ChartThemeDark);
	mistakesChart->setTheme(QChart::ChartThemeDark);
	timeChart->setTheme(QChart::ChartThemeDark);
	// Create chart views
	QChartView *speedChartView = new QChartView(speedChart, ui->chartFrame);
	QChartView *mistakesChartView = new QChartView(mistakesChart, ui->chartFrame);
	QChartView *timeChartView = new QChartView(timeChart, ui->chartFrame);
	ui->chartLayout->addWidget(speedChartView);
	ui->chartLayout->addWidget(mistakesChartView);
	ui->chartLayout->addWidget(timeChartView);
	// Init charts
	// Speed
	speedChart->setTitle(tr("Speed"));
	// Mistakes
	mistakesChart->setTitle(tr("Mistakes"));
	// Time
	timeChart->setTitle(tr("Time"));
	refreshCharts();
	// Connections
	connect(ui->backButton,SIGNAL(clicked()),this,SLOT(goBack()));
	connect(ui->studentsTable,SIGNAL(itemSelectionChanged()),this,SLOT(verify()));
	connect(ui->studentsTable,&QTableWidget::itemDoubleClicked,this,&classControls::openDetails);
	connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addStudent()));
	connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(removeStudent()));
	connect(ui->editButton,SIGNAL(clicked()),this,SLOT(editStudent()));
	connect(ui->detailsButton,SIGNAL(clicked()),this,SLOT(openDetails()));
	connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(refreshCharts()));
	connect(ui->packBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->lessonBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->sublessonBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->exerciseBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->refreshButton,SIGNAL(clicked()),this,SLOT(refreshCharts()));
}

/*! Destroys the classControls object. */
classControls::~classControls()
{
	delete ui;
}

/*! Connected from backButton->clicked().\n
 * Emits backClicked().
 */
void classControls::goBack(void)
{
	emit backClicked();
}

/*! Loads the students. */
void classControls::setupTable(void)
{
	ui->studentsTable->clear();
	ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Columns
	ui->studentsTable->setColumnCount(2);
	ui->studentsTable->setHorizontalHeaderLabels({ tr("Full name"), tr("Username") });
	// Rows
	QList<int> students = classManager::studentIDs(classID);
	ui->studentsTable->setRowCount(students.count());
	for(int i=0; i < students.count(); i++)
	{
		// Full name
		QTableWidgetItem *nameItem = new QTableWidgetItem(classManager::studentName(classID,students[i]));
		ui->studentsTable->setItem(i,0,nameItem);
		// Username
		QTableWidgetItem *usernameItem = new QTableWidgetItem(classManager::studentUsername(classID,students[i]));
		ui->studentsTable->setItem(i,1,usernameItem);
	}
}

/*!
 * Connected from studentsTable->itemSelectionChanged().\n
 * Checks if everything is correct and enables buttons.
 */
void classControls::verify(void)
{
	// Check students table
	bool enable = ui->studentsTable->selectionModel()->hasSelection();
	ui->editButton->setEnabled(enable);
	ui->removeButton->setEnabled(enable);
	ui->detailsButton->setEnabled(enable);
}

/*!
 * Connected from addButton->clicked().\n
 * Opens studentEdit and adds a new student.
 *
 * \see studentEdit
 */
void classControls::addStudent(void)
{
	studentEdit dialog(true,classID);
	dialog.exec();
	setupTable();
}

/*!
 * Connected from removeButton->clicked().\n
 * Removes selected student.
 */
void classControls::removeStudent(void)
{
	int studentID = classManager::studentIDs(classID).value(ui->studentsTable->selectionModel()->selectedRows()[0].row());
	QMessageBox confirmDialog;
	confirmDialog.setWindowTitle(tr("Confirm"));
	confirmDialog.setText(tr("Are you sure you want to remove student") + " " + classManager::studentName(classID, studentID) + "?");
	confirmDialog.setInformativeText(tr("This will remove whole training history of the student!"));
	QPushButton *yesButton = confirmDialog.addButton(tr("Yes"),QMessageBox::YesRole);
	QPushButton *noButton = confirmDialog.addButton(tr("No"),QMessageBox::NoRole);
	confirmDialog.setIcon(QMessageBox::Question);
	confirmDialog.exec();
	if(confirmDialog.clickedButton() == yesButton)
		classManager::removeStudent(classID, studentID);
	else if(confirmDialog.clickedButton() == noButton)
		return;
	setupTable();
}

/*!
 * Connected from editButton->clicked().\n
 * Opens studentEdit and edits the student.
 *
 * \see studentEdit
 */
void classControls::editStudent(void)
{
	studentEdit dialog(false,classID,classManager::studentIDs(classID).value(ui->studentsTable->selectionModel()->selectedRows()[0].row()));
	dialog.exec();
	setupTable();
}

/*!
 * Connected from detailsButton->clicked() and studentsTable->itemDoubleClicked().\n
 * Emits detailsClicked() signal.
 *
 * \see detailsClicked()
 */
void classControls::openDetails(void)
{
	emit detailsClicked(classManager::studentIDs(classID).value(ui->studentsTable->selectionModel()->selectedRows()[0].row()));
}

/*! Refreshes the charts. */
void classControls::refreshCharts(void)
{
	QList<int> studentIDs = classManager::studentIDs(classID);
	// Save old indexes
	int oldP, oldL, oldS, oldE;
	oldP = ui->packBox->currentIndex();
	oldL = ui->lessonBox->currentIndex();
	oldS = ui->sublessonBox->currentIndex();
	oldE = ui->exerciseBox->currentIndex();
	// Packs
	ui->packBox->clear();
	QStringList packs;
	packs.clear();
	for(int i=0; i < studentIDs.count(); i++)
	{
		QStringList list = classManager::studentPacks(classID,studentIDs[i]);
		for(int i=0; i < list.count(); i++)
		{
			if(!packs.contains(list[i]))
				packs += list[i];
		}
	}
	ui->packBox->addItems(packs);
	if(ui->packBox->count() == 0)
	{
		ui->exerciseFrame->hide();
		ui->chartFrame->hide();
		ui->refreshButton->hide();
		return;
	}
	else
	{
		ui->exerciseFrame->show();
		ui->chartFrame->show();
		ui->refreshButton->show();
	}
	if(oldP == -1)
		oldP = 0;
	ui->packBox->setCurrentIndex(oldP);
	// Lessons
	ui->lessonBox->clear();
	QList<int> lessons;
	lessons.clear();
	for(int i=0; i < studentIDs.count(); i++)
	{
		QList<int> list = classManager::studentLessons(classID,studentIDs[i],ui->packBox->currentText());
		for(int i=0; i < list.count(); i++)
		{
			if(!lessons.contains(list[i]))
				lessons += list[i];
		}
	}
	ui->lessonBox->addItems(classManager::lessonList(lessons));
	if(oldL == -1)
		oldL = 0;
	ui->lessonBox->setCurrentIndex(oldL);
	// Sublessons
	ui->sublessonBox->clear();
	QList<int> sublessons;
	sublessons.clear();
	for(int i=0; i < studentIDs.count(); i++)
	{
		QList<int> list = classManager::studentSublessons(classID,studentIDs[i],ui->packBox->currentText(),
			lessons[ui->lessonBox->currentIndex()]);
		for(int i=0; i < list.count(); i++)
		{
			if(!sublessons.contains(list[i]))
				sublessons += list[i];
		}
	}
	ui->sublessonBox->addItems(classManager::sublessonList(sublessons));
	if(oldS == -1)
		oldS = 0;
	ui->sublessonBox->setCurrentIndex(oldS);
	// Exercise
	ui->exerciseBox->clear();
	QList<int> exercises;
	exercises.clear();
	for(int i=0; i < studentIDs.count(); i++)
	{
		QList<int> list = classManager::studentExercises(classID,studentIDs[i],ui->packBox->currentText(),
			lessons[ui->lessonBox->currentIndex()],
			sublessons[ui->sublessonBox->currentIndex()]);
		for(int i=0; i < list.count(); i++)
		{
			if(!exercises.contains(list[i]))
				exercises += list[i];
		}
	}
	ui->exerciseBox->addItems(classManager::exerciseList(exercises));
	if(oldE == -1)
		oldE = 0;
	ui->exerciseBox->setCurrentIndex(oldE);
	// Refresh charts
	speedChart->removeAllSeries();
	mistakesChart->removeAllSeries();
	timeChart->removeAllSeries();
	QString pack = packs[ui->packBox->currentIndex()];
	int lesson = lessons[ui->lessonBox->currentIndex()];
	int sublesson = sublessons[ui->sublessonBox->currentIndex()];
	int exercise = exercises[ui->exerciseBox->currentIndex()];
	for(int i=0; i < studentIDs.count(); i++)
	{
		int i2, count = classManager::historySize(classID,studentIDs[i],pack,lesson,sublesson,exercise);
		QString studentName = classManager::studentName(classID,studentIDs[i]);
		QLineSeries *speedSeries = new QLineSeries;
		QLineSeries *mistakesSeries = new QLineSeries;
		QLineSeries *timeSeries = new QLineSeries;
		speedSeries->setName(studentName);
		mistakesSeries->setName(studentName);
		timeSeries->setName(studentName);
		for(i2=0; i2 < count; i2++)
		{
			QStringList entry = classManager::historyEntry(classID,studentIDs[i],pack,lesson,sublesson,exercise,i2);
			speedSeries->append(i2,entry[0].toInt());
			mistakesSeries->append(i2,entry[1].toInt());
			timeSeries->append(i2,entry[2].toInt());
		}
		speedChart->addSeries(speedSeries);
		speedChart->createDefaultAxes();
		mistakesChart->addSeries(mistakesSeries);
		mistakesChart->createDefaultAxes();
		timeChart->addSeries(timeSeries);
		timeChart->createDefaultAxes();
	}
}
