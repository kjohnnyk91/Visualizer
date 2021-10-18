#include "SerialChart.hpp"

namespace ModelView
{
	SerialChart::SerialChart(Viewer* viewer, QWidget* parent, bool inst):
		CurveChart(viewer, parent)
	{
		// std::cout << "SerialChart::SerialChart()" << std::endl;
		
		if(inst == true)
		{
			setID(++BaseChart::sid_);
			setType(SerialType);
		}
		
		xm_ = 0;
		xM_ = 10;
		ym_ = 0;
		yM_ = 10;
		
		setAxisScale(QwtPlot::yLeft, ym_, yM_, yStep_);
		setAxisScale(QwtPlot::xBottom, xm_, xM_, xStep_);
	}
	
	SerialChart::SerialChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xm, int xM, int ym, int yM, unsigned int xDivision, unsigned int yDivision, bool inst):
		CurveChart(viewer, title, parent, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision)
	{
		// std::cout << "SerialChart::SerialChart(" << std::boolalpha << inst << ") 2" << std::endl;
		
		if(inst == true)
		{
			setID(++BaseChart::sid_);
			setType(SerialType);
		}
		
		setAxisScale(QwtPlot::yLeft, ym_, yM_, yStep_);
		setAxisScale(QwtPlot::xBottom, xm_, xM_, xStep_);
	}
	
	SerialChart::~SerialChart()
	{}
	
	void SerialChart::addPoint(std::complex<double> c, unsigned int serie)
	{
		CurveGraph* curveGraph(NULL);
		std::map<unsigned int, CurveGraph*>::const_iterator it(graphes_.find(serie));
		
		if(it != graphes_.end())
		{
			curveGraph = it-> second;
			
			Graph* graph(curveGraph-> graph());
			double x;
			
			// x = ((graph-> xMax()) + xStep_);
			
			x = graph-> xMax();
			
			CurveChart::addPoint(x, f(c), serie);
			
			graph-> setXMax(x + xStep_);
		}
	}
	
	void SerialChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "SerialChart::setCurveOption() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
	
	double SerialChart::f(std::complex<double> c)
	{
		return abs(c);
	}
	
	bool SerialChart::toXml3(std::fstream& file)
	{
		if(file.good())
		{
			file << indent << "<curve family=\"serial\" >" << std::endl;
			
			file << ++indent << "<graphes>" << std::endl;
			
			++indent;
			
			for(std::map<unsigned int, CurveGraph*>::iterator it = graphes_.begin(); it != graphes_.end(); it++)
			{	
				if(not ((it-> second)-> toXml(file)))
				{
					return false;
				}
			}
			
			file << --indent << "</graphes>" << std::endl;
			
			file << --indent << "</curve>" << std::endl;
			
			--indent;
			
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void SerialChart::updateData(unsigned int index, unsigned int serie)
	{
		// std::cout << "SerialChart::updateData()" << std::endl;
		
		std::map<unsigned int, CurveGraph*>::const_iterator it(graphes_.find(serie));
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			ScatterData* datas(curveGraph-> datas());
			Graph* graph(curveGraph-> graph());
			std::complex<double> c;
			
			if(index < graph-> size())
			{
				c = (graph-> data(index));
			}
			else
			{
				std::cerr << "SerialChart::updateData() : the index " << index << " is out of range" << std::endl;
				
				return;
			}
				
			QPointF point(index * xStep_, f(c));
			
			(datas-> data(index)) = point;
		}
		else
		{
			std::cerr << "SerialChart::updateData() : The serie " << serie << " doesn't exist" << std::endl;
			
			return;
		}
	}
	
	void SerialChart::updateData(unsigned int serie)
	{
		// std::cout << "SerialChart::updateData()" << std::endl;
		CurveGraph* curveGraph(NULL);
		std::map<unsigned int, CurveGraph*>::const_iterator it(graphes_.find(serie));
		
		if(it != graphes_.end())
		{
			curveGraph = (it-> second);
			Graph* graph(curveGraph-> graph());
			
			std::complex<double> c(graph-> lastData());
			
			addPoint(c, serie);
		}
		else
		{
			cerr << "SerialChart::updateData() : The serie " << serie << " doesn't exist" << endl;
		}
	}
	
	void SerialChart::updateSerie(unsigned int serie)
	{
		std::map<unsigned int, CurveGraph*>::const_iterator it(graphes_.find(serie));
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			// curveGraph = it-> second;
			
			Graph* graph(curveGraph-> graph());
			ScatterData* datas(curveGraph-> datas());
			std::vector<std::complex<double>> graphDatas(graph-> datas());
			
			datas-> clear();
			graph-> setXMin(-xStep_);
			graph-> setXMax(-xStep_);
			
			for(std::vector<std::complex<double>>::iterator it = graphDatas.begin(); it < graphDatas.end(); it++)
			{
				addPoint(*it, serie);
			}
		}
		else
		{
			cerr << "SerialChart::updateSerie() : The serie " << serie << " doesn't exist" << endl;
		}
	}
	
	SignalTimeChart::SignalTimeChart(Viewer* viewer, QWidget* parent):
		SerialChart(viewer, parent)
	{
		// cout << "SignalTimeChart::SignalTimeChart()" << endl;
		setTitle("Signal time chart");
		
		setID(SIGNALTIME_ID);
		setType(SignalTimeType);
		setXQuantity(Time);
		setYQuantity(Power);
		setXUnit(second);
		setYUnit(Watt);
		setXMultiple(unity);
		setYMultiple(unity);
		
		// setStyle(QwtPlotCurve::Sticks);
		
		// Graph* graph(NULL);
		
		// try
		// {
			// graph = graphes_.at(0);
		// }
		// catch(const out_of_range& oor)
		// {
			// cerr << "Out of range error : " << oor.what() << endl;
			
			// return;
		// }
		
		setXQuantity(Time);
		setYQuantity(Power);
		setXUnit(second);
		setYUnit(Watt);
		setXMultiple(unity);
		setYMultiple(unity);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::xBottom, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	SignalTimeChart::SignalTimeChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xM, int yM, unsigned int xDivision, unsigned int yDivision):
		SerialChart(viewer, title, parent, xStep, yStep, 0, xM, 0, yM, xDivision, yDivision)
	{
		// cout << "SignalTimeChart::SignalTimeChart() 2" << endl;
		// setTitle("Signal time chart");
		
		setID(SIGNALTIME_ID);
		setType(SignalTimeType);
		setXQuantity(Time);
		setYQuantity(Power);
		setXUnit(second);
		setYUnit(Watt);
		setXMultiple(unity);
		setYMultiple(unity);
		
		// std::cout << quantityString(xQuantity_) << std::endl;
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	SignalTimeChart::~SignalTimeChart()
	{}
	
	void SignalTimeChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "SignalTimeChart::setCurveOption() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
	
	SpectrumChart::SpectrumChart(Viewer* viewer, QWidget* parent):
		SerialChart(viewer, parent)
	{
		setTitle("Spectrum chart");
		
		setID(SPECTRUM_ID);
		setType(SpectrumType);
		// setStyle(QwtPlotCurve::Sticks);
		
		setXQuantity(Frequency);
		setYQuantity(Voltage);
		setXUnit(Hertz);
		setYUnit(Volt);
		setXMultiple(unity);
		setYMultiple(unity);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	SpectrumChart::SpectrumChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xm, int xM, int ym, int yM, unsigned int xDivision, unsigned int yDivision):
		SerialChart(viewer, title, parent, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision)
	{
		// setTitle("Spectrum chart");
		setID(SPECTRUM_ID);
		setType(SpectrumType);
		
		setXQuantity(Frequency);
		setYQuantity(Voltage);
		setXUnit(Hertz);
		setYUnit(Volt);
		setXMultiple(unity);
		setYMultiple(unity);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	SpectrumChart::~SpectrumChart()
	{}
	
	void SpectrumChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "SpectrumChart::setCurveOption() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
	
	LikelihoodChart::LikelihoodChart(Viewer* viewer, QWidget* parent):
		SerialChart(viewer, parent)
	{
		setTitle("Likelihood ratio chart");
		setID(LIKELIHOOD_ID);
		setType(LikelihoodType);
		
		// setStyle(QwtPlotCurve::Sticks);

		setXQuantity(Time);
		setYQuantity(Likelihood);
		setXUnit(second);
		setYUnit(No);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	LikelihoodChart::LikelihoodChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xM, int yM, unsigned int xDivision, unsigned int yDivision):
		SerialChart(viewer, title, parent, xStep, yStep, 0, xM, 0, yM, xDivision, yDivision)
	{
		// setTitle("Likelihood ratio chart");
		setID(LIKELIHOOD_ID);
		setType(LikelihoodType);
		
		setXQuantity(Time);
		setYQuantity(Likelihood);
		setXUnit(second);
		setYUnit(No);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	LikelihoodChart::~LikelihoodChart()
	{}
	
	void LikelihoodChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "LikelihoodChart::setGraphParam() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
	
	FrequencyResponseChart::FrequencyResponseChart(Viewer* viewer, QWidget* parent):
		SerialChart(viewer, parent)
	{
		setTitle("Frequency response chart");
		setID(FREQUENCYRESPONSE_ID);
		setType(FrequencyResponseType);
		
		// setStyle(QwtPlotCurve::Sticks);
		
		setXQuantity(Frequency);
		setYQuantity(Gain);
		setXUnit(Hertz);
		setYUnit(decibel);
		
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	FrequencyResponseChart::FrequencyResponseChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xM, int yM, unsigned int xDivision, unsigned int yDivision):
		SerialChart(viewer, title, parent, xStep, yStep, 0, xM, 0, yM, xDivision, yDivision)
	{
		setID(FREQUENCYRESPONSE_ID);
		setType(FrequencyResponseType);
		
		setXQuantity(Frequency);
		setYQuantity(Gain);
		setXUnit(Hertz);
		setYUnit(decibel);
		
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
		
		// setStyle(QwtPlotCurve::Sticks);
	}
	
	FrequencyResponseChart::~FrequencyResponseChart()
	{}
	
	void FrequencyResponseChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "FrequencyResponseChart::setGraphParam() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
	
	ImpulseResponseChart::ImpulseResponseChart(Viewer* viewer, QWidget* parent):
		SerialChart(viewer, parent)
	{
		setTitle("Impulse response chart");
		setID(IMPULSERESPONSE_ID);
		setType(ImpulseResponseType);
		
		setXQuantity(Frequency);
		setYQuantity(Gain);
		setXUnit(Hertz);
		setYUnit(decibel);
		
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	ImpulseResponseChart::ImpulseResponseChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xM, int yM, unsigned int xDivision, unsigned int yDivision):
		SerialChart(viewer, title, parent, xStep, yStep, 0, xM, 0, yM, xDivision, yDivision)
	{
		setID(IMPULSERESPONSE_ID);
		setType(ImpulseResponseType);
		
		setXQuantity(Frequency);
		setYQuantity(Gain);
		setXUnit(Hertz);
		setYUnit(decibel);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	ImpulseResponseChart::~ImpulseResponseChart()
	{}
	
	void ImpulseResponseChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "ImpulseResponseChart::setGraphParam() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
	
	DataRateChart::DataRateChart(Viewer* viewer, QWidget* parent):
		SerialChart(viewer, parent)
	{
		// setTitle("Data rate chart");
		setID(DATARATE_ID);
		setType(DataRateType);
		// setStyle(QwtPlotCurve::Sticks);
		
		setXQuantity(Time);
		setYQuantity(DataRate);
		setXUnit(second);
		setYUnit(bytesPerSecond);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	DataRateChart::DataRateChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xM, int yM, unsigned int xDivision, unsigned int yDivision):
		SerialChart(viewer, title, parent, xStep, yStep, 0, xM, 0, yM, xDivision, yDivision)
	{
		setID(DATARATE_ID);
		setType(DataRateType);
		
		setXQuantity(Time);
		setYQuantity(DataRate);
		setXUnit(second);
		setYUnit(bytesPerSecond);
		
		// setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str() );
		// setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str() );
	}
	
	DataRateChart::~DataRateChart()
	{}
	
	void DataRateChart::setCurveOption(CurveGraph* curveGraph)
	{
		QwtSymbol* symbol(NULL);
		
		try
		{
			symbol = new QwtSymbol;
		}
		catch(const std::bad_alloc&ba)
		{
			std::cerr << "DataRateChart::setGraphParam() : " << ba.what() << std::endl;
			
			return;
		}
		
		curveGraph-> setStyle(QwtPlotCurve::Lines);
		curveGraph-> setSymbol(symbol);
	}
}