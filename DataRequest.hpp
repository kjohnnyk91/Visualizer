#ifndef HPP_DATAREQUEST
#define HPP_DATAREQUEST

#include "GraphSeries.hpp"
#include "InterfaceTypeHeader.hpp"
#include "IDHeader.hpp"

namespace ModelView
{
	class SerieSignal;
	
	class DataRequest
	{
		private:
			unsigned int id_;
			DataType type_;
			GraphSeries series_;
			unsigned int numData_;
			ProcessID processID_;
			
		public:
			DataRequest();
			DataRequest(unsigned int id, DataType type, GraphSeries series, unsigned int numData, ProcessID processID);
			~DataRequest();
			
			inline unsigned int id() const;
			inline DataType type() const;
			inline GraphSeries series();
			inline unsigned int numData() const;
			inline ProcessID processID() const;
			
			inline void setID(unsigned int);
			inline void setType(DataType);
			inline void setSeries(GraphSeries);
			inline void setNumData(unsigned int);
			inline void setProcessID(ProcessID);
			
			SerieSignal serie(unsigned int) const;
			inline bool setSerie(SerieSignal serieSignal);
			inline bool addSerie(SerieSignal serieSignal);
			inline bool removeSerie();
			inline bool removeSerie(unsigned int serie);
			inline unsigned int size() const;
			
			// void updateRequest(GraphSeries series);
	};
	
	inline unsigned int DataRequest::id() const
	{
		return id_;
	}
	
	inline DataType DataRequest::type() const
	{
		return type_;
	}
	
	inline GraphSeries DataRequest::series()
	{
		return series_;
	}
	
	inline unsigned int DataRequest::numData() const
	{
		return numData_;
	}
	
	inline ProcessID DataRequest::processID() const
	{
		return processID_;
	}
	
	inline void DataRequest::setID(unsigned int id)
	{
		id_ = id;
	}
	
	inline void DataRequest::setType(DataType type)
	{
		type_ = type;
	}
	
	inline void DataRequest::setSeries(GraphSeries series)
	{
		series_ = series;
	}
	
	inline void DataRequest::setNumData(unsigned int numData)
	{
		numData_ = numData;
	}
	
	inline void DataRequest::setProcessID(ProcessID processID)
	{
		processID_ = processID;
	}
	
	inline bool DataRequest::setSerie(SerieSignal serieSignal)
	{
		return series_.setSerie(serieSignal);
	}
	
	inline bool DataRequest::addSerie(SerieSignal serie)
	{
		return series_.addSerie(serie);
	}
	
	inline bool DataRequest::removeSerie()
	{
		return series_.removeSerie();
	}
	
	inline bool DataRequest::removeSerie(unsigned int serie)
	{
		return series_.removeSerie(serie);
	}
	
	inline unsigned int DataRequest::size() const
	{
		return series_.end();
	}
}

#endif