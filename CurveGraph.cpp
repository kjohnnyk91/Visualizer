#include "CurveGraph.hpp"

namespace ModelView
{
	CurveGraph::CurveGraph():
		QwtPlotCurve(),
		graph_(NULL),
		pen_(NULL),
		symbol_(NULL),
		serie_(UNDEFINED_SERIE)
	{
		try
		{
			graph_ = new Graph;
			pen_ = new QPen;
			symbol_ = new QwtSymbol;
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "CurveGraph::CurveGraph() : " << ba.what() << endl;
			
			return;
		}
		
		QwtPlotCurve::setPen(*pen_);
		QwtPlotCurve::setSymbol(symbol_);
	}
	
	CurveGraph::CurveGraph(Graph* graph, QPen* pen, QwtSymbol* symbol, const QString& title):
		QwtPlotCurve(title),
		graph_(graph),
		pen_(pen),
		symbol_(symbol),
		serie_(UNDEFINED_SERIE)
	{
		if(pen != NULL)
		{
			QwtPlotCurve::setPen(*pen_);
		}
		
		if(symbol != NULL)
		{
			QwtPlotCurve::setSymbol(symbol_);
		}
		
		if(graph != NULL)
		{
			setSerie(graph-> serie());
		}
	}
	
	CurveGraph::~CurveGraph()
	{}
	
	void CurveGraph::setSerie(unsigned int serie)
	{
		serie_ = serie;
		
		graph_-> setSerie(serie);
	}
	
	bool CurveGraph::toXml(std::fstream& file)
	{
		if(file.good())
		{
			file << indent <<  "<curvegraph serie=\"" << serie_
											<< "\" style=\"" << styleString(style())
											<< "\">" 
											<< std::endl;
			
			++indent;
			
			if(graph_-> toXml(file))
			{
				file << indent << "<pen width=\"" << pen_-> width()
										<< "\" color=\"" << colorString(pen_-> color())
										<< "\" style=\"" << penStyleString(pen_-> style())
										<< "\" />" << std::endl;
				
				file << indent << "<symbol style=\"" <<symbolStyleString(symbol_-> style())
										<< "\" height=\"" << (symbol_-> size()).height()
										<< "\" width=\"" << (symbol_-> size()).width()
										<< "\" />" << std::endl;
										
				file << --indent << "</curvegraph>" << std::endl;
				
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}