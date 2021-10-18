#ifndef HPP_CURVECHART
#define HPP_CURVECHART

#include <fstream>
#include "GraphSeries.hpp"
#include "GraphView.hpp"
#include "CurveGraph.hpp"

namespace ModelView
{
	class CurveChart : public BaseChart
	{
		Q_OBJECT
		
		protected :
			map<unsigned int, CurveGraph*> graphes_;
			
		public :
			CurveChart(Viewer* viewer, QWidget* parent = NULL);
			CurveChart(Viewer*, string title, QWidget* parent = NULL, double xStep = 1, double yStep = 1, int xm = -5, int xM = 5, int ym = -5, int yM = 5, unsigned int xDivision = 1, unsigned int yDivision = 1);
			// CurveChart(Viewer* viewer, string title, double xStep, double yStep, int xm = 0, int xM = 5, int ym = 0, int yM = 5, unsigned int xDivision = 1, unsigned int yDivision = 1, Quantity xQuantity = None, Quantity yQuantity = None, Unit xUnit = No, Unit yUnit = No, Multiple xMultiple = Unity, Multiple yMultiple = Unity, DataHeader::DataType = DataHeader::UndefinedType, QWidget* parent = NULL);
			~CurveChart();
			
			static std::vector<QColor> serieColor_;
			
			CurveGraph* curveGraph(unsigned int id) const;
			inline std::map<unsigned int, CurveGraph*> graphes() const;
			
			void setCurveGraph(unsigned int serie, CurveGraph* curveGraph);
			unsigned int addCurveGraph(CurveGraph* curveGraph);
			
			void addPoint(const QPointF& point, unsigned int serie);
			virtual void addPoint(double x, double y, unsigned int serie);
			
			void clearData();
			void clearPoints();
			
			bool createSerie(unsigned int serie);
			
			void addData(complex<short>, unsigned int);
			void addData(complex<int>, unsigned int);
			void addData(complex<float>, unsigned int);
			void addData(complex<double>, unsigned int);
			
			virtual void setCurveOption(CurveGraph* curveGraph);
			
			// static QwtPlotCurve::CurveStyle stringToStyle(string);
			// static QwtPlotCurve::CurveAttribute stringToAttribute(string);
			// static QwtPlotCurve::LegendAttribute stringToLegend(string);
			// static QwtPlotCurve::PaintAttribute stringToPaint(string);
			// static QwtSymbol::Style stringToSymbol(string);
			// static Qt::GlobalColor stringToColor(string);
			// static Qt::PenStyle stringToPenStyle(string);
			
			bool toXml2(std::fstream& file);
			virtual bool toXml3(std::fstream& file);
			
		public slots:
			void updateXAxis();
			void updateYAxis();
			virtual void updateData(unsigned int, unsigned int) = 0;
			virtual void updateData(unsigned int) = 0;
			virtual void updateSerie(unsigned int) = 0;
	};
	
	inline std::map<unsigned int, CurveGraph*> CurveChart::graphes() const
	{
		return graphes_;
	}
}

#endif