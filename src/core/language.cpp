/*
 * language.cpp
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

#include "core/language.h"

/*! Constructs languageManager. */
languageManager::languageManager(QObject *parent) :
	QObject(parent)
{
	supportedLanguages.clear();
	supportedCountries.clear();
	// Supported languages
	// de_DE
	// TODO: Add German translations
	/*supportedLanguages += QLocale::German;
	supportedCountries += QLocale::Germany;*/
	// en_US
	supportedLanguages += QLocale::English;
	supportedCountries += QLocale::UnitedStates;
	// sk_SK
	supportedLanguages += QLocale::Slovak;
	supportedCountries += QLocale::Slovakia;
	// QStringList for combo boxes and list widgets
	boxItems.clear();
	for(int i=0; i < supportedLanguages.count(); i++)
		boxItems += QLocale::languageToString(supportedLanguages[i]) + " (" + QLocale::countryToString(supportedCountries[i]) + ")";
	boxItems.insert(0,tr("System (default)"));
}

/*! Destroys the languageManager object. */
languageManager::~languageManager() { }

/*! Loads language from settings and sets the application translator. */
void languageManager::setLanguage(void)
{
	// Load language
	QSettings *settings = new QSettings(fileUtils::configLocation() + "/settings.ini",QSettings::IniFormat);
	QString lang = settings->value("main/language","").toString();
	QTranslator *translator = new QTranslator;
	QLocale targetLocale;
	if(lang == "")
		targetLocale = QLocale::system();
	else
	{
		int index = boxItems.indexOf(lang);
		targetLocale = QLocale(supportedLanguages[index-1],supportedCountries[index-1]);
	}
	// Remove all translators
	for(int i=0; i < globalTranslators.count(); i++)
		QCoreApplication::removeTranslator(globalTranslators[i]);
	globalTranslators.clear();
	if(translator->load(targetLocale,QLatin1String("ClassMonitor"),QLatin1String("_"),QLatin1String(":/res/lang")))
		QCoreApplication::installTranslator(translator);
	globalTranslators += translator;
}
