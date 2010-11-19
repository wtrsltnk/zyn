/*
  ZynAddSubFX - a software synthesizer

  GroupBox.cpp - A generic groupbox that has additional controls related to the widgets contained
  inside it

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "GroupBox.h"
#include "ControlHelper.h"

#include <QFontMetrics>
#include <QMessageBox>
#include <QStyleOptionFrame>
#include <QtDebug>
#include <QToolButton>
#include <QAction>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>

static const int ButtonTopMargin = 10;
static const int ButtonSideMargin = 5;
static const int ButtonSpacing = 5;
static const int ButtonDimensions = 20;

GroupBox::GroupBox(QWidget *parent)
    : QGroupBox(parent)
{
    int nextButtonX = ButtonSideMargin;

    //Menu button
    QToolButton *b = new QToolButton(this);
    b->move(nextButtonX, ButtonTopMargin);
    b->resize(ButtonDimensions, ButtonDimensions);
    b->setFocusPolicy(Qt::NoFocus);
    b->setArrowType(Qt::NoArrow);
    b->setAutoRaise(true);
    b->setPopupMode(QToolButton::InstantPopup);

    nextButtonX += ButtonDimensions + ButtonSpacing;

    //Copy button
    QToolButton *copy = new QToolButton(this);
    copy->move(nextButtonX, ButtonTopMargin);
    copy->resize(ButtonDimensions, ButtonDimensions);
    copy->setFocusPolicy(Qt::NoFocus);
    copy->setArrowType(Qt::NoArrow);
    copy->setAutoRaise(true);
    copy->setPopupMode(QToolButton::InstantPopup);
    copy->setText("C");

    nextButtonX += ButtonDimensions + ButtonSpacing;

    //Paste button
    QToolButton *paste = new QToolButton(this);
    paste->move(nextButtonX, ButtonTopMargin);
    paste->resize(ButtonDimensions, ButtonDimensions);
    paste->setFocusPolicy(Qt::NoFocus);
    paste->setArrowType(Qt::NoArrow);
    paste->setAutoRaise(true);
    paste->setPopupMode(QToolButton::InstantPopup);
    paste->setText("P");

    QAction *action = new QAction("Reset to defaults", b);
    b->addAction(action);
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotReset()));

    action = new QAction("Copy...", b);
    b->addAction(action);
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotCopy()));
    connect(copy, SIGNAL(clicked()),
            action, SLOT(trigger()));

    action = new QAction("Paste...", b);
    b->addAction(action);
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotPaste()));
    connect(paste, SIGNAL(clicked()),
            action, SLOT(trigger()));
}

void GroupBox::paintEvent(QPaintEvent * event)
{
    QPainter p(this);
    QStyleOptionFrame frameOpt;
    frameOpt.initFrom(this);

    qApp->style()->drawPrimitive(QStyle::PE_FrameGroupBox, &frameOpt,
            &p, this);

    QFontMetrics fm(font());
    p.drawText(QPoint(0,fm.height()), title());
}

void GroupBox::slotReset()
{
    QList<ControlHelper *> controlHelpers =
        findChildren<ControlHelper *>();

    foreach(ControlHelper * helper, controlHelpers)
    {
        helper->defaults();
    }
}

void GroupBox::slotCopy()
{
    QMessageBox::information(this, QString(), "Copy is not yet implemented");
}

void GroupBox::slotPaste()
{
    QMessageBox::information(this, QString(), "Paste is not yet implemented");
}

#include "GroupBox.moc"
// vim: sw=4 sts=4 et tw=100
