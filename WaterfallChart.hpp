#ifndef HPP_WATERFALLCHART
#define HPP_WATERFALLCHART

#include <complex>
#include <fstream>
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_color_map.h>
// #include "qwt_plot_canvas.h"
// #include "qwt_plot_magnifier.h"
// #include "qwt_plot_panner.h"
#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QtWidgets>
#include "GraphView.hpp"
#include "DataHeader.hpp"
#include "Indentation.hpp"
#include "XmlTools.hpp"

using namespace std;

namespace ModelView
{
	class WaterfallChart : public BaseChart
	{
		private:
			Graph* graph_;
			QwtPlotSpectrogram::DisplayMode display_;
			QwtPlotSpectrogram* plot_;
			QwtMatrixRasterData* datas_;
			QwtInterval xInterval_;
			QwtInterval yInterval_;
			QwtInterval zInterval_;
			double timeStep_;
			unsigned int rows_;
			unsigned int columns_;
			double min_;
			double max_;
			QColor minColor_;
			QColor maxColor_;
			unsigned int index_;
			
			
		public:
			WaterfallChart(Viewer* viewer, std::string title = "Waterfall chart", double timeStep = 1, int rows = 400, int columns = 1000, QColor minColor = QColor(Qt::white), QColor maxColor = QColor(Qt::black), bool custom = false, QWidget* parent = NULL);
			~WaterfallChart();
			
			inline Graph* graph() const;
			inline QwtPlotSpectrogram* plot() const;
			inline QwtMatrixRasterData* datas() const;
			inline QwtInterval xInterval() const;
			inline QwtInterval yInterval() const;
			inline QwtInterval zInterval() const;
			inline double timeStep() const;
			inline unsigned int rows() const;
			inline unsigned int columns() const;
			inline double min() const;
			inline double max() const;
			inline unsigned int index() const;
			
			void setGraph(Graph*);
			inline void setPlot(QwtPlotSpectrogram*);
			inline void setData(QwtMatrixRasterData*);
			inline void setTimeStep(double);
			inline void setRows(unsigned int);
			inline void setColumns(unsigned int);
			inline void setMin(double);
			inline void setMax(double);
			inline void setIndex(unsigned int);
			
			inline void setColorMap(QwtLinearColorMap*);
			inline void addColorStop(double, const QColor&);
			inline void setXInterval(double, double, QwtInterval::BorderFlags = QwtInterval::IncludeBorders);
			inline void setYInterval(double, double, QwtInterval::BorderFlags = QwtInterval::IncludeBorders);
			inline void setZInterval(double, double, QwtInterval::BorderFlags = QwtInterval::IncludeBorders);
			
			inline const QwtColorMap* colorMap();
			
			void addData(std::complex<short> cs, unsigned int serie = 0);
			void addData(std::complex<int> ci, unsigned int serie = 0);
			void addData(std::complex<float> cf, unsigned int serie = 0);
			void addData(std::complex<double> c, unsigned int serie = 0);
			void clear();
			void clearData();
			
			bool createSerie(unsigned int serie);
			
			inline void setDisplayMode(QwtPlotSpectrogram::DisplayMode display);
			static QwtPlotSpectrogram::DisplayMode stringDisplayMode(std::string sdisplay);
			static std::string displayModeString(QwtPlotSpectrogram::DisplayMode display);
			
			bool toXml2(std::fstream& file);
			
		public slots:
			void updateData(unsigned int serie);
			void updateData(unsigned int index, unsigned int serie);
			void updateSerie();
	};

	inline Graph* WaterfallChart::graph() const
	{
		return graph_;
	}
	
	inline QwtPlotSpectrogram* WaterfallChart::plot() const
	{
		return plot_;
	}
	
	inline QwtMatrixRasterData* WaterfallChart::datas() const
	{
		return datas_;
	}

	inline double WaterfallChart::timeStep() const
	{
		return timeStep_;
	}

	inline unsigned int WaterfallChart::rows() const
	{
		return rows_;
	}

	inline unsigned int WaterfallChart::columns() const
	{
		return columns_;
	}

	inline double WaterfallChart::min() const
	{
		return min_;
	}

	inline double WaterfallChart::max() const
	{
		return max_;
	}

	inline unsigned int WaterfallChart::index() const
	{
		return index_;
	}

	inline const QwtColorMap* WaterfallChart::colorMap()
	{
		return plot_-> colorMap();
	}

	inline void WaterfallChart::setPlot(QwtPlotSpectrogram* plot)
	{
		plot_ = plot;
	}

	inline void WaterfallChart::setData(QwtMatrixRasterData* datas)
	{
		datas_ = datas;
	}

	inline void WaterfallChart::setTimeStep(double timeStep)
	{
		timeStep_ = timeStep;
	}

	inline void WaterfallChart::setRows(unsigned int rows)
	{
		rows_ = rows;
	}

	inline void WaterfallChart::setColumns(unsigned int columns)
	{
		columns_ = columns;
	}

	inline void WaterfallChart::setMin(double min)
	{
		min_ = min;
	}

	inline void WaterfallChart::setMax(double max)
	{
		max_ = max;
	}

	inline void WaterfallChart::setIndex(unsigned int index)
	{
		index_ = index;
	}

	inline void WaterfallChart::setColorMap(QwtLinearColorMap* colorMap)
	{
		plot_-> setColorMap(colorMap);
	}

	inline void WaterfallChart::addColorStop(double value, const QColor& color)
	{
		QwtLinearColorMap* qwtColorMap((QwtLinearColorMap*) colorMap());
		
		if((value >= 0) and (value <= 1))
		{
			qwtColorMap-> addColorStop(value, color);
		}
	}

	inline void WaterfallChart::setXInterval(double min, double max, QwtInterval::BorderFlags flag)
	{
		datas_-> setInterval(Qt::XAxis, QwtInterval(min, max, flag));
	}

	inline void WaterfallChart::setYInterval(double min, double max, QwtInterval::BorderFlags flag)
	{
		datas_-> setInterval(Qt::YAxis, QwtInterval(min, max, flag));
	}

	inline void WaterfallChart::setZInterval(double min, double max, QwtInterval::BorderFlags flag)
	{
		datas_-> setInterval(Qt::ZAxis, QwtInterval(min, max, flag));
	}
	
	inline void WaterfallChart::setDisplayMode(QwtPlotSpectrogram::DisplayMode display)
	{
		plot_-> setDisplayMode(display);
	}
}
#endif