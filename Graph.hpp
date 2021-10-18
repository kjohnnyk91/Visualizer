#ifndef HPP_MODEL_GRAPH
#define HPP_MODEL_GRAPH

#include <iostream>
#include <fstream>
#include <complex>
#include <map>
#include <vector>
#include <string>
#include <QtCharts/QChartView>
#include <QColor>
#include "GraphView.hpp"
#include "TypeHeader.hpp"
#include "Indentation.hpp"

#define SERIE_UNDEFINED 5000

// using namespace std;

namespace ModelView
{
	class BaseChart;
	
	/*
		The class Graph contains the numeric data of the signal
	*/
	
	class Graph : public QObject
	{
		Q_OBJECT
		
		private:
			std::vector<std::complex<double>> datas_;	// Data value
			Quantity xQuantity_;	// Horizontal axis physical quantity 
			Quantity yQuantity_;	// Vertical axis physical quantity
			Unit xUnit_;	// Horizontal axis unit
			Unit yUnit_;	// Vertical axis unit
			Multiple xMultiple_;	// Horizontal axis magnitude order
			Multiple yMultiple_;	// Vertical axis magnitude order
			BaseChart* chart_;	// Pointer to the Graph's chart
			double xMin_;	// Minimal horizontal coordinate of the data
			double xMax_;	// Maximal horizontal coordinate of the data
			double yMin_;	// Minimal vertical coordinate of the data
			double yMax_;	// Minimal horizontal coordinate of the data
			unsigned int serie_;	// Serial number of the Graph
			
		public:
			Graph();
			Graph(const Graph&);
			Graph(const Graph*);
			Graph(Quantity xQuantity, Quantity yQuantity, 
						Unit xUnit, Unit yUnit, Multiple xMultiple, 
						Multiple yMultiple, unsigned int serie);
			Graph(std::vector<std::complex<double>>, Quantity, Quantity, Unit, Unit, Multiple, Multiple, unsigned int = 0, BaseChart* = NULL);
			~Graph();
			
			// Attribute modifiers
			
			void setData(unsigned int, std::complex<double> data);
			void setDatas(std::vector<std::complex<double>> datas);
			void setDatas(unsigned int, std::vector<std::complex<double>> datas);
			void setDatas(std::vector<unsigned int>, std::vector<std::complex<double>> datas);
			inline void setXQuantity(Quantity xQuantity);
			inline void setYQuantity(Quantity yQuantity);
			inline void setXUnit(Unit xUnit);
			inline void setYUnit(Unit yUnit);
			inline void setXMultiple(Multiple xMultiple);
			inline void setYMultiple(Multiple yMultiple);
			inline void setChart(BaseChart* chart);
			inline void setXMin(double xMin);
			inline void setXMax(double xMax);
			inline void setYMin(double yMin);
			inline void setYMax(double yMax);
			inline void setSerie(unsigned int serie);
			
			// Attributes readers
			
			inline std::complex<double> data(unsigned int) const;
			std::vector<std::complex<double>> datas() const;
			std::vector<std::complex<double>> datas(unsigned int, unsigned int) const;
			std::vector<std::complex<double>> datas(std::vector<unsigned int>) const;
			inline Unit xUnit() const;
			inline Unit yUnit() const;
			inline Multiple xMultiple() const;
			inline Multiple yMultiple() const;
			inline Quantity xQuantity() const;
			inline Quantity yQuantity() const;
			inline double xMin() const;
			inline double xMax() const;
			inline double yMin() const;
			inline double yMax() const;
			inline unsigned int serie() const;
			
			// String attribute converter
			
			inline std::string xUnitString() const;
			inline std::string yUnitString() const;
			inline std::string xMultipleString() const;
			inline std::string yMultipleString() const;
			inline std::string xQuantityString() const;
			inline std::string yQuantityString() const;
			
			inline std::string xUnitSymbol() const;
			inline std::string yUnitSymbol() const;
			inline std::string xMultipleSymbol() const;
			inline std::string yMultipleSymbol() const;
			inline unsigned int size() const;
			
			inline bool empty() const;
			inline std::complex<double> lastData() const;
			
			inline std::complex<double> addData(std::complex<double> data);
			std::vector<std::complex<double>> addDatas(std::vector<std::complex<double>> datas);
			inline void clearData();
			
			bool toXml(std::fstream& file);
			
