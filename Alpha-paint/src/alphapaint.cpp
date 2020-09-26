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

	ui.scrollArea->verticalScrollBar()->installEventFilter(this);
	ui.scrollArea->horizontalScrollBar()->installEventFilter(this);

	connect(ui.actionOpen, &QAction::triggered, this, &AlphaPaint::openFile);
	connect(ui.actionSave, &QAction::triggered, this, &AlphaPaint::saveFile);
	connect(ui.myLabelImage, &ImageLabel::signalMouseDragged, this, &AlphaPaint::onMouseDragged);
	connect(ui.myLabelImage, &ImageLabel::signalMouseMoved, this, &AlphaPaint::onMouseMoved);
	connect(ui.myLabelImage, &ImageLabel::signalMousePaint, this, &AlphaPaint::onMousePaint);
	connect(ui.myButtonShowAlpha, &QPushButton::clicked, this, &AlphaPaint::onButtonAlpha);
	connect(ui.mySliderAlphaValue, &QSlider::sliderMoved, this, &AlphaPaint::onSliderValueChanged);
	
	myStatusLabel = new QLabel(ui.statusBar);
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
bool AlphaPaint::eventFilter(QObject* object, QEvent* event)
{
	if (object == ui.scrollArea->verticalScrollBar() && event->type() == QEvent::Wheel) {
		return true;
	}
	if (object == ui.scrollArea->horizontalScrollBar() && event->type() == QEvent::Wheel) {
		return true;
	}

	return false;
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Image path"), "",
		tr("Image file (*.jpg *.jpeg *.png *.bmp);;All Files (*)"));
	if (fileName != "")
	{
		myImage.load(fileName);
		myImage.convertToFormat(QImage::Format_ARGB32);
		
		ui.myLabelImage->setPixmap(QPixmap::fromImage(myImage));
		ui.myLabelImage->adjustSize();
		ui.myLabelImage->reset();
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::saveFile()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save image"), "",
		tr("JPG (*.jpg);;PNG (*.png);;JPEG(*.jpeg);;BMP(*.bmp);;All Files (*)"));

	bool saved = ui.myLabelImage->pixmap()->save(fileName);
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::keyReleaseEvent(QKeyEvent* event)
{
	QMainWindow::keyReleaseEvent(event);

	if (event->key() == 'S' || event->key() == 's')
	{
		ui.myButtonShowAlpha->setChecked(!ui.myButtonShowAlpha->isChecked());
		affectPixmap(ui.myButtonShowAlpha->isChecked());
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::onMouseDragged(const QPointF& offset)
{
	auto hVal = ui.scrollArea->horizontalScrollBar()->value();
	ui.scrollArea->horizontalScrollBar()->setValue(hVal + offset.x());

	auto vVal = ui.scrollArea->verticalScrollBar()->value();
	ui.scrollArea->verticalScrollBar()->setValue(vVal + offset.y());
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::onMouseMoved(const QPoint& pos)
{
	if (myImage.rect().contains(pos))
	{			
		QRgb rgba = myImage.pixel(pos);
		int alpha = qAlpha(rgba);
		std::ostringstream oss;
		oss << alpha;
	
		ui.myEditAlpha->setText(QString::number(alpha));
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::onMousePaint(const QPoint& oldPos, const QPoint& pos)
{
	if (myImage.rect().contains(pos))
	{
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

		affectPixmap(ui.myButtonShowAlpha->isChecked());

		QRgb rgba2 = myImage.pixel(pos);
		ui.myEditAlpha->setText(QString::number(alpha));
	}
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::onButtonAlpha(bool clicked)
{
	affectPixmap(clicked);
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
void AlphaPaint::affectPixmap(bool alpha)
{
	const QPixmap* pix = ui.myLabelImage->pixmap();
	if (alpha)
	{
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
	myStatusLabel->setText(QString::number(value));
	myStatusLabel->repaint();
}