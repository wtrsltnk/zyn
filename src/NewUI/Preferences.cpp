#include "Preferences.h"
#include "../Nio/Nio.h"

#include <QtDebug>

Preferences::Preferences(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    audioMap["ALSA"] = audioAlsa;
    audioMap["JACK"] = audioJack;
    audioMap["OSS"] = audioOss;
    audioMap["NULL"] = audioNull;

    midiMap["ALSA"] = midiAlsa;
    midiMap["JACK"] = midiJack;
    midiMap["OSS"] = midiOss;
    midiMap["NULL"] = midiNull;

    QString currentAudio = QString::fromStdString(Nio::getInstance().getSink());
    QRadioButton *audioToActivate = audioMap[currentAudio];

    if (!audioToActivate) {
        qDebug() << "Warning: unknown audio set: " << currentAudio;
    } else {
        audioToActivate->setChecked(true);
    }

    QString currentMidi = QString::fromStdString(Nio::getInstance().getSource());
    QRadioButton *midiToActivate = midiMap[currentMidi];

    if (!midiToActivate) {
        qDebug() << "Warning: unknown midi set: " << currentMidi;
    } else {
        midiToActivate->setChecked(true);
    }


}

void Preferences::on_actionAudio_changed_triggered()
{
    foreach (const QString& key, audioMap.keys()) {
        if (audioMap[key]->isChecked()) {

            if (key.toStdString() == Nio::getInstance().getSink()) {
                qDebug() << "Already set.";
                return;
            }

            bool wasSet = Nio::getInstance().setSink(key.toStdString());
            qDebug() << "wasSet: " << wasSet;
        }
    }
}

void Preferences::on_actionMidi_changed_triggered()
{
    foreach (const QString& key, midiMap.keys()) {
        if (midiMap[key]->isChecked()) {
            if (key.toStdString() == Nio::getInstance().getSource()) {
                qDebug() << "Already set.";
                return;
            }
            bool wasSet = Nio::getInstance().setSource(key.toStdString());
            qDebug() << "wasSet: " << wasSet;
        }
    }
}

#include "Preferences.moc"
// vim: sw=4 sts=4 et tw=100
