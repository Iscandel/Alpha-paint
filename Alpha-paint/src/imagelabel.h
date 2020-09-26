#pragma once

#include <QLabel>
#include "ui_ImageLabel.h"

class ImageLabel : public QLabel
{
	Q_OBJECT

public:
	ImageLabel(QWidget *parent = Q_NULLPTR);
	~ImageLabel();

	void paintEvent(QPaintEvent* event) override;

	void wheelEvent(QWheelEvent * event) override;

	void mousePressEvent(QMouseEvent* event) override;

	void mouseReleaseEvent(QMouseEvent *event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	QPoint posToImagePos(const QPoint& pos);

	void reset();

signals:
	void signalMouseDragged(const QPointF& offset);
	void signalMouseMoved(const QPoint& pos);
	void signalMousePaint(const QPoint& oldPos, const QPoint& pos);

protected:
	qreal myScale;
	int myInitialSizeX;
	int myInitialSizeY;

	Qt::MouseButton myButtonPressed;

	QPointF myLastMousePos;
	QPoint myLastRightMousePos;
	//QPoint myPixmapOffset;

private:
	Ui::ImageLabel ui;
};
