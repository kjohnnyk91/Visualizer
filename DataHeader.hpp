#ifndef HPP_DATAHEADER
#define HPP_DATAHEADER

#define foreach_type(type) \
	type(TypeUndefined) \
	type(SignalTime) \
	type(Spectrum) \
	type(LikelihoodRatio)\
	type(FrequencyResponse)\
	type(ImpulseResponse)\
	type(WaterfallData)\
	type(DataRate)\
	type(IQData)\
	type(Custom)
#define type_enum(type) type,

#include "InterfaceTypeHeader.hpp"
#include "IDHeader.hpp"

// #define INVALID_ID 0
// #define SIGNALTIME_ID 1
// #define SPECTRUM_ID 2
// #define LIKELIHOOD_ID 3
// #define FREQUENCYRESPONSE_ID 4
// #define IMPULSERESPONSE_ID 5
// #define DATARATE_ID 6
// #define IQ_ID 7
// #define WATERFALL_ID 8

namespace ModelView
{
	class DataHeader
	{
		public :
			// enum DataFormat
			// {
				// FormatUndefined,
				// Short,
				// Int,
				// Float,
				// Double
			// };
			
			// enum DataType
			// {
				// UndefinedType,
				// SignalTimeType,
				// SpectrumType,
				// LikelihoodType,
				// FrequencyResponseType,
				// ImpulseResponseType,
				// DataRateType,
				// IQType,
				// WaterfallType,
				// SerialType,
				// ScatterType
			// };
			
			// enum State
			// {
				// Begin,
				// Send,
				// End,
				// BeginEnd
			// };

		private :
			unsigned int id_;
			DataType type_;
			DataFormat format_;
			unsigned int serie_;
			unsigned int number_;
			State state_;

		public :
			inline DataHeader();
			inline DataHeader(unsigned int);
			inline ~DataHeader();

			inline unsigned int id() const;
			inline DataFormat format() const;
			inline DataType type() const;
			inline unsigned int serie() const;
			inline unsigned int number() const;
			inline State state() const;

			inline void setID(unsigned int id);
			inline void setFormat(DataFormat format);
			inline void setType(DataType type);
			inline void setSerie(unsigned int serie);
			inline void setNumber(unsigned int number);
			inline void setState(State state);
			
			static inline DataType IDToType(unsigned int ID);
	};

	inline DataHeader::DataHeader():
		id_(0),
		format_(FormatUndefined),
		serie_(0),
		number_(0),
		state_(Begin)
	{}

	inline DataHeader::DataHeader(unsigned int id):
		id_(id),
		format_(FormatUndefined),
		serie_(0),
		number_(0),
		state_(Begin)
	{}

	inline DataHeader::~DataHeader()
	{}

	inline unsigned int DataHeader::id() const
	{
		return id_;
	}

	inline DataFormat DataHeader::format() const
	{
		return format_;
	}
	
	inline DataType DataHeader::type() const
	{
		return type_;
	}

	inline unsigned int DataHeader::serie() const
	{
		return serie_;
	}

	inline unsigned int DataHeader::number() const
	{
		return number_;
	}
	
	inline State DataHeader::state() const
	{
		return state_;
	}
	
	inline void DataHeader::setID(unsigned int id)
	{
		id_ = id;
	}

	inline void DataHeader::setFormat(DataFormat format)
	{
		format_ = format;
	}
	
	inline void DataHeader::setType(DataType type)
	{
		type_ = type;
	}

	inline void DataHeader::setSerie(unsigned int serie)
	{
		serie_ = serie;
	}

	inline void DataHeader::setNumber(unsigned int number)
	{
		number_ = number;
	}
	
	inline void DataHeader::setState(State state)
	{
		state_ = state;
	}
	
	inline DataType DataHeader::IDToType(unsigned int ID)
	{
		switch(ID)
		{
			case INVALID_ID :
			{
				return UndefinedType;
				
				break;
			}
			
			case SIGNALTIME_ID :
			{
				return SignalTimeType;
				
				break;
			}
			
			case SPECTRUM_ID :
			{
				return SpectrumType;
			}
			
			case LIKELIHOOD_ID :
			{
				return LikelihoodType;
				
				break;
			}
			
			case FREQUENCYRESPONSE_ID :
			{
				return FrequencyResponseType;
				
				break;
			}
			
			case IMPULSERESPONSE_ID :
			{
				return ImpulseResponseType;
				
				break;
			}
			
			case DATARATE_ID :
			{
				return DataRateType;
				
				break;
			}
			
			case IQ_ID :
			{
				return IQType;
				
				break;
			}
			
			case WATERFALL_ID :
			{
				return WaterfallType;
				
				break;
			}
			
			default :
			{
				return UndefinedType;
				
				break;
			}
		}
	}
}

#endif