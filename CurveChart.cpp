#include "CurveChart.hpp"

namespace ModelView
{
	std::vector<QColor> CurveChart::serieColor_ = 
	{
		QColor(Qt::black), 
		QColor(Qt::red), 
		QColor(Qt::green), 
		QColor(Qt::blue), 
		QColor(Qt::cyan),
		QColor(Qt::magenta),
		QColor(Qt::yellow),
		QColor(Qt::gray),
		QColor(255,115,0),
		QColor(138,43,226),
		QColor(128,0,0)
	};
	
	CurveChart::CurveChart(Viewer* viewer, QWidget* parent):
		BaseChart(viewer, parent),
		graphes_()
	{
		// std::cout << "CurveChart::CurveChart()" << std::endl;
	}
	
	CurveChart::CurveChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xm, int xM, int ym, int yM, unsigned int xDivision, unsigned int yDivision):
		BaseChart(viewer, title, parent, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision),
		graphes_()
	{
		// std::cout << "CurveChart::CurveChart() 2" << std::endl;
	}
	
	// CurveChart(Viewer* viewer, string title, double xStep, double yStep, int xm = 0, int xM = 5, int ym = 0, int yM = 5, unsigned int xDivision = 1, unsigned int yDivision = 1, Quantity xQuantity = None, Quantity yQuantity = None, Unit xUnit = No, Unit yUnit = No, Multiple xMultiple = Unity, Multiple yMultiple = Unity, DataType = UndefinedType, QWidget* parent = NULL):
		// BaseChart()
		
	CurveChart::~CurveChart()
	{}
	
	CurveGraph* CurveChart::curveGraph(unsigned int serie) const
	{
		CurveGraph* ret(NULL);
		std::map<unsigned int, CurveGraph*>::const_iterator it;
		
		it = graphes_.find(serie);
		
		if(it != graphes_.end())
		{
			ret = it-> second;
		}
		
		return ret;
	}
	
	void CurveChart::setCurveGraph(unsigned int serie, CurveGraph* curveGraph)
	{
		if(curveGraph != NULL)
		{
			Graph* graph(curveGraph-> graph());
			graphes_[serie] = curveGraph;
			
			curveGraph-> setSerie(serie);
			
			curveGraph-> attach(this);
			
			switch(type_)
			{
				case UndefinedType :
				{
					std::cerr << "CurveChart::addCurveGraph() : The type is undefined" << std::endl;
					
					break;
				}
				
				case ScatterType :
				case IQType :
				{
					curveGraph-> setWidth(5);
					curveGraph-> setStyle(QwtPlotCurve::Dots);
					
					break;
				}
				
				default :
				{
					curveGraph-> setWidth(1);
					curveGraph-> setStyle(QwtPlotCurve::Lines);
					
					break;
				}
			}
			
			// if((type_ == IQType) or (type_ == ScatterType))
			// {
				// curveGraph-> setWidth(5);
				// curveGraph-> setStyle(QwtPlotCurve::Dots);
			// }
			
			QObject::connect(graph, SIGNAL(dataAdded(unsigned int)), this, SLOT(updateData(unsigned int)));
		}
	}
	
	unsigned int CurveChart::addCurveGraph(CurveGraph* curveGraph)
	{
		if(curveGraph != NULL)
		{
			unsigned int serie(0);
			Graph* graph(curveGraph-> graph());
			
			switch(type_)
			{
				case UndefinedType :
				{
					std::cerr << "CurveChart::addCurveGraph() : The type is undefined" << std::endl;
					
					return UNDEFINED_SERIE;
					
					break;
				}
				
				case ScatterType :
				case IQType :
				{
					curveGraph-> setWidth(5);
					curveGraph-> setStyle(QwtPlotCurve::Dots);
					
					break;
				}
				
				default :
				{
					curveGraph-> setStyle(QwtPlotCurve::Lines);
					
					break;
				}
			}
			
			while(graphes_.find(serie) != graphes_.end())
			{
				serie++;
			}
			
			// curveGraph-> setStyle(QwtPlotCurve::Lines);
			curveGraph-> setSerie(serie);
			
			if((type_ == IQType) or (type_ == ScatterType))
			{
				curveGraph-> setWidth(5);
				curveGraph-> setStyle(QwtPlotCurve::Dots);
			}
			
			graphes_[serie] = curveGraph;
			
			curveGraph-> attach(this);
			
			QObject::connect(graph, SIGNAL(dataAdded(unsigned int)), this, SLOT(updateData(unsigned int)));
			
			return serie;
		}
		else
		{
			return UNDEFINED_SERIE;
		}
	}
	
	void CurveChart::addPoint(const QPointF& point, unsigned int serie)
	{
		// std::cout << "CurveChart::addPoint() " << std::endl;
		
		std::map<unsigned int, CurveGraph*>::const_iterator it;
		it = graphes_.find(serie);
		
		CurveGraph* curveGraph(NULL);
		
		if(it != graphes_.end())
		{
			curveGraph = it-> second;
			
			Graph* graph(curveGraph-> graph());
			
			ScatterData* data(static_cast<ScatterData*>(curveGraph-> data()));
			
			data-> append(point);
			
			if(data-> size() == 1)
			{
				graph-> setXMin(point.x());
				graph-> setXMax(point.x());
				graph-> setYMin(point.y());
				graph-> setYMax(point.y());
			}
			else
			{
				if(point.x() < (graph-> xMin()))
				{
					graph-> setXMin(point.x());
				}
				
				if(point.x() > (graph-> xMax()))
				{
					graph-> setXMax(point.x());
				}
				
				if(point.y() < (graph-> yMin()))
				{
					graph-> setYMin(point.y());
				}
				
				if(point.y() > (graph-> yMax()))
				{
					graph-> setYMax(point.y());
				}
				
				if((graph-> xMin()) < xm_)
				{
					xm_ = (graph-> xMin()) - xStep_;
					
					updateXAxis();
				}
				
				if((graph-> xMax()) > xM_)
				{
					xM_ = (graph-> xMax()) + xStep_;
					
					updateXAxis();
				}
				
				if((graph-> yMin()) < ym_)
				{
					ym_ = (graph-> yMin()) - yStep_;
					
					updateYAxis();
				}
				
				if((graph-> yMax()) > yM_)
				{
					yM_ = (graph-> yMax()) + yStep_;
					
					updateYAxis();
				}
			}
		}
		else
		{
			std::cerr << "CurveChart::addPoint() : The serie " << serie << " doesn't exist" << std::endl;
			
			std::cout << "CurveChart::addPoint() : Creating the serie" << std::endl;
			
			createSerie(serie);
			
			addPoint(point, serie);
		}
	}
	
	void CurveChart::addPoint(double x, double y, unsigned int serie)
	{
		const QPointF point(x,y);
		
		addPoint(point, serie);
	}
	
	void CurveChart::clearData()
	{
		for(std::map<unsigned int, CurveGraph*>::iterator it = graphes_.begin(); it != graphes_.end(); it++)
		{
			Graph* graph((it-> second)-> graph());
			
			graph-> clearData();
		}
	}
	
	void CurveChart::clearPoints()
	{
		for(std::map<unsigned int, CurveGraph*>::iterator it = graphes_.begin(); it != graphes_.end(); it++)
		{
			ScatterData* datas(static_cast<ScatterData*>((it-> second)-> data()));
			datas-> clear();			
		}
	}
	
	bool CurveChart::createSerie(unsigned int serie)
	{
		cout << "CurveChart::createSerie(" << serie << ')' << endl;
		std::map<unsigned int, CurveGraph*>::iterator it(graphes_.find(serie));
		
		if(it != graphes_.end())
		{
			std::cerr << "The serie " << serie << " already exist" << std::endl;
			
			return false;
		}
		else
		{
			CurveGraph* curveGraph(NULL);
			Graph* graph(NULL);
			QPen* pen(NULL);
			QwtSymbol* symbol(NULL);
			
			try
			{
				graph = new Graph;
				pen = new QPen(CurveChart::serieColor_[serie%(CurveChart::serieColor_.size())]);
				
				if(type_ == IQType)
				{
					// symbol = new QwtSymbol(QwtSymbol::XCross);
					// symbol-> setSize(5,5);
					
					// curveGraph-> setStyle(QwtPlotCurve::Dots);
				}
				else
				{
					symbol = new QwtSymbol(QwtSymbol::NoSymbol);
				}
			}
			catch(const bad_alloc& ba)
			{
				cerr << "CurveChart::createSerie() : " << ba.what() << endl;
				
				return false;
			}
			catch(...)
			{
				cerr << "CurveChart::createSerie() : An unknown exception was thrown" << endl;
				
				return false;
			}
			
			graph-> setXQuantity(xQuantity_);
			graph-> setYQuantity(yQuantity_);
			graph-> setXUnit(xUnit_);
			graph-> setYUnit(yUnit_);
			graph-> setXMultiple(xMultiple_);
			graph-> setYMultiple(yMultiple_);
			graph-> setSerie(serie);
			
			try
			{
				curveGraph = new CurveGraph(graph, pen, symbol, QString("Untitled"));
			}
			catch(const bad_alloc& ba)
			{
				cerr << "BaseChart::createSerie() : " << ba.what() << endl;
				
				return false;
			}
			catch(...)
			{
				cerr << "BaseChart::createSerie() : An unknown exception was thrown" << endl;
				
				return false;
			}
			
			if(type_ == IQType)
			{
				pen-> setWidth(3);
				curveGraph-> setStyle(QwtPlotCurve::Dots);
			}
			
			setCurveGraph(serie, curveGraph);
			
			return true;
		}
	}
	
	void CurveChart::addData(std::complex<short> cs, unsigned int serie)
	{
		std::map<unsigned int, CurveGraph*>::iterator it;
		it = graphes_.find(serie);
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			Graph* graph(curveGraph-> graph());
			std::complex<double> c(static_cast<double>(cs.real()), static_cast<double>(cs.imag()));
			
			graph-> addData(c);
		}
		else
		{
			std::cerr << "The serie " << serie << " hasn't been found" << std::endl;
		}
	}
	
	void CurveChart::addData(std::complex<int> ci, unsigned int serie)
	{
		std::map<unsigned int, CurveGraph*>::iterator it;
		it = graphes_.find(serie);
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			Graph* graph(curveGraph-> graph());
			std::complex<double> c(static_cast<double>(ci.real()), static_cast<double>(ci.imag()));
			
			graph-> addData(c);
		}
		else
		{
			std::cerr << "The serie " << serie << " hasn't been found" << std::endl;
		}
	}
	
	void CurveChart::addData(std::complex<float> cf, unsigned int serie)
	{
		std::map<unsigned int, CurveGraph*>::iterator it;
		it = graphes_.find(serie);
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			Graph* graph(curveGraph-> graph());
			std::complex<double> c(static_cast<double>(cf.real()), static_cast<double>(cf.imag()));
			
			graph-> addData(c);
		}
		else
		{
			std::cerr << "The serie " << serie << " hasn't been found" << std::endl;
		}
	}
	
	void CurveChart::addData(std::complex<double> c, unsigned int serie)
	{
		std::map<unsigned int, CurveGraph*>::iterator it;
		it = graphes_.find(serie);
		
		if(it != graphes_.end())
		{
			CurveGraph* curveGraph(it-> second);
			Graph* graph(curveGraph-> graph());
			
			graph-> addData(c);
		}
		else
		{
			std::cerr << "The serie " << serie << " hasn't been found" << std::endl;
		}
	}
	
	void CurveChart::setCurveOption(CurveGraph* curveGraph)
	{
		curveGraph-> setStyle(QwtPlotCurve::Lines);
	}
	
	bool CurveChart::toXml2(std::fstream& file)
	{
		if(not file.good())
		{
			return false;
		}
		else
		{
			if(toXml3(file))
			{
				// file << --indent << "</curve>" << std::endl;
				
				return true;
			}
			else
			{
				return false;
			}
			
			return false;
		}
	}
	
	bool CurveChart::toXml3(std::fstream& file)
	{
		return true;
	}
	
	void CurveChart::updateXAxis()
	{
		double tmp(xStep_);
		int nbDiv((xM_ - xm_) / tmp);
		double widthDivRatio(static_cast<int>(width()/nbDiv));
		
		while(widthDivRatio < 70)					// Tant que l'espacement entre deux divisions est plus petit que 70 pixels
		{
			if(tmp == xStep_)
			{
				tmp += 4*xStep_;
			}
			else
			{
				tmp += 5*xStep_;
			}
			
			nbDiv = static_cast<int>((xM_ - xm_) / tmp);
			widthDivRatio = static_cast<int>(width()/nbDiv);
		}
		
		setAxisScale(QwtPlot::xBottom, xm_, xM_, tmp);
	}
	
	void CurveChart::updateYAxis()
	{
		double tmp(yStep_);
		int nbDiv((yM_ - ym_) / tmp);
		double widthDivRatio(static_cast<int>(width()/nbDiv));
		// cout << "nbDiv = " << nbDiv << endl;
		// cout << "widthDivRatio = " << widthDivRatio << endl;
		
		while(widthDivRatio < 70)
		{
			if(tmp == yStep_)
			{
				tmp += 4*yStep_;
			}
			else
			{
				tmp += 5*yStep_;
			}
			
			nbDiv = static_cast<int>((yM_ - ym_) / tmp);
			widthDivRatio = static_cast<int>(width()/nbDiv);
		}
		
		setAxisScale(QwtPlot::yLeft, ym_, yM_, tmp);
	}
}