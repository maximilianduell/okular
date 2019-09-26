/***************************************************************************
 *   Copyright (C) 2008 by Pino Toscano <pino@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "tts.h"

#include <qdbusservicewatcher.h>
#include <qset.h>

#include <KLocalizedString>

#include "settings.h"

/* Private storage. */
class OkularTTS::Private
{
public:
    Private( OkularTTS *qq )
        : q( qq ), speech( new QTextToSpeech( Okular::Settings::ttsEngine() ) )
    {
    }

    ~Private()
    {
        delete speech;
        speech = nullptr;
    }

    OkularTTS *q;
    QTextToSpeech *speech;
};

OkularTTS::OkularTTS( QObject *parent )
    : QObject( parent ), d( new Private( this ) )
{
    connect( d->speech, &QTextToSpeech::stateChanged, this, &OkularTTS::slotSpeechStateChanged);
}

OkularTTS::~OkularTTS()
{
    delete d;
}

void OkularTTS::say( const QString &text )
{
    if ( text.isEmpty() )
        return;

    d->speech->say( text );
}

void OkularTTS::stopAllSpeechs()
{
    if ( !d->speech )
        return;

    d->speech->stop();
}

void OkularTTS::pauseResumeSpeech()
{
    if ( !d->speech )
        return;

    if ( d->speech->state() == QTextToSpeech::Speaking )
        d->speech->pause();
    else
        d->speech->resume();
}

void OkularTTS::slotSpeechStateChanged(QTextToSpeech::State state)
{
    if (state == QTextToSpeech::Speaking)
    {
        emit isSpeaking(true);
        emit canPauseOrResume(true);
    }
    else
    {
        emit isSpeaking(false);
        if (state == QTextToSpeech::Paused)
            emit canPauseOrResume(true);
        else
            emit canPauseOrResume(false);
    }
}

#include "moc_tts.cpp"

