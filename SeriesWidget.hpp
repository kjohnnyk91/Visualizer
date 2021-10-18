#ifndef HPP_SERIESWIDGET
#define HPP_SERIESWIDGET

#include "Viewer.hpp"
#include "GraphSeries.hpp"
#include <set>
#include <QSpinBox>
#include <QPushButton>

namespace ModelView
{
	class SeriesWidget : public QWidget
	{
		Q_OBJECT
		
		private :
			Viewer* viewer_;
			GraphSeries series_;
			QSpinBox* serieSpin_;
			QSpinBox* signalSpin_;
			QPushButton* addSerieButton_;
			QPushButton* removeSerieButton_;
			QString seriesString_;
			QLineEdit* seriesLine_;
			
		public :
			SeriesWidget(QWidget* = NULL);
			~SeriesWidget();
			
			inline GraphSeries series() const;
			
		signals :
			void sendSeries(std::set<unsigned int>);
			
		public slots :
			void addSerie();
			void removeSerie();
	};
	
	inline GraphSeries SeriesWidget::series() const
	{
		return series_;
	}
}

#endif