		signals:
			void xQuantityChanged(unsigned int);
			void yQuantityChanged(unsigned int);
			void xUnitChanged(unsigned int);
			void yUnitChanged(unsigned int);
			void xMultipleChanged(unsigned int);
			void yMultipleChanged(unsigned int);
			void dataChanged(unsigned int, unsigned int);
			void dataAdded(unsigned int);
	};
	
	inline void Graph::setXQuantity(Quantity xQuantity)
	{
		xQuantity_ = xQuantity;
		
		emit xQuantityChanged(serie_);
	}

	inline void Graph::setYQuantity(Quantity yQuantity)
	{
		yQuantity_ = yQuantity;
		
		emit yQuantityChanged(serie_);
	}
	
	inline void Graph::setXUnit(Unit xUnit)
	{
		xUnit_ = xUnit;
		
		emit xUnitChanged(serie_);
	}
	
	inline void Graph::setYUnit(Unit yUnit)
	{
		yUnit_ = yUnit;
		
		emit yUnitChanged(serie_);
	}
	
	inline void Graph::setXMultiple(Multiple xMultiple)
	{
		xMultiple_ = xMultiple;
		
		emit xMultipleChanged(serie_);
	}
	
	inline void Graph::setYMultiple(Multiple yMultiple)
	{
		yMultiple_ = yMultiple;
		
		emit yMultipleChanged(serie_);
	}
	
	inline void Graph::setChart(BaseChart* chart)
	{
		chart_ = chart;
	}
	
	inline void Graph::setXMin(double xMin)
	{
		xMin_ = xMin;
	}
	
	inline void Graph::setXMax(double xMax)
	{
		xMax_ = xMax;
	}
	
	inline void Graph::setYMin(double yMin)
	{
		yMin_ = yMin;
	}
	
	inline void Graph::setYMax(double yMax)
	{
		yMax_ = yMax;
	}
	
	inline void Graph::setSerie(unsigned int serie)
	{
		serie_ =  serie;
	}
	
	inline std::complex<double> Graph::data(unsigned int index) const
	{
		return datas_.at(index);
	}
	
	inline Unit Graph::xUnit() const
	{
		return xUnit_;
	}
	
	inline Unit Graph::yUnit() const
	{
		return yUnit_;
	}
	
	inline Multiple Graph::xMultiple() const
	{
		return xMultiple_;
	}
	
	inline Multiple Graph::yMultiple() const
	{
		return yMultiple_;
	}
	
	inline Quantity Graph::xQuantity() const
	{
		return xQuantity_;
	}
	
	inline Quantity Graph::yQuantity() const
	{
		return yQuantity_;
	}
	
	inline std::string Graph::xUnitString() const
	{
		return unitString(xUnit_);
	}
	
	inline std::string Graph::yUnitString() const
	{
		return unitString(yUnit_);
	}
	
	inline std::string Graph::xMultipleString() const
	{
		return multipleString(xMultiple_);
	}
	
	inline std::string Graph::yMultipleString() const
	{
		return multipleString(yMultiple_);
	}
	
	inline std::string Graph::xQuantityString() const
	{
		return quantityString(xQuantity_);
	}
	
	inline std::string Graph::yQuantityString() const
	{
		return quantityString(yQuantity_);
	}
	
	inline double Graph::xMin() const
	{
		return xMin_;
	}
	
	inline double Graph::xMax() const
	{
		return xMax_;
	}
	
	inline double Graph::yMin() const
	{
		return yMin_;
	}
	
	inline double Graph::yMax() const
	{
		return yMax_;
	}
	
	inline unsigned int Graph::serie() const
	{
		return serie_;
	}
	
	inline std::string Graph::xUnitSymbol() const
	{
		return unitSymbol(xUnit_);
	}
	
	inline std::string Graph::yUnitSymbol() const
	{
		return unitSymbol(yUnit_);
	}
	
	inline std::string Graph::xMultipleSymbol() const
	{
		return multipleSymbol(xMultiple_);
	}
	
	inline std::string Graph::yMultipleSymbol() const
	{
		return multipleSymbol(yMultiple_);
	}
	
	inline unsigned int Graph::size() const
	{
		return datas_.size();
	}
	
	inline bool Graph::empty() const
	{
		return datas_.empty();
	}
	
	inline std::complex<double> Graph::lastData() const
	{
		return datas_.back();
	}
	
	inline std::complex<double> Graph::addData(std::complex<double> data)
	{
		datas_.push_back(data);
		
		emit dataAdded(serie_);
		
		return datas_.back();
	}
	
	inline void Graph::clearData()
	{
		datas_.clear();
		xMax_ = 0;
	}
}

#endif