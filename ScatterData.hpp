#ifndef HPP_SCATTERDATA
#define HPP_SCATTERDATA

#include <qwt_series_data.h>
#include <QPointF>
#include <iostream>

namespace ModelView
{
	class ScatterData : public QwtArraySeriesData<QPointF>
	{
		public:
			ScatterData();
			~ScatterData();
			
			QRectF boundingRect() const;
			inline QPointF& data(unsigned int);
			
			inline void append(const QPointF&);
			inline void remove(unsigned int);
			void clear();
	};

	QPointF& ScatterData::data(unsigned int index)
	{
		return d_samples[index];
	}

	inline void ScatterData::append(const QPointF& point)
	{
		d_samples.append(point);
	}

	inline void ScatterData::remove(unsigned int index)
	{
		d_samples.remove(index);
	}
}

#endif