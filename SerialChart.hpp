#ifndef HPP_SERIALCHART
#define HPP_SERIALCHART

#include <fstream>
#include "GraphView.hpp"
// #include "qwt_plot_canvas.h"
// #include "qwt_plot_magnifier.h"
// #include "qwt_plot_panner.h"
#include "Viewer.hpp"
#include "DataHeader.hpp"
#include "CurveChart.hpp"

using namespace std;

namespace ModelView
{
	// class Viewer;
	
	class SerialChart : public CurveChart
	{
		Q_OBJECT
		
		public:
			SerialChart(Viewer* viewer, QWidget* parent = NULL, bool inst = false);
			SerialChart(Viewer* viewer, string title, QWidget* parent = NULL, double xStep = 1, 
				double yStep = 1, int xm = -5, int xM = 5, int ym = -5, int yM = 5, 
				unsigned int xDivision = 1, unsigned int yDivision = 1, bool inst = false);
			~SerialChart();
			
			virtual void addPoint(complex<double>, unsigned int);
			static double f(complex<double>);
			
			bool toXml3(std::fstream& file);
		
		private:
			virtual void setCurveOption(CurveGraph* curveGraph);
		
		public slots:
			void updateData(unsigned int, unsigned int);
			void updateData(unsigned int);
			void updateSerie(unsigned int);
	};
	
	class SignalTimeChart : public SerialChart
	{	
		public :
			SignalTimeChart(Viewer*, QWidget* = NULL);
			SignalTimeChart(Viewer*, string, QWidget*, double = 1, double = 1, int = 10, int = 10, unsigned int = 1, unsigned int = 1);
			~SignalTimeChart();
			
		private :
			void setCurveOption(CurveGraph* curveGraph);
	};
	
	class SpectrumChart : public SerialChart
	{
		public :
			SpectrumChart(Viewer*, QWidget* = NULL);
			SpectrumChart(Viewer*, string, QWidget*, double = 1, double = 1, int = -10, int = 10, int = 0, int = 10, unsigned int = 1, unsigned int = 1);
			~SpectrumChart();
			
		private :
			void setCurveOption(CurveGraph* curveGraph);
	};
	
	class LikelihoodChart : public SerialChart
	{
		public :
			LikelihoodChart(Viewer*, QWidget* = NULL);
			LikelihoodChart(Viewer*, string, QWidget*, double = 1, double = 1, int = 10, int = 10, unsigned int = 1, unsigned int = 1);
			~LikelihoodChart();
			
		private :
			void setCurveOption(CurveGraph* curveGraph);
	};
	
	class FrequencyResponseChart : public SerialChart
	{
		public :
			FrequencyResponseChart(Viewer*, QWidget* = NULL);
			FrequencyResponseChart(Viewer*, string, QWidget*, double = 1, double = 1, int = 10, int = 10, unsigned int = 1, unsigned int = 1);
			~FrequencyResponseChart();
			
		private :
			void setCurveOption(CurveGraph* curveGraph);
	};
	
	class ImpulseResponseChart : public SerialChart
	{
		public :
			ImpulseResponseChart(Viewer*, QWidget* = NULL);
			ImpulseResponseChart(Viewer*, string, QWidget*, double, double, int, int, unsigned int, unsigned int);
			~ImpulseResponseChart();
			
		private :
			void setCurveOption(CurveGraph* curveGraph);
	};
	
	class DataRateChart : public SerialChart
	{
		public :
			DataRateChart(Viewer*, QWidget* = NULL);
			DataRateChart(Viewer*, string, QWidget*, double = 1, double = 1, int = 10, int = 10, unsigned int = 1, unsigned int = 1);
			~DataRateChart();
			
		private :
			void setCurveOption(CurveGraph* curveGraph);
	};
}

#endif