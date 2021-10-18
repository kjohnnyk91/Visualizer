#ifndef HPP_CURVEGRAPH
#define HPP_CURVEGRAPH

// #define UNDEFINED_SERIE 99

#include <fstream>
#include "Graph.hpp"
#include "GraphSeries.hpp"
#include "qwt_plot_curve.h"
#include "Indentation.hpp"
#include "XmlTools.hpp"

namespace ModelView
{
	class CurveGraph : public QwtPlotCurve
	{
		private :
			Graph* graph_;
			QPen* pen_;
			QwtSymbol* symbol_;
			unsigned int serie_;
			
			// static std::vector<QColor> serieColor_;
			
		public :
			CurveGraph();
			CurveGraph(Graph* graph, QPen* pen, QwtSymbol* symbol, const QString& title = QString());
			~CurveGraph();
			
			inline Graph* graph() const;
			inline QPen* pen() const;
			inline QwtSymbol* symbol() const;
			inline unsigned int serie() const;
			
			inline void setGraph(Graph* graph);
			inline void setPen(QPen* pen);
			inline void setSymbol(QwtSymbol* symbol);
			void setSerie(unsigned int serie);
			
			inline void setWidth(unsigned int width);
			
			inline ScatterData* datas();
			inline void clearDatas();
			
			bool toXml(std::fstream& file);
	};
	
	inline Graph* CurveGraph::graph() const
	{
		return graph_;
	}
	
	inline QPen* CurveGraph::pen() const
	{
		return pen_;
	}
	
	inline QwtSymbol* CurveGraph::symbol() const
	{
		return symbol_;
	}
	
	inline unsigned int CurveGraph::serie() const
	{
		return serie_;
	}
	
	inline void CurveGraph::setGraph(Graph* graph)
	{
		graph_ = graph;
	}
	
	inline void CurveGraph::setPen(QPen* pen)
	{
		pen_ = pen;
		QwtPlotCurve::setPen(*pen_);
		
		// std::cout << "CurveGraph::setPen() : The size of the pen is " << pen-> width() << std::endl;
	}
	
	inline void CurveGraph::setSymbol(QwtSymbol* symbol)
	{
		symbol_ = symbol;
		QwtPlotCurve::setSymbol(symbol_);
	}
	
	inline void CurveGraph::setWidth(unsigned int width)
	{
		pen_-> setWidth(width);
		QwtPlotCurve::setPen(*pen_);
	}
	
	inline ScatterData* CurveGraph::datas()
	{
		return static_cast<ScatterData*>(data());
	}
	
	inline void CurveGraph::clearDatas()
	{
		ScatterData* datas(static_cast<ScatterData*>(data()));
		
		graph_-> clearData();
		datas-> clear();
	}
}

#endif