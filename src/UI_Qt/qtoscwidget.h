#ifndef QTOSCWIDGET_H
#define QTOSCWIDGET_H

#include <QWidget>
#include "qtoscobject.h"
#include "qtoscpane.h"

#if 0
/**
 * @brief The QtOscWidget class
 * This class is useful because many of our widgets have QWidget as a base.
 */
class QtOscWidget : public QWidget, public QtOscObject
{
public:
	//! Ctor for widgets which have parents
	QtOscWidget(QtOscWidget* treeBase, const QString& subPath);
	//! Ctor for parent-less widgets
	QtOscWidget(QtOscObject* treeBase, const QString& subPath);
};
#endif

#endif // QTOSCWIDGET_H
