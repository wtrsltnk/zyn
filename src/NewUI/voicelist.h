#ifndef VOICELIST_H
#define VOICELIST_H

#include "ui_voicelist.h"

class VoiceList:public QDialog, public Ui::VoiceList
{
    public:
        VoiceList(QString absoluteId, QWidget *parent = NULL);
};

#endif // VOICELIST_H
