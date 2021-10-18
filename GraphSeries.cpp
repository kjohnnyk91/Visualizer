#include "GraphSeries.hpp"

namespace ModelView
{
	SerieSignal::SerieSignal():
		serie_(UNDEFINED_SERIE),
		signal_(UNDEFINED_SIGNAL)
	{}
	
	SerieSignal::SerieSignal(const SerieSignal& serieSignal):
		serie_(serieSignal.serie()),
		signal_(serieSignal.signal())
	{}
		
	SerieSignal::SerieSignal(unsigned int serie, unsigned int signal):
		serie_(serie),
		signal_(signal)
	{}
	
	SerieSignal::~SerieSignal()
	{}
		
	unsigned int SerieSignal::serie() const
	{
		return serie_;
	}
	
	unsigned int SerieSignal::signal() const
	{
		return signal_;
	}
	
	void SerieSignal::setSerie(unsigned int serie)
	{
		serie_ = serie;
	}
	
	void SerieSignal::setSignal(unsigned int signal)
	{
		signal_ = signal;
	}
	
	std::ostream& operator<<(std::ostream& stream, const SerieSignal& serieSignal)
	{
		return stream << '(' << serieSignal.serie() << ',' << serieSignal.signal() << ')';
	}
	
	GraphSeries::GraphSeries() :
		series_(),
		end_(0)
	{
		for(unsigned int i = 0; i < MAX_SERIE; i++)
		{
			series_[i] = SerieSignal();
		}
	}
	
	GraphSeries::GraphSeries(SerieSignal series[MAX_SERIE]) :
		series_(),
		end_(0)
	{
		for(unsigned int i = 0; i < MAX_SERIE; i++)
		{
			if(series[i].serie() == UNDEFINED_SERIE)
			{
				break;
			}
			else
			{
				try
				{
					addSerie(series[i]);
				}
				catch(const std::out_of_range& oor)
				{
					std::cerr << "GraphSeries::GraphSeries() : " << oor.what() << std::endl;
				}
			}
		}
	}
	
	GraphSeries::~GraphSeries()
	{}
	
	unsigned int GraphSeries::end() const
	{
		return end_;
	}
	
	SerieSignal GraphSeries::serie(unsigned int index) const
	{
		if(index > end_)
		{
			const std::out_of_range oor("The index is out of range");
			
			throw oor;
		}
		else
		{
			return series_[index];
		}
	}
	
	bool GraphSeries::setSerie(SerieSignal serieSignal)
	{
		unsigned int serie(serieSignal.serie());
		unsigned int signal(serieSignal.signal());
		
		if(serie >= MAX_SERIE)
		{
			std::cerr << "GraphSeries::setSerie() : The serie is out of range" << std::endl;
			
			return false;
		}
		
		unsigned int ret(find(serie));
		
		if(ret != MAX_SERIE)
		{
			series_[ret] = serieSignal;
			
			return true;
		}
		else
		{
			return addSerie(serieSignal);
		}
	}
	
	bool GraphSeries::addSerie(SerieSignal serie)
	{
		if(serie.serie() >= MAX_SERIE)
		{
			return false;
		}
		else
		{
			if(find(serie.serie()) == MAX_SERIE)
			{
				series_[end_] = serie;
				end_++;
				
				return true;
			}
			else
			{
				std::cerr << "GraphSeries::addSerie() : The serie " << serie.serie() << " has already been added" << std::endl;
				
				return false;
			}
		}
	}
	
	bool GraphSeries::removeSerie()
	{
		if(end_ > 0)
		{
			end_--;
			series_[end_] = SerieSignal();
			
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool GraphSeries::removeSerie(unsigned int serie)
	{
		unsigned int index(find(serie));
		
		if(index == MAX_SERIE)
		{
			std::cerr << "The serie was not found" << std::endl;
			
			return false;
		}
		else
		{
			for(unsigned int i = (index + 1) ; i < end_; i++)
			{
				series_[i - 1] = series_[i]; 
			}
			
			end_--;
			series_[end_] = SerieSignal();
			
			return true;
		}
	}
	
	unsigned int GraphSeries::find(unsigned int serie) const
	{
		for(unsigned int i = 0; i < end_; i++)
		{
			if(series_[i].serie() == serie)
			{
				return i;
			}
		}
		
		return MAX_SERIE;
	}
}