#ifndef HPP_BASECHART
#define HPP_BASECHART

#include <QtWidgets/QWidget>
#include <QString>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_symbol.h>
#include "qwt_plot_canvas.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_barchart.h"
#include "qwt_column_symbol.h"
#include "Graph.hpp"
#include "ScatterData.hpp"
#include "DataHeader.hpp"
#include "TypeHeader.hpp"
#include "Indentation.hpp"

using namespace std;

namespace ModelView
{
	class Graph;
	class Viewer;
	
	/*
		The class BaseChart is the base class of all charts
		It's an pure abstract class since no data placing method has been defined yet
	*/
	
	class BaseChart : public QwtPlot
	{
		Q_OBJECT
		
		protected:
			unsigned int id_;
			Viewer* viewer_;
			string title_;
			double xStep_;
			double yStep_;
			int xm_;
			int xM_;
			int ym_;
			int yM_;
			unsigned int xDivision_;
			unsigned int yDivision_;
			
			Quantity xQuantity_;
			Quantity yQuantity_;
			Unit xUnit_;
			Unit yUnit_;
			Multiple xMultiple_;
			Multiple yMultiple_;
			
			DataType type_;
			
			static unsigned int sid_;
			
		public:
			BaseChart(Viewer*, QWidget* = NULL);
			BaseChart(Viewer*, string title, QWidget* parent = NULL, double xStep = 1, double yStep = 1, int xm = -5, int xM = 5, int ym = -5, int yM = 5, unsigned int xDivision = 1, unsigned int yDivision = 1);
			virtual ~BaseChart();
			
			inline void setID(unsigned int);
			inline void setViewer(Viewer*);
			inline void setTitle(string);
			inline void setXStep(double);
			inline void setYStep(double);
			inline void setXm(int);
			inline void setXM(int);
			inline void setYm(int);
			inline void setYM(int);
			inline void setXDivision(unsigned int);
			inline void setYDivision(unsigned int);
			void setXQuantity(Quantity);
			void setYQuantity(Quantity);
			void setXUnit(Unit);
			void setYUnit(Unit);
			void setXMultiple(Multiple);
			void setYMultiple(Multiple);
			inline void setType(DataType);
			
			inline unsigned int id() const;
			inline Viewer* viewer() const;
			inline string title() const;
			inline double xStep() const;
			inline double yStep() const;
			inline int xm() const;
			inline int xM() const;
			inline int ym() const;
			inline int yM() const;
			inline unsigned int xDivision() const;
			inline unsigned int yDivision() const;
			Quantity xQuantity() const;
			Quantity yQuantity() const;
			Unit xUnit() const;
			Unit yUnit() const;
			Multiple xMultiple() const;
			Multiple yMultiple() const;
			DataType type() const;
			
			inline double xScale() const;
			inline double yScale() const;
			
			virtual void clearData() = 0;
			
			virtual bool createSerie(unsigned int serie) = 0;
			
			virtual void addData(complex<short> cs, unsigned int serie) = 0;
			virtual void addData(complex<int> ci, unsigned int serie) = 0;
			virtual void addData(complex<float> cf, unsigned int serie) = 0;
			virtual void addData(complex<double> c, unsigned int serie) = 0;
			
			void updateAxisName();
			
			bool toXml(std::fstream& file);
			virtual bool toXml2(std::fstream& file) = 0;
			// bool toXmlBase(std::fstream& file);
			
		signals:
			void activeChart(BaseChart*);
			
		public slots:
			inline void updateTitle(std::string);
			virtual void updateData(unsigned int) = 0;
			virtual void updateData(unsigned int, unsigned int) = 0;
			virtual void updateSerie(unsigned int);
			
		protected:
			void mousePressEvent(QMouseEvent*);
	};
	
	inline void BaseChart::setID(unsigned int id)
	{
		id_ = id;
	}
	
	inline void BaseChart::setViewer(Viewer* viewer)
	{
		viewer_ = viewer;
	}
	
	inline void BaseChart::setTitle(string title)
	{
		title_ = title;
		
		QString qtitle(title.c_str());
		
		QwtPlot::setTitle(qtitle);
	}
	
	inline void BaseChart::setXStep(double xStep)
	{
		xStep_ = xStep;
	}
	
	inline void BaseChart::setYStep(double yStep)
	{
		yStep_ = yStep;
	}
	
	inline void BaseChart::setXm(int xm)
	{
		xm_ = xm;
	}
	
	inline void BaseChart::setXM(int xM)
	{
		xM_ = xM;
	}
	
	inline void BaseChart::setYm(int ym)
	{
		ym_ = ym;
	}
	
	inline void BaseChart::setYM(int yM)
	{
		yM_ = yM;
	}
	
	inline void BaseChart::setXDivision(unsigned int xDivision)
	{
		xDivision_ = xDivision;
	}
	
	inline void BaseChart::setYDivision(unsigned int yDivision)
	{
		yDivision_ = yDivision;
	}
	
	inline void BaseChart::setType(DataType type)
	{
		type_ = type;
	}
	
	inline unsigned int BaseChart::id() const
	{
		return id_;
	}
	
	inline Viewer* BaseChart::viewer() const
	{
		return viewer_;
	}
	
	inline string BaseChart::title() const
	{
		return title_;
	}
	
	inline double BaseChart::xStep() const
	{
		return xStep_;
	}
	
	inline double BaseChart::yStep() const
	{
		return yStep_;
	}
	
	inline int BaseChart::xm() const
	{
		return xm_;
	}
	
	inline int BaseChart::xM() const
	{
		return xM_;
	}
	
	inline int BaseChart::ym() const
	{
		return ym_;
	}
	
	inline int BaseChart::yM() const
	{
		return yM_;
	}
	
	inline unsigned int BaseChart::xDivision() const
	{
		return xDivision_;
	}
	
	inline unsigned int BaseChart::yDivision() const
	{
		return yDivision_;
	}
	
	inline DataType BaseChart::type() const
	{
		return type_;
	}
	
	inline double BaseChart::xScale() const
	{
		return xStep_ * xDivision_;
	}
	
	inline double BaseChart::yScale() const
	{
		return yStep_ * yDivision_;
	}
	
	inline void BaseChart::updateTitle(std::string title)
	{
		QString qTitle(QString::fromStdString(title));
		
		QwtPlot::setTitle(qTitle);
	}
}

#endif