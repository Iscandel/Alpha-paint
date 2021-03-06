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
	//NB Enable mouse tracking to get mouse move events
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
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void ImageLabel::mousePressEvent(QMouseEvent* event)
{
	myButtonPressed = event->button();

	if (event->button() == Qt::LeftButton)
	{
		myLastMousePos = event->screenPos();
	}
	else if (event->button() == Qt::RightButton)
	{
		myLastRightMousePos = event->pos();
		emit signalMousePaint(posToImagePos(myLastRightMousePos), posToImagePos(event->pos()));
	}

	repaint();
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
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
	else if (myButtonPressed == Qt::RightButton)
	{	
		emit signalMousePaint(posToImagePos(myLastRightMousePos), posToImagePos(event->pos()));		
		myLastRightMousePos = event->pos();
		repaint();
	}
	else if (pixmap())
	{
		emit signalMouseMoved(posToImagePos(event->pos()));
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
QPoint ImageLabel::posToImagePos(const QPoint& pos)
{
	return QPoint(pos.x() * (1. / myScale), pos.y() * (1. / myScale));
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
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
}
