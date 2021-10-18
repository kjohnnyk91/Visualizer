// #include <qwt_plot_scaleitem.h>
#include "GraphView.hpp"
#include "Viewer.hpp"

using namespace std;

namespace ModelView
{
	unsigned int BaseChart::sid_ = WATERFALL_ID;
	
	BaseChart::BaseChart(Viewer* viewer, QWidget* parent):
		QwtPlot(parent),
		id_(INVALID_ID),
		viewer_(viewer),
		title_("Base chart"),
		// graphes_(),
		xStep_(1),
		yStep_(1),
		xm_(-5),
		xM_(5),
		ym_(-5),
		yM_(5),
		xDivision_(1),
		yDivision_(1),
		xQuantity_(None),
		yQuantity_(None),
		xUnit_(No),
		yUnit_(No),
		xMultiple_(unity),
		yMultiple_(unity),
		type_(UndefinedType)
	{
		// cout << "BaseChart::BaseChart(Viewer*, QWidget*)" << endl;
		
		setMinimumSize(200,200);
		
		// Retrieving the plot canvas to set a border radius
		
		QwtPlotCanvas* canvas = new QwtPlotCanvas(this);
			canvas-> setBorderRadius(5);
			setCanvas(canvas);
		
		setTitle(title_.c_str());
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " (" + multipleString(xMultiple_) + unitString(xUnit_) + ")").c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " (" + multipleString(yMultiple_) + unitString(yUnit_) + ")").c_str() );
		
		enableAxis(QwtPlot::xBottom, true);
		enableAxis(QwtPlot::yLeft, true);
		
		QObject::connect(this, SIGNAL(activeChart(BaseChart*)), viewer_, SLOT(setActiveChart(BaseChart*)));
	}
	
	BaseChart::BaseChart(Viewer* viewer, string title, QWidget* parent, double xStep, double yStep, int xm, int xM, int ym, int yM, unsigned int xDivision, unsigned int yDivision):
		QwtPlot(parent),
		id_(INVALID_ID),
		viewer_(viewer),
		title_(title),
		xStep_(xStep),
		yStep_(yStep),
		xm_(xm),
		xM_(xM),
		ym_(ym),
		yM_(yM),
		xDivision_(xDivision),
		yDivision_(yDivision),
		xQuantity_(None),
		yQuantity_(None),
		xUnit_(No),
		yUnit_(No),
		xMultiple_(unity),
		yMultiple_(unity),
		type_(UndefinedType)
	{
		// std::cout << "BaseChart::BaseChart() " << std::endl;
		QwtPlotCanvas* canvas = new QwtPlotCanvas(this);
		canvas-> setBorderRadius(5);
		setCanvas(canvas);
		
		setMinimumSize(200,200);
		
		setTitle(title_.c_str());
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " (" + multipleString(xMultiple_) + unitString(xUnit_) + ")").c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " (" + multipleString(yMultiple_) + unitString(yUnit_) + ")").c_str() );
		
		enableAxis(QwtPlot::xBottom, true);
		enableAxis(QwtPlot::yLeft, true);
		
		QObject::connect(this, SIGNAL(activeChart(BaseChart*)), viewer_, SLOT(setActiveChart(BaseChart*)));
	}
	
	BaseChart::~BaseChart()
	{}
	
	void BaseChart::setXQuantity(Quantity xQuantity)
	{
		xQuantity_ = xQuantity;
		
		updateAxisName();
	}
	
	void BaseChart::setYQuantity(Quantity yQuantity)
	{
		yQuantity_ = yQuantity;
		updateAxisName();
	}
	
	void BaseChart::setXUnit(Unit xUnit)
	{
		xUnit_ = xUnit;
		updateAxisName();
	}
	
	void BaseChart::setYUnit(Unit yUnit)
	{
		yUnit_ = yUnit;
		updateAxisName();
	}
	
	void BaseChart::setXMultiple(Multiple xMultiple)
	{
		xMultiple_ = xMultiple;
		updateAxisName();
	}
	
	void BaseChart::setYMultiple(Multiple yMultiple)
	{
		yMultiple_ = yMultiple;
		updateAxisName();
	}
	
	Quantity BaseChart::xQuantity() const
	{
		return xQuantity_;
	}
	
	Quantity BaseChart::yQuantity() const
	{
		return yQuantity_;
	}
	
	Unit BaseChart::xUnit() const
	{
		return xUnit_;
	}
	
	Unit BaseChart::yUnit() const
	{
		return yUnit_;
	}
	
	Multiple BaseChart::xMultiple() const
	{
		return xMultiple_;
	}
	
	Multiple BaseChart::yMultiple() const
	{
		return yMultiple_;
	}
	
	void BaseChart::updateAxisName()
	{
		setAxisTitle(QwtPlot::xBottom, (quantityString(xQuantity_) + " (" + multipleString(xMultiple_) + unitString(xUnit_) + ")").c_str() );
		setAxisTitle(QwtPlot::yLeft, (quantityString(yQuantity_) + " (" + multipleString(yMultiple_) + unitString(yUnit_) + ")").c_str() );
	}
	
	bool BaseChart::toXml(std::fstream& file)
	{
		if(not file.good())
		{
			return false;
		}
		else
		{
			if(not file.good())
			{
				return false;
			}
			else
			{
				file << ++indent << "<base id=\"" << id_ 
												<< "\" title=\"" << title_ 
												<< "\" xstep=\"" << xStep_ 
												<< "\" ystep=\"" << yStep_ 
												<< "\" xm=\"" << xm_ 
												<< "\" xM=\"" << xM_ 
												<< "\" ym=\"" << ym_ 
												<< "\" yM=\"" << yM_ 
												<< "\" xdiv=\"" << xDivision_
												<< "\" ydiv=\"" << yDivision_
												<< "\" xqty=\"" << quantityString(xQuantity_)
												<< "\" yqty=\"" << quantityString(yQuantity_)
												<< "\" xunit=\"" << unitString(xUnit_)
												<< "\" yunit=\"" << unitString(yUnit_)
												<< "\" xmultiple=\"" << multipleString(xMultiple_)
												<< "\" ymultiple=\"" << multipleString(yMultiple_)
												<< "\" type=\"" << typeString(type_)
												<< "\"/>" 
												<< std::endl;
				
				if(toXml2(file))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			
			return toXml2(file);
		}
	}
	
	void BaseChart::updateData(unsigned int serie)
	{
		(void) serie;
	}
	
	void BaseChart::updateData(unsigned int index, unsigned int serie)
	{
		(void) index;
		(void) serie;
	}
	
	void BaseChart::updateSerie(unsigned int serie)
	{
		(void) serie;
	}
	
	void BaseChart::mousePressEvent(QMouseEvent* event)
	{
		if(event-> button() == Qt::LeftButton)
		{
			cout << "BaseChart::mousePressEvent()" << endl;
			
			emit activeChart(this);
		}
	}
}