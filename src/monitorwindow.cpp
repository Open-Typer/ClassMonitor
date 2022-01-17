/*
 * monitorwindow.cpp
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

#include "monitorwindow.h"
#include "ui_monitorwindow.h"

/*! Constructs MonitorWindow. */
MonitorWindow::MonitorWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MonitorWindow)
{
	ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	classID = 0;
	settings = new QSettings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	setTheme();
	updateSchoolName();
	controlWidgets.clear();
	classMenu *newWidget = new classMenu();
	controlWidgets += newWidget;
	updateControlWidget();
	// Connections
	connect(ui->optionsButton,SIGNAL(clicked()),this,SLOT(openOptions()));
}

/*! Destroys the MonitorWindow object. */
MonitorWindow::~MonitorWindow()
{
	delete ui;
}

/*! Sets the theme. */
void MonitorWindow::setTheme(void)
{
	QString fileName;
	bool dark = settings->value("customization/darktheme","false").toBool();
	if(dark)
		fileName = ":/dark-theme/style.qss";
	else
		fileName = ":/light-theme/style.qss";
	QFile styleSheetFile(fileName);
	if(styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text))
		setStyleSheet(styleSheetFile.readAll());
	emit themeChanged(dark);
}

/*! Updates schoolNameLabel. */
void MonitorWindow::updateSchoolName(void)
{
	QString name = settings->value("main/schoolname","?").toString();
	if(classID == 0)
		ui->schoolNameLabel->setText(name);
	else
		ui->schoolNameLabel->setText(name + " - " + classManager::className(classID));
}

/*!
 * Connected from optionsButton->clicked().\n
 * Opens optionsWindow.
 */
void MonitorWindow::openOptions(void)
{
	optionsWindow dialog;
	dialog.setStyleSheet(styleSheet());
	dialog.exec();
	setTheme();
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
	if(controlWidgets.last()->metaObject()->indexOfSlot("changeTheme(bool)") != -1)
		connect(this,SIGNAL(themeChanged(bool)),controlWidgets.last(),SLOT(changeTheme(bool)));
	if(widgetClass == "classMenu")
	{
		connect(controlWidgets.last(),SIGNAL(classOpened(int)),this,SLOT(openClass(int)));
		connect(controlWidgets.last(),SIGNAL(userManagerOpened()),this,SLOT(openUserManager()));
	}
	else
	{
		connect(controlWidgets.last(),SIGNAL(backClicked()),this,SLOT(goBack()));
		if(widgetClass == "classControls")
			connect(controlWidgets.last(),SIGNAL(detailsClicked(int)),this,SLOT(openDetails(int)));
	}
	setTheme();
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
	updateSchoolName();
}

/*!
 * Connected from classMenu#classOpened().\n
 * Opens classControls.
 */
void MonitorWindow::openClass(int id)
{
	classID = id;
	outAnim();
	classControls *newWidget = new classControls(classID);
	controlWidgets += newWidget;
}

/*!
 * Connected from classMenu#userManagerOpened().\n
 * Opens userManagerWidget.
 */
void MonitorWindow::openUserManager(void)
{
	classID = 0;
	outAnim();
	userManagerWidget *newWidget = new userManagerWidget(classID);
	controlWidgets += newWidget;
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

/*!
 * Connected from moveAnim->valueChanged().\n
 * Shows move in animation with the new widget.
 */
void MonitorWindow::checkBackAnim(const QVariant value)
{
	if(value == moveAnim->endValue())
	{
		controlWidgets.removeLast();
		updateControlWidget();
		disconnect(moveAnim,nullptr,nullptr,nullptr);
		moveAnim = new QPropertyAnimation(ui->classControlsBody,"geometry");
		moveAnim->setDuration(63);
		QRect widgetGeometry = ui->classControlsBody->geometry();
		int oldWidth = widgetGeometry.width();
		widgetGeometry.setWidth(0);
		moveAnim->setStartValue(widgetGeometry);
		widgetGeometry.setWidth(oldWidth);
		moveAnim->setEndValue(widgetGeometry);
		moveAnim->start();
	}
}

/*!
 * Connected from backClicked() signal of current widget.\n
 * Shows inverted move out animation and switches to the previous widget.
 */
void MonitorWindow::goBack(void)
{
	disconnect(controlWidgets.last(),nullptr,nullptr,nullptr);
	QString widgetClass = controlWidgets.last()->metaObject()->className();
	if(widgetClass == "classControls")
		classID = 0;
	moveAnim = new QPropertyAnimation(ui->classControlsBody,"geometry");
	moveAnim->setDuration(63);
	QRect widgetGeometry = ui->classControlsBody->geometry();
	moveAnim->setStartValue(widgetGeometry);
	widgetGeometry.setX(geometry().width());
	moveAnim->setEndValue(widgetGeometry);
	connect(moveAnim,SIGNAL(valueChanged(const QVariant)),this,SLOT(checkBackAnim(const QVariant)));
	moveAnim->start();
	updateSchoolName();
}
