#include "qtoscwidget.h"

QtOscWidget::QtOscWidget(QtOscWidget *treeBase, const QString &subPath)
	: QWidget(treeBase),
	QtOscObject(treeBase, subPath, this)
	//QtOscPane()
{
}

QtOscWidget::QtOscWidget(QtOscObject *treeBase, const QString &subPath)
	: QWidget(NULL),
	QtOscObject(treeBase, subPath, NULL)
{
}


