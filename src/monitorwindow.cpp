/*
 * monitorwindow.cpp
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

#include "monitorwindow.h"
#include "ui_monitorwindow.h"

/*! Constructs MonitorWindow. */
MonitorWindow::MonitorWindow(int openClassID, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MonitorWindow)
{
	ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	classID = openClassID;
	updateSchoolName();
	controlWidgets.clear();
	classControls *newWidget = new classControls(classID);
	controlWidgets += newWidget;
	updateControlWidget();
}

/*! Destroys the MonitorWindow object. */
MonitorWindow::~MonitorWindow()
{
	delete ui;
}

/*! Updates schoolNameLabel. */
void MonitorWindow::updateSchoolName(void)
{
	QSettings settings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	ui->schoolNameLabel->setText(settings.value("main/schoolname","?").toString() + " - " + classManager::className(classID));
}

/*! Sets current control widget. */
void MonitorWindow::updateControlWidget(void)
{
	// Remove all widgets
	for(int i=0; i < ui->classControlsBody->count(); i++)
	{
		disconnect(ui->classControlsBody->widget(i),nullptr,nullptr,nullptr);
		ui->classControlsBody->removeWidget(ui->classControlsBody->widget(i));
	}
	// Add current widget
	ui->classControlsBody->addWidget(controlWidgets.last());
	QString widgetClass = controlWidgets.last()->metaObject()->className();
	if(widgetClass == "classControls")
		connect(controlWidgets.last(),SIGNAL(detailsClicked(int)),this,SLOT(openDetails(int)));
}

/*! Shows move out animation. */
void MonitorWindow::outAnim(void)
{
	disconnect(controlWidgets.last(),nullptr,nullptr,nullptr);
	moveAnim = new QPropertyAnimation(ui->classControlsBody,"geometry");
	moveAnim->setDuration(63);
	QRect widgetGeometry = ui->classControlsBody->geometry();
	moveAnim->setStartValue(widgetGeometry);
	widgetGeometry.setWidth(0);
	moveAnim->setEndValue(widgetGeometry);
	connect(moveAnim,SIGNAL(valueChanged(const QVariant)),this,SLOT(checkAnim(const QVariant)));
	moveAnim->start();
}

/*!
 * Connected from classControls#detailsClicked().\n
 * Opens student details.
 */
void MonitorWindow::openDetails(int id)
{
	outAnim();
	studentDetails *newWidget = new studentDetails(classID,id);
	controlWidgets += newWidget;
}

/*!
 * Connected from moveAnim->valueChanged().\n
 * Shows move in animation with the new widget.
 */
void MonitorWindow::checkAnim(const QVariant value)
{
	if(value == moveAnim->endValue())
	{
		updateControlWidget();
		disconnect(moveAnim,nullptr,nullptr,nullptr);
		moveAnim = new QPropertyAnimation(ui->classControlsBody,"geometry");
		moveAnim->setDuration(63);
		QRect widgetGeometry = ui->classControlsBody->geometry();
		widgetGeometry.setX(geometry().width());
		moveAnim->setStartValue(widgetGeometry);
		widgetGeometry.setX(0);
		moveAnim->setEndValue(widgetGeometry);
		moveAnim->start();
	}
}
