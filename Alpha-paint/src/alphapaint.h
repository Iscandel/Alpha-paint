#pragma once

#include "QtWidgets/QMainWindow"
#include "ui_alphapaint.h"

#include <QtWidgets>

class AlphaPaint : public QMainWindow
{
    Q_OBJECT

public:
    AlphaPaint(QWidget *parent = Q_NULLPTR);

	//bool eventFilter(QObject* object, QEvent* event);

public slots:
	void openFile();
	void onMouseDragged(const QPointF& offset);
	void onMouseMoved(const QPoint& pos);

private:
    Ui::AlphapaintClass ui;
	
	QImage myImage;
};