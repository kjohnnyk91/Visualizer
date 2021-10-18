#include "SeriesWidget.hpp"

namespace ModelView
{
	SeriesWidget::SeriesWidget(QWidget* parent):
		QWidget(parent),
		series_(),
		serieSpin_(NULL),
		signalSpin_(NULL),
		addSerieButton_(NULL),
		removeSerieButton_(NULL),
		seriesString_(),
		seriesLine_(NULL)
	{
		QHBoxLayout* layout(NULL);
		
		try
		{
			serieSpin_ = new QSpinBox(this);
			signalSpin_ = new QSpinBox(this);
			addSerieButton_ = new QPushButton(QString("+"), this);
			removeSerieButton_ = new QPushButton(QString("-"), this);
			seriesLine_ = new QLineEdit(this);
			layout = new QHBoxLayout(this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "SeriesWidget::SeriesWidget() : " << ba.what() << std::endl;
			
			return;
		}
		
		setLayout(layout);
		
		serieSpin_-> setSingleStep(1);
		serieSpin_-> setRange(0, MAX_SERIE - 1);
		serieSpin_-> setValue(0);
		
		signalSpin_-> setSingleStep(1);
		signalSpin_-> setRange(0, MAX_SERIE - 1);
		signalSpin_-> setValue(0);
		
		// seriesLine_-> setReadOnly(true);
		
		layout-> addWidget(serieSpin_);
		layout-> addWidget(signalSpin_);
		layout-> addWidget(addSerieButton_);
		layout-> addWidget(removeSerieButton_);
		layout-> addWidget(seriesLine_);
				
		QObject::connect(addSerieButton_, SIGNAL(clicked()), this, SLOT(addSerie()));
		QObject::connect(removeSerieButton_, SIGNAL(clicked()), this, SLOT(removeSerie()));
	}
	
	SeriesWidget::~SeriesWidget()
	{}
	
	void SeriesWidget::addSerie()
	{
		bool added(false);
		SerieSignal serieSignal(static_cast<unsigned int>(serieSpin_-> value()), static_cast<unsigned int>(signalSpin_-> value()));
		added = series_.addSerie(serieSignal);
		
		if(added == true)
		{
			if(series_.end() != 1)
			{
				seriesString_.append(QString(","));
			}
			
			seriesString_.append('(');
			seriesString_.append(QString::number(serieSpin_-> value()));
			seriesString_.append(',');
			seriesString_.append(QString::number(signalSpin_-> value()));
			seriesString_.append(')');
			
			seriesLine_-> setText(seriesString_);
		}
	}
	
	void SeriesWidget::removeSerie()
	{
		series_.removeSerie();
		
		if(series_.end() == 0)
		{
			seriesString_.clear();
		}
		else
		{
			seriesString_.truncate((seriesString_.size()) - (QString(",(.,.)")).size());
		}
		
		seriesLine_-> setText(seriesString_);
	}
}