#include "ScatterData.hpp"

namespace ModelView
{
	ScatterData::ScatterData()
	{}

	ScatterData::~ScatterData()
	{}

	QRectF ScatterData::boundingRect() const
	{
		if(d_samples.isEmpty())
			return QRectF();
		
		double xMin(d_samples.constFirst().x());
		double xMax(d_samples.constFirst().x());
		double yMin(d_samples.constFirst().y());
		double yMax(d_samples.constFirst().y());
		
		for(QVector<QPointF>::const_iterator it = d_samples.begin() + 1; it < d_samples.end(); it++)
		{
			if(it-> x() < xMin)
				xMin = it-> x();
			
			if(it-> x() > xMax)
				xMax = it-> x();
			
			if(it-> y() < yMin)
				yMin = it-> y();
			
			if(it-> y() > yMax)
				yMax = it-> y();
		}
		
		return QRectF(xMin, yMin, xMax - xMin, yMax - yMin);
	}

	void ScatterData::clear()
	{
		d_samples.clear();
		d_samples.squeeze();
		d_boundingRect = QRect();
	}
}