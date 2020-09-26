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
	connect(ui.mySliderAlphaValue, SIGNAL(sliderMoved(int)), this, SLOT(onSliderValueChanged(int)));

	myStatusLabel = new QLabel(ui.statusBar);

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
		tr("Image file (*.jpg *.jpeg *.png *.bmp);;All Files (*)"));
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
	
		ui.myEditAlpha->setText(QString::number(alpha));
	}
}

void AlphaPaint::onMousePaint(const QPoint& oldPos, const QPoint& pos)
{
	if (myImage.rect().contains(pos))
	{
		//QImage alphaImg = myImage.alphaChannel();
		//QImage image = pix->toImage();
		QRgb rgba = myImage.pixel(pos);
		int alpha = ui.mySliderAlphaValue->value();
		QPainter painter(&myImage);
		QRgb newRgba = qRgba(qRed(rgba), qGreen(rgba), qBlue(rgba), alpha);
		//painter.setOpacity(alpha);
		painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);
		QColor col(newRgba);
		col.setAlpha(alpha);
		QPen pen(col);
		pen.setWidth(ui.mySpinPenThickness->value());
		painter.setPen(pen);
		painter.drawLine(oldPos, pos);
		std::cout << oldPos.x() << " " << oldPos.y() << " " << pos.x() << " " << pos.y() << std::endl;
		myImage.setPixel(pos, newRgba);

		//myImage.setAlphaChannel(alphaImg);

		ui.myLabelImage->repaint();
		affectPixmap(ui.myButtonShowAlpha->isChecked());
		//ui.myLabelImage->setPixmap(QPixmap::fromImage(myImage));

		std::ostringstream oss;
		oss << alpha;

		QRgb rgba2 = myImage.pixel(pos);
		ui.myEditAlpha->setText(QString::fromStdString(oss.str()));
	}
}

void AlphaPaint::onButtonAlpha(bool clicked)
{
	affectPixmap(clicked);
	
	//ui.myLabelImage->adjustSize();
	//ui.myLabelImage->reset();
}

void AlphaPaint::affectPixmap(bool alpha)
{
	const QPixmap* pix = ui.myLabelImage->pixmap();
	if (alpha)
	{
		//QImage image = pix->toImage();
		QImage alpha = myImage.alphaChannel();

		ui.myLabelImage->setPixmap(QPixmap::fromImage(alpha));
	}
	else
	{
		ui.myLabelImage->setPixmap(QPixmap::fromImage(myImage));
	}
}

void AlphaPaint::onSliderValueChanged(int value)
{
	//ui.mySliderAlphaValue->setStatusTip(QString::fromStdString(oss.str()));
	//ui.mySliderAlphaValue->update();
	//ui.mySliderAlphaValue->repaint();
	//ui.statusBar->setStatusTip(QString::fromStdString(oss.str()));
	//ui.statusBar->repaint();
	myStatusLabel->setText(QString::number(value));
	myStatusLabel->repaint();
}