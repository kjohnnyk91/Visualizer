#ifndef HPP_SCATTERCHART
#define HPP_SCATTERCHART

#include <fstream>
#include <qwt_plot_scaleitem.h>
#include "CurveChart.hpp"
#include "Indentation.hpp"

namespace ModelView
{
	class ScatterChart : public CurveChart
	{
		Q_OBJECT
		
		public:
			ScatterChart(Viewer* viewer, QWidget* parent = NULL, bool inst = false);
			ScatterChart(Viewer*, string title, QWidget* parent = NULL, double xStep = 1, double yStep = 1, int xm = -5, int xM = 5, int ym = -5, int yM = 5, unsigned int xDivision = 1, unsigned int yDivision = 1, bool inst = false);
			~ScatterChart();
			
			// void addData(complex<int>, unsigned int);
			// void addData(complex<float>, unsigned int);
			// void addData(complex<double>, unsigned int);
			void addPoint(complex<double>, unsigned int);
			
			bool toXml3(std::fstream& file);
			
		public slots:
			void updateData(unsigned int, unsigned int);
			void updateData(unsigned int);
			void updateSerie(unsigned int);
	};
	
	class IQChart : public ScatterChart
	{
		public :
			IQChart(Viewer* viewer, QWidget* parent = NULL);
			IQChart(Viewer* viewer, string title, QWidget* parent = NULL, double xStep = 1, double yStep = 1, int xm = -5, int xM = 5, int ym = -5, int yM = 5, unsigned int xDivision = 1, unsigned int yDivision = 1);
			~IQChart();
	};
}

#endif