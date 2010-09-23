#ifndef NEWUI_PREFERENCES_H
#define NEWUI_PREFERENCES_H

#include "ui_Preferences.h"
#include <QDialog>
#include <QMap>

class Preferences : public QDialog, Ui::Preferences
{
    Q_OBJECT

    public:
        Preferences(QWidget *parent);

    private slots:
        void on_actionAudio_changed_triggered();
        void on_actionMidi_changed_triggered();

    private:
        QMap<QString, QRadioButton*> audioMap;
        QMap<QString, QRadioButton*> midiMap;

};

#endif // NEWUI_PREFERENCES_H
