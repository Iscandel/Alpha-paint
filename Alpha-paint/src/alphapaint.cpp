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
		QPixmap pixmap(fileName);
		ui.myLabelImage->setPixmap(pixmap);
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
	const QPixmap* pix = ui.myLabelImage->pixmap();
	if (pix && pix->rect().contains(pos))
	{	
		
		QImage image = pix->toImage();
		QRgb rgba = image.pixel(pos);
		int alpha = qAlpha(rgba);
		std::ostringstream oss;
		oss << alpha;
	
		ui.myEditAlpha->setText(QString::fromStdString(oss.str()));
		std::cout << pos.x() << " " << pos.y() << std::endl;
	}
	
}
