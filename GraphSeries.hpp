#ifndef HPP_GRAPHSERIES
#define HPP_GRAPHSERIES

#define MAX_SERIE 10
#define UNDEFINED_SERIE 99
#define UNDEFINED_SIGNAL 99

#include <stdexcept>
#include <iostream>

namespace ModelView
{
	class SerieSignal
	{
		private :
			unsigned int serie_;
			unsigned int signal_;
		
		public :
			SerieSignal();
			SerieSignal(const SerieSignal& serieSignal);
			SerieSignal(unsigned int serie, unsigned int signal = UNDEFINED_SIGNAL);
			~SerieSignal();
			
			unsigned int serie() const;
			unsigned int signal() const;
			
			void setSerie(unsigned int serie);
			void setSignal(unsigned int signal);
	};
	
	std::ostream& operator<<(std::ostream& stream, const SerieSignal& serieSignal);
	
	class GraphSeries
	{
		private :
			SerieSignal series_[MAX_SERIE];
			unsigned int end_;
			
		public :
			GraphSeries();
			GraphSeries(SerieSignal series[MAX_SERIE]);
			~GraphSeries();
			
			unsigned int end() const;
			SerieSignal serie(unsigned int index) const;
			
			bool setSerie(SerieSignal serie);
			bool addSerie(SerieSignal serie);
			bool removeSerie();
			bool removeSerie(unsigned int serie);
			
			unsigned int find(unsigned int serie) const;
	};
}

#endif