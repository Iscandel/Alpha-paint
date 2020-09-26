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
	void onButtonAlpha(bool clicked);
	void onMousePaint(const QPoint& oldPos, const QPoint& pos);
	void onSliderValueChanged(int value);

private:
    Ui::AlphapaintClass ui;

	QLabel* myStatusLabel;
	
	QImage myImage;
};
