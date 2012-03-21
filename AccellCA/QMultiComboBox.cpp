/****************************************************************************
**
** Copyright (C) 2010 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** Observe the License Information
**
****************************************************************************/

#include "QMultiComboBox.h"


QMultiComboBox::QMultiComboBox(QWidget *widget ) :
    QComboBox(widget),
    popheight_(200),
    popframe_(NULL, Qt::Popup)
{

    m_DisplayText_ = "Not Set";

    // setup the popup list
    vlist_.setSelectionMode(QAbstractItemView::MultiSelection);
    vlist_.setSelectionBehavior(QAbstractItemView::SelectItems);
    vlist_.clearSelection();
    popframe_.setLayout(new QVBoxLayout());
    popframe_.layout()->addWidget(&vlist_);
    popframe_.layout()->setContentsMargins(0,0,0,0);

    connect(&vlist_, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(scanItemSelect(QListWidgetItem*)));

}


QMultiComboBox::~QMultiComboBox()
{
    disconnect(&vlist_,0,0,0);
}


void QMultiComboBox::SetDisplayText(QString text)
{
    m_DisplayText_ = text;
    const int textWidth = fontMetrics().width(text);
    setMinimumWidth(textWidth + 30);
    updateGeometry();
    repaint();
}


QString QMultiComboBox::GetDisplayText() const
{
    return m_DisplayText_;
}


void QMultiComboBox::setPopupHeight(int h)
{
    popheight_ = h;
}


void QMultiComboBox::paintEvent(QPaintEvent *e)
{
    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));
    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;

    initStyleOption(&opt);
    opt.currentText = m_DisplayText_;
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);
    // draw the icon and text
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}


void QMultiComboBox::showPopup()
{

    QRect rec = QRect(geometry());
    QPoint p = this->mapToGlobal(QPoint(0,rec.height()));
    QRect rec2(p , p + QPoint(rec.width(), rec.height()));

    // height should be adjusted dynamically
    rec2.setHeight(popheight_);

    popframe_.setGeometry(rec2);
    popframe_.raise();
    popframe_.show();
}


void QMultiComboBox::hidePopup()
{
    popframe_.hide();
}


void QMultiComboBox::addItem ( const QString & text, const QVariant & userData)
{
    cout << __FUNCTION__ << endl;
    QListWidgetItem* wi = new QListWidgetItem(text);
    wi->setFlags(wi->flags() | Qt::ItemIsUserCheckable);
    if (userData.toBool())
        wi->setCheckState(Qt::Checked);
    else
        wi->setCheckState(Qt::Unchecked);

    vlist_.addItem(wi);
}


int QMultiComboBox::count()
{
    return vlist_.count();
}


void QMultiComboBox::setCurrentIndex(int index)
{
    cout << __FUNCTION__ << "DONT USE THIS ................" << endl;
}


QString QMultiComboBox::currentText()
{
    return vlist_.currentItem()->text();
}


QString QMultiComboBox::itemText(int row)
{
    return vlist_.item(row)->text();
}


QVariant QMultiComboBox::itemData(int row)
{
    QListWidgetItem* item = vlist_.item(row);
    if (item->checkState() == Qt::Checked) return QVariant(true);
    return QVariant(false);
}

void QMultiComboBox::setItemData(int index, const QVariant &value, int role)
{
    QListWidgetItem* item = vlist_.item(index);

	if(value == QVariant(true)) {
		item->setCheckState(Qt::Checked);
	}
	else {
		item->setCheckState(Qt::Unchecked);
	}

}


void QMultiComboBox::scanItemSelect(QListWidgetItem* item)
{

    QList<QListWidgetItem*> list = vlist_.selectedItems();
    for (int i = 0; i < list.count(); i++)
    {
        if (item->checkState() == Qt::Checked)
        {
            list[i]->setCheckState(Qt::Checked);
        }
        else
        {
            list[i]->setCheckState(Qt::Unchecked);
        }
        list[i]->setSelected(false);
    }
    emit itemChanged();
}

void QMultiComboBox::initStyleOption(QStyleOptionComboBox *option) const
{
    //Initializes the state, direction, rect, palette, and fontMetrics member variables based on the specified widget.
    //This is a convenience function; the member variables can also be initialized manually.
    option->initFrom(this);

}



int QMultiComboBox::findText ( const QString & text, Qt::MatchFlags flags){

	for(int i =0; i < vlist_.count(); i++) {

		QListWidgetItem* item = vlist_.item(i);

		if(item->text() == text){
			return i;
		}

	}
	
	return -1; //not incollection
}