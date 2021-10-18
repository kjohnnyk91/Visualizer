#include "WaterfallChart.hpp"
#include <qwt_scale_map.h>
#include <iostream>

namespace ModelView
{
	WaterfallChart::WaterfallChart(Viewer* viewer, std::string title, double timeStep, int rows, int columns, QColor minColor, QColor maxColor, bool custom, QWidget* parent):
		BaseChart(viewer, parent),
		graph_(NULL),
		display_(QwtPlotSpectrogram::ImageMode),
		plot_(NULL),
		datas_(NULL),
		// xInterval_(0, 500, QwtInterval::IncludeBorders),
		// yInterval_(0, timeStep_ * columns_ * (rows_ - 1), QwtInterval::IncludeBorders),
		// zInterval_(min_, max_, QwtInterval::IncludeBorders),
		timeStep_(timeStep),
		rows_(rows),
		columns_(columns),
		min_(0),
		max_(1),
		minColor_(minColor),
		maxColor_(maxColor),
		index_(0)
	{
		if(custom == false)
		{
			setID(WATERFALL_ID);
			setType(WaterfallType);
		}
		else
		{
			setID(++BaseChart::sid_);
			setType(SignalTimeWaterfallType);
		}
		
		setTitle(title);
		
		try
		{
			graph_ = new Graph;
			plot_ = new QwtPlotSpectrogram(QString(title.c_str()));
			datas_ = new QwtMatrixRasterData;
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "WaterfallChart::WaterfallChart() : " << ba.what() << std::endl;
			
			return;
		}
		
		graph_-> setSerie(0);
		graph_-> setXQuantity(Time);
		graph_-> setYQuantity(Time);
		graph_-> setXUnit(second);
		graph_-> setYUnit(second);
		graph_-> setXMultiple(unity);
		graph_-> setYMultiple(unity);
		
		QwtPlot::setTitle(QString(title_.c_str()));
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " (" + multipleString(xMultiple_) + unitString(xUnit_) + ")").c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " (" + multipleString(yMultiple_) + unitString(yUnit_) + ")").c_str() );
		
		QVector<double> sizeMatrixVector(rows_*columns_, -1);
		
		xInterval_ = QwtInterval(0, columns_ * timeStep_, QwtInterval::IncludeBorders);
		yInterval_ = QwtInterval(0, timeStep_ * columns_ * (rows_ - 1), QwtInterval::IncludeBorders);
		zInterval_ = QwtInterval(min_, max_, QwtInterval::IncludeBorders);
		
		datas_-> setInterval(Qt::XAxis, xInterval_);
		datas_-> setInterval(Qt::YAxis, yInterval_);
		datas_-> setInterval(Qt::ZAxis, zInterval_);
		
		datas_-> setValueMatrix(sizeMatrixVector, columns_);
		
		// std::cout << "Rows = " << datas_-> numRows() << std::endl;
		// std::cout << "Columns = " << datas_-> numColumns() << std::endl;
		
		QwtLinearColorMap* colorMap(new QwtLinearColorMap(minColor_, maxColor_));

		plot_-> setColorMap(colorMap);
		plot_-> setData(datas_);
		
		plot_-> attach(this);
		
		// cout << "min_ = " << min_ << endl;
		// cout << "max_ = " << max_ << endl;
		
		// addValue(complex<double>(min_,0));
		// addValue(complex<double>(max_,0));
		
		show();
		
		QObject::connect(graph_, SIGNAL(dataChanged(unsigned int, unsigned int)), this, SLOT(updateData(unsigned int, unsigned int)));
		QObject::connect(graph_, SIGNAL(dataAdded(unsigned int)), this, SLOT(updateData(unsigned int)));
	}

	WaterfallChart::~WaterfallChart()
	{
		cout << "~WaterfallChart" << endl;
		
		if(plot_ != NULL)
			delete plot_;
	}
	
	void WaterfallChart::setGraph(Graph* graph)
	{
		if(graph_ != NULL)
			delete graph_;
		
		graph_ = graph;
		
		QObject::connect(graph_, SIGNAL(dataChanged(unsigned int, unsigned int)), this, SLOT(updateData(unsigned int, unsigned int)));
		QObject::connect(graph_, SIGNAL(dataAdded(unsigned int)), this, SLOT(updateData(unsigned int)));
	}
	
	void WaterfallChart::addData(std::complex<short> cs, unsigned int serie)
	{
		// std::cout << "WaterfallChart::addData()" << std::endl;
		(void) serie;
		std::complex<double> c(static_cast<double>(cs.real()), static_cast<double>(cs.imag()));
		
		graph_-> addData(c);
	}
	
	void WaterfallChart::addData(complex<int> ci, unsigned int serie)
	{
		// std::cout << "WaterfallChart::addData()" << std::endl;
		(void) serie;
		complex<double> c(static_cast<double>(ci.real()), static_cast<double>(ci.imag()));
		
		graph_-> addData(c);
	}
	
	void WaterfallChart::addData(complex<float> cf, unsigned int serie)
	{
		// std::cout << "WaterfallChart::addData()" << std::endl;
		(void) serie;
		complex<double> c(static_cast<complex<double>>(cf));
			
		graph_-> addData(c);
	}

	void WaterfallChart::addData(complex<double> c, unsigned int serie)
	{
		// std::cout << "WaterfallChart::addData()" << std::endl;
		(void) serie;
		
		// std::cout << "index = " << index_ << std::endl;
		
		graph_-> addData(c);
	}
	
	void WaterfallChart::clear()
	{
		QVector<double> sizeMatrixVector(rows_*columns_, -1);
		
		datas_-> setValueMatrix(sizeMatrixVector, columns_);
		
		index_ = 0;
	}
	
	void WaterfallChart::clearData()
	{
		graph_-> clearData();
	}
	
	bool WaterfallChart::createSerie(unsigned int serie)
	{
		(void) serie;
		
		return false;
	}
	
	QwtPlotSpectrogram::DisplayMode WaterfallChart::stringDisplayMode(std::string sdisplay)
	{
		if(sdisplay == "image")
		{
			return QwtPlotSpectrogram::ImageMode;
		}
		
		if(sdisplay == "contour")
		{
			return QwtPlotSpectrogram::ContourMode;
		}
		else
		{
			const std::invalid_argument ia(sdisplay);

			throw ia;
		}
	}
	
	std::string WaterfallChart::displayModeString(QwtPlotSpectrogram::DisplayMode display)
	{
		if(display == QwtPlotSpectrogram::ImageMode)
		{
			return "image";
		}
		else
		{
			return "contour";
		}
	}
	
	bool WaterfallChart::toXml2(std::fstream& file)
	{
		if(not file.good())
		{
			return false;
		}
		else
		{
			file << ++indent << "<waterfall time=\"" << timeStep_ 
											<< "\" rows=\"" << rows_
											<< "\" columns=\"" << columns_
											<< "\" mincolor=\"" << colorString(minColor_)
											<< "\" maxcolor=\"" << colorString(maxColor_)
											<< "\">"
											<< std::endl;
			
			file << ++indent << "<plot display=\"" << displayModeString(display_) 
					<< "\"/>"
					<< std::endl;
					
			if(graph_-> toXml(file))
			{
				
				file << --indent << "</waterfall>" << std::endl;
				
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	void WaterfallChart::updateData(unsigned int serie)
	{
		// std::cout << "WaterfallChart::updateData()" << std::endl;
		
		(void) serie;
		
		complex<double> c(graph_-> lastData());
		
		double s(std::abs(c));
		
		// cout << "The data to add is " << s << endl;
		
		datas_-> setValue(index_ / columns_, index_ %columns_, abs(c));
		
		if(index_ == 0)
		{
			min_ = s;
			max_ = s;
		}
		else
		{
			if((s < min_) or (s > max_))
			{
				if(s < min_)
				{
					cout << "s < min_" << endl;
					min_ = s;
				}
				
				if(s > max_)
				{
					cout << "s > max_" << endl;
					
					max_ = s;
				}
				
				datas_-> setInterval(Qt::ZAxis, QwtInterval(min_, max_, QwtInterval::IncludeBorders));
				
				replot();
			}
		}
		
		index_++;
	}
	
	void WaterfallChart::updateData(unsigned int index, unsigned int serie)
	{
		(void) serie;
		
		if(index < graph_-> size())
		{
			std::complex<double> c(graph_-> data(index));
			
			datas_-> setValue(index / rows_, index %columns_, abs(c));
			
			if((abs(c) < min_) or (abs(c) > max_))
			{
				if(abs(c) < min_)
				{
					min_ = abs(c);
				}
				else
				{
					max_ = abs(c);
				}
				
				datas_-> setInterval(Qt::ZAxis, QwtInterval(min_, max_, QwtInterval::IncludeBorders));
				
				replot();
			}
		}
		else
		{
			std::cerr << "WaterfallChart::updateData() : The index " << index << " is out of range" << std::endl;
		}
		
		// if(serie < graphes_.size())
		// {
			// complex<double> c(graphes_[serie]-> data(index));
			
			// if((abs(c) < min_) or (abs(c) > max_))
			// {
				// if(abs(c) < min_)
				// {
					// cout << "s < min_" << endl;
					// min_ = abs(c);
				// }
				
				// if(abs(c) > max_)
				// {
					// cout << "s > max_" << endl;
					
					// max_ = abs(c);
				// }
				
				// datas_-> setInterval(Qt::ZAxis, QwtInterval(min_, max_, QwtInterval::IncludeBorders));
				
				// replot();
			// }
			
			// cout << "Before : (" << (index_ / rows_) * (columns_ * timeStep_) << ", " << (index_ %columns_)* timeStep_ << endl;
			// cout << datas_-> value((index_ / rows_) * (columns_ * timeStep_), (index_ %columns_)* timeStep_) << endl;
			
			// datas_-> setValue(index_ / rows_, index_ %columns_, abs(c));
			
			// cout << "After :" << endl;
			// cout << datas_-> value(index_ * timeStep_, index_ %columns_) << endl;
		// }
		// else
		// {
			// cerr << "The serie " << serie << " doesn't exist" << endl;
		// }
	}
	
	void WaterfallChart::updateSerie()
	{
		vector<complex<double>> graphDatas(graph_-> datas());
			
		if(graphDatas.size() != 0)
		{
			clear();
			
			for(unsigned int i = 0; i < graphDatas.size(); i++)
				updateData(i);
		}
	}
}