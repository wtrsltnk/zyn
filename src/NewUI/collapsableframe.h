#ifndef COLLAPSABLEFRAME_H
#define COLLAPSABLEFRAME_H

#include <QFrame>
#include "ui_collapsableframe.h"

class CollapsableFrame : public QFrame, public Ui::CollapsableFrame
{
    Q_OBJECT

    public:
        CollapsableFrame(QWidget *parent, QWidget *expanded, QWidget *collapsed);
        void setExpandedWidget(QWidget *widget);
        void setCollapsedWidget(QWidget *widget);

    public slots:
        void toggleCollapsed();

    private:
        QWidget *expanded, *collapsed;

};

#endif // COLLAPSABLEFRAME_H