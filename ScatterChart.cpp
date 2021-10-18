#include "ScatterChart.hpp"

namespace ModelView
{
	ScatterChart::ScatterChart(Viewer* viewer, QWidget* parent, bool inst):
		CurveChart(viewer, parent)
	{
		std::cout << "ScatterChart::ScatterChart()" << std::endl;
		
		if(inst == true)
		{
			setID(++BaseChart::sid_);
			setType(ScatterType);
		}
		
		setTitle("Scatter chart");
		xm_ = -5;
		xM_ = 5;
		ym_ = -5;
		yM_ = 5;
		
		// setAxisTitle(QwtPlot::xBottom, );
		
		QwtPlotScaleItem* xAxis;
		QwtPlotScaleItem* yAxis;
		
		xAxis = new QwtPlotScaleItem(QwtScaleDraw::LeftScale, 0.0);
		yAxis = new QwtPlotScaleItem(QwtScaleDraw::BottomScale, 0.0);
		
		xAxis-> attach(this);
		yAxis-> attach(this);
		
		setAxisScale(QwtPlot::yLeft, ym_, yM_, yStep_);
		setAxisScale(QwtPlot::xBottom, xm_, xM_, xStep_);
		
		axisScaleDraw(QwtPlot::xBottom)-> enableComponent(QwtAbstractScaleDraw::Labels, false);
		axisScaleDraw(QwtPlot::xBottom)-> enableComponent(QwtAbstractScaleDraw::Backbone, false);
		axisScaleDraw(QwtPlot::xBottom)-> enableComponent(QwtAbstractScaleDraw::Ticks, false);
		
		axisScaleDraw(QwtPlot::yLeft)-> enableComponent(QwtAbstractScaleDraw::Labels, false);
		axisScaleDraw(QwtPlot::yLeft)-> enableComponent(QwtAbstractScaleDraw::Backbone, false);
		axisScaleDraw(QwtPlot::yLeft)-> enableComponent(QwtAbstractScaleDraw::Ticks, false);
	}
	
	ScatterChart::ScatterChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xm, int xM, int ym, int yM, unsigned int xDivision, unsigned int yDivision, bool inst):
		CurveChart(viewer, title, parent, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision)
	{
		if(inst == true)
		{
			setID(++BaseChart::sid_);
			setType(ScatterType);
		}
		
		QwtPlotScaleItem* xAxis;
		QwtPlotScaleItem* yAxis;
		
		xAxis = new QwtPlotScaleItem(QwtScaleDraw::LeftScale, 0.0);
		yAxis = new QwtPlotScaleItem(QwtScaleDraw::BottomScale, 0.0);
		
		xAxis-> attach(this);
		yAxis-> attach(this);
		
		setAxisScale(QwtPlot::yLeft, ym_, yM_, yStep_);
		setAxisScale(QwtPlot::xBottom, xm_, xM_, xStep_);
		
		axisScaleDraw(QwtPlot::xBottom)-> enableComponent(QwtAbstractScaleDraw::Labels, false);
		axisScaleDraw(QwtPlot::xBottom)-> enableComponent(QwtAbstractScaleDraw::Backbone, false);
		axisScaleDraw(QwtPlot::xBottom)-> enableComponent(QwtAbstractScaleDraw::Ticks, false);
		
		axisScaleDraw(QwtPlot::yLeft)-> enableComponent(QwtAbstractScaleDraw::Labels, false);
		axisScaleDraw(QwtPlot::yLeft)-> enableComponent(QwtAbstractScaleDraw::Backbone, false);
		axisScaleDraw(QwtPlot::yLeft)-> enableComponent(QwtAbstractScaleDraw::Ticks, false);
	}
		
	
	ScatterChart::~ScatterChart()
	{}
	
	void  ScatterChart::addPoint(std::complex<double> c, unsigned int serie)
	{
		CurveChart::addPoint(c.real(), c.imag(), serie);
	}
	
	void ScatterChart::updateData(unsigned int index, unsigned int serie)
	{
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
				std::cerr << "ScatterChart::updateData() : the index " << index << " is out of range" << std::endl;
				
				return;
			}
				
			QPointF point(c.real(), c.imag());
			
			(datas-> data(index)) = point;
		}
		else
		{
			std::cerr << "ScatterChart::updateData() : the serie " << serie << " doesn't exist" << std::endl;
		}
	}
	
	void ScatterChart::updateData(unsigned int serie)
	{
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
			cerr << "ScatterChart::updateData() : The serie " << serie << " doesn't exist" << endl;
		}
	}

	void ScatterChart::updateSerie(unsigned int serie)
	{
		std::map<unsigned int, CurveGraph*>::const_iterator it(graphes_.find(serie));
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			
			Graph* graph(curveGraph-> graph());
			ScatterData* datas(curveGraph-> datas());
			std::vector<std::complex<double>> graphDatas(graph-> datas());
			
			datas-> clear();
			graph-> setXMin(std::numeric_limits<double>::lowest());
			graph-> setXMax(std::numeric_limits<double>::max());
			
			for(std::vector<std::complex<double>>::iterator it = graphDatas.begin(); it < graphDatas.end(); it++)
			{
				addPoint(*it, serie);
			}
		}
		else
		{
			cerr << "SerialChart2::updateSerie() : The serie " << serie << " doesn't exist" << endl;
		}
	}
	
	bool ScatterChart::toXml3(std::fstream& file)
	{
		if(file.good())
		{
			file << ++indent << "<curve family=\"scatter\">"
					<< std::endl;
					
			file << ++indent << "<graphes>" << std::endl;
			
			// ++indent;
			
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
	
	IQChart::IQChart(Viewer* viewer, QWidget* parent):
		ScatterChart(viewer, parent)
	{
		setID(IQ_ID);
		setType(IQType);
		setTitle("IQ chart");
		
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str());
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str());
	}
	
	IQChart::IQChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xm, int xM, int ym, int yM, unsigned int xDivision, unsigned int yDivision):
		ScatterChart(viewer, title, parent, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision)
	{
		setID(IQ_ID);
		setType(IQType);
		setTitle("IQ chart");
		
		setXQuantity(Voltage);
		setYQuantity(Voltage);
		setXUnit(Volt);
		setYUnit(Volt);
		setXMultiple(unity);
		setYMultiple(unity);
		
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " in " + multipleString(xMultiple_) + unitString(xUnit_)).c_str());
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " in " + multipleString(yMultiple_) + unitString(yUnit_)).c_str());
	}
	
	IQChart::~IQChart()
	{}
}