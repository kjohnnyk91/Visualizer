#include "WaterfallColorsWidget.hpp"

namespace ModelView
{
	WaterfallColorsWidget::WaterfallColorsWidget(NewDialog* newDialog, QWidget* parent):
		QWidget(parent),
		newDialog_(newDialog),
		minColor_(Qt::white),
		maxColor_(Qt::black),
		minColorDialog_(this),
		maxColorDialog_(this),
		minColorLabel_(this),
		maxColorLabel_(this)
		// minColorPixmap_(20,20),
		// maxColorPixmap_(20,20)
	{
		QPushButton* minButton(NULL);
		QPushButton* maxButton(NULL);
		
		try
		{
			minButton = new QPushButton(QString("Min"), this);
			maxButton = new QPushButton(QString("Max"), this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "WaterfallColorsWidget::WaterfallColorsWidget() : bad allocation" << ba.what() << std::endl;
			
			return;
		}
		
		minColorPixmap_ = QPixmap(minButton-> width(), minButton-> height());
		maxColorPixmap_ = QPixmap(maxButton-> width(), maxButton-> height());
		
		minColorPixmap_.fill(minColor_);
		maxColorPixmap_.fill(maxColor_);
		
		minColorLabel_.setPixmap(minColorPixmap_);
		maxColorLabel_.setPixmap(maxColorPixmap_);
		
		if(layout() == NULL)
		{
			std::cout << "layout() is NULL" << std::endl;
			
			QGridLayout* layout(NULL);
			
			try
			{
				layout = new QGridLayout(this);
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "WaterfallColorsWidget::WaterfallColorsWidget() : bad allocation" << ba.what() << std::endl;
			
				return;
			}
			
			layout-> addWidget(minButton, 0, 0, Qt::AlignCenter);
			layout-> addWidget(maxButton, 0, 1, Qt::AlignCenter);
			layout-> addWidget(&minColorLabel_, 1, 0, Qt::AlignCenter);
			layout-> addWidget(&maxColorLabel_, 1, 1, Qt::AlignCenter);
			
			setLayout(layout);
		}
		
		QObject::connect(minButton, SIGNAL(clicked()), this, SLOT(showMinColorDialog()));
		QObject::connect(maxButton, SIGNAL(clicked()), this, SLOT(showMaxColorDialog()));
		QObject::connect(&minColorDialog_, SIGNAL(colorSelected(const QColor&)), this, SLOT(setMinColor(const QColor&)));
		QObject::connect(&maxColorDialog_, SIGNAL(colorSelected(const QColor&)), this, SLOT(setMaxColor(const QColor&)));
	}
	
	WaterfallColorsWidget::~WaterfallColorsWidget()
	{}
	
	QColor WaterfallColorsWidget::minColor() const
	{
		return minColor_;
	}
	
	QColor WaterfallColorsWidget::maxColor() const
	{
		return maxColor_;
	}
	
	// void WaterfallColorsWidget::setMinColor(const QColor& minColor)
	// {
		// minColor_ = minColor;
	// }
	
	// void WaterfallColorsWidget::setMaxColor(const QColor& maxColor)
	// {
		// maxColor_ = maxColor;
	// }
	
	// void WaterfallColorsWidget::showMinColorDialog()
	// {
		// minColorDialog_.show();
	// }
	
	// void WaterfallColorsWidget::showMaxColorDialog()
	// {
		// maxColorDialog_.show();
	// }
}