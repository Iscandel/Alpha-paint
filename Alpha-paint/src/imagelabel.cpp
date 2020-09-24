#include "imagelabel.h"

#include "qevent.h"
#include "qpainter.h"

#include <iostream>

ImageLabel::ImageLabel(QWidget *parent)
: QLabel(parent)
, myScale(qreal(1.))
, myInitialSizeX(0)
, myInitialSizeY(0)
{
	ui.setupUi(this);
	//NB mouseMoveEvent is only called if a mouse button is pressed (see MouseTracking to change the behaviour)
	setMouseTracking(true);
}

ImageLabel::~ImageLabel()
{
}

void ImageLabel::reset()
{
	if (pixmap())
	{
		myInitialSizeX = pixmap()->width();
		myInitialSizeY = pixmap()->height();
	}
	
	myScale = 1.f;
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void ImageLabel::paintEvent(QPaintEvent* event)
{
	//QLabel::paintEvent(event);

	QPainter painter(this);
	if (pixmap())
	{
		painter.resetTransform();
		painter.fillRect(0, 0, myInitialSizeX, myInitialSizeY, Qt::GlobalColor::lightGray);
		painter.scale(myScale, myScale);

		painter.drawPixmap(0, 0, *pixmap());
		//painter.setPen(QColor(128, 128, 128));
		//painter.drawRect(myStartX, myStartY, myEndX - myStartX, myEndY - myStartY);
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void ImageLabel::mousePressEvent(QMouseEvent* event)
{
	myButtonPressed = event->button();
	//myFirstMousePos = event->pos();

	if (event->button() == Qt::LeftButton)
	{
		myLastMousePos = event->screenPos();
	}

	repaint();
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
	//myPixmapOffset = QPoint();

	myButtonPressed = Qt::NoButton;
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void ImageLabel::mouseMoveEvent(QMouseEvent* event)
{
	
	if (myButtonPressed == Qt::LeftButton)
	{
		QPointF pixmapOffset = myLastMousePos - event->screenPos();

		emit signalMouseDragged(pixmapOffset);

		myLastMousePos = event->screenPos();
		repaint();
	}
	else if (pixmap())
	{
		QPoint pos(event->pos().x() * (1. / myScale), event->pos().y() * (1. / myScale));
		emit signalMouseMoved(pos);
	}
}

void ImageLabel::wheelEvent(QWheelEvent * event)
{
		double normalizedValue = (event->delta() / 120.); //To get the minimal stepsize in [-1, 1]
		double ratio = 1.;
		double minRatio = 0.7;
		if (event->delta() < 0)
			ratio = -minRatio / normalizedValue;
		else
			ratio = 1. / minRatio * normalizedValue;
		myScale *= ratio;

		this->setMinimumWidth(myInitialSizeX * myScale);
		this->setMinimumHeight(myInitialSizeY * myScale);
		adjustSize();
		update();
		repaint();
		//real startX = ratio * (0 - event->x()) + event->x();
		//real endX = ratio * (pixmap()->width() - event->x()) + event->x();
		//real startY = ratio * (0 - event->y()) + event->y();
		//real endY = ratio * (pixmap()->height() - event->y()) + event->y();

		//emit signalRightButtonDrawFractal(startX, startY, endX, endY, myAngle);

}
