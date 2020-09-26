#include "alphapaint.h"
#include <qpixmap.h>

#include <sstream>
#include <iostream>

AlphaPaint::AlphaPaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//ui.scrollArea->verticalScrollBar()->blockSignals(true);
	//ui.scrollArea->horizontalScrollBar()->blockSignals(true);

	//ui.scrollArea->verticalScrollBar()->installEventFilter(this);
	//ui.scrollArea->horizontalScrollBar()->installEventFilter(this);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.myLabelImage, SIGNAL(signalMouseDragged(const QPointF&)), this, SLOT(onMouseDragged(const QPointF&)));
	connect(ui.myLabelImage, SIGNAL(signalMouseMoved(const QPoint&)), this, SLOT(onMouseMoved(const QPoint&)));
	connect(ui.myLabelImage, SIGNAL(signalMousePaint(const QPoint&, const QPoint&)), this, SLOT(onMousePaint(const QPoint&, const QPoint&)));
	connect(ui.myButtonShowAlpha, SIGNAL(clicked(bool)), this, SLOT(onButtonAlpha(bool)));

	//myProgress = ProgressDialog::ptr(new ProgressDialog);


	////QCheckBox* checkDragMode = new QCheckBox(ui.statusBar);
	////ui.mainToolBar->addWidget(checkDragMode);
	//connect(ui.actionDrag_mode, SIGNAL(toggled(bool)), this, SLOT(changeDragMode()));
}

//bool AlphaPaint::eventFilter(QObject* object, QEvent* event)
//{
//	if (object == ui.scrollArea->verticalScrollBar() && event->type() == QEvent::Wheel) {
//		return true;
//	}
//	if (object == ui.scrollArea->horizontalScrollBar() && event->type() == QEvent::Wheel) {
//		return true;
//	}
//
//	return false;
//}

void AlphaPaint::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Image path"), "",
		tr("Image file (*.jpg, *.jpeg, *.png, *.bmp);;All Files (*)"));
	if (fileName != "")
	{
		myImage.load(fileName);
		myImage.convertToFormat(QImage::Format_ARGB32);
		//QPixmap pixmap(fileName);
		ui.myLabelImage->setPixmap(QPixmap::fromImage(myImage));
		ui.myLabelImage->adjustSize();
		ui.myLabelImage->reset();
	}
	
	//myImage.load(fileName);
}

void AlphaPaint::onMouseDragged(const QPointF& offset)
{
	auto hVal = ui.scrollArea->horizontalScrollBar()->value();
	ui.scrollArea->horizontalScrollBar()->setValue(hVal + offset.x());

	auto vVal = ui.scrollArea->verticalScrollBar()->value();
	ui.scrollArea->verticalScrollBar()->setValue(vVal + offset.y());
}

void AlphaPaint::onMouseMoved(const QPoint& pos)
{
	//const QPixmap* pix = ui.myLabelImage->pixmap();

	if (myImage.rect().contains(pos))
	{			
		//QImage image = pix->toImage();
		QRgb rgba = myImage.pixel(pos);
		int alpha = qAlpha(rgba);
		std::ostringstream oss;
		oss << alpha;
	
		ui.myEditAlpha->setText(QString::fromStdString(oss.str()));
	}
}

void AlphaPaint::onMousePaint(const QPoint& oldPos, const QPoint& pos)
{
	if (myImage.rect().contains(pos))
	{
		QImage alphaImg = myImage.alphaChannel();
		//QImage image = pix->toImage();
		QRgb rgba = myImage.pixel(pos);
		int alpha = 128;
		QPainter painter(&myImage);
		QRgb newRgba = qRgba(qRed(rgba), qGreen(rgba), qBlue(rgba), alpha);
		//painter.setOpacity(alpha);
		painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);
		QColor col(newRgba);
		col.setAlpha(alpha);
		painter.setPen(col);
		painter.drawLine(oldPos, pos);
		//myImage.setPixel(pos, newRgba);

		//myImage.setAlphaChannel(alphaImg);

		ui.myLabelImage->repaint();

		std::ostringstream oss;
		oss << alpha;

		QRgb rgba2 = myImage.pixel(pos);
		ui.myEditAlpha->setText(QString::fromStdString(oss.str()));
	}
}

void AlphaPaint::onButtonAlpha(bool clicked)
{
	const QPixmap* pix = ui.myLabelImage->pixmap();
	if (clicked)
	{
		//QImage image = pix->toImage();
		QImage alpha = myImage.alphaChannel();

		ui.myLabelImage->setPixmap(QPixmap::fromImage(alpha));
	}
	else
	{
		ui.myLabelImage->setPixmap(QPixmap::fromImage(myImage));
		
	}
	
	//ui.myLabelImage->adjustSize();
	//ui.myLabelImage->reset();
}
