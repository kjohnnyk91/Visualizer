#include "DataRequest.hpp"

namespace ModelView
{
	DataRequest::DataRequest():
		id_(INVALID_ID),
		type_(UndefinedType),
		series_(),
		numData_(0),
		processID_(InvalidID)
	{}
	
	DataRequest::DataRequest(unsigned int id, DataType type, GraphSeries series, unsigned int numData, ProcessID processID):
		id_(id),
		type_(type),
		series_(series),
		numData_(numData),
		processID_(processID)
	{}
	
	DataRequest::~DataRequest()
	{}
	
	SerieSignal DataRequest::serie(unsigned int index) const
	{
		SerieSignal ret;
		
		try
		{
			ret = series_.serie(index);
		}
		catch(const std::out_of_range& oor)
		{
			std::cerr << "DataRequest::serie() : " << oor.what() << std::																																																																																																																															endl;
		}
		
		return ret;
	}
	
	// void DataRequest::updateRequest(GraphSeries series)
	// {
		// for(unsigned int i = 0; i < series.end(); i++)
		// {
			// SerieSignal serieSignal(series.serie(i));
			// unsigned int serie(serieSignal.serie());
			
			// unsigned int ret(series_.find(serie));
			
			// if(ret != MAX_SERIE)
			// {
				// series_.setSerie(serieSignal);
			// }
			// else
			// {
				// series_.addSerie(serieSignal);
			// }
		// }
	// }
}