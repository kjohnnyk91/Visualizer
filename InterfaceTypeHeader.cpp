#include "InterfaceTypeHeader.hpp"

namespace ModelView
{
	std::string typeString(DataType type)
	{
		switch(type)
		{
			case UndefinedType :
			{
				return "Undefined type";
				break;
			}
			
			case DataRateType :
			{
				return "Datarate type";
				break;
			}
			
			case FrequencyResponseType :
			{
				return "Frequency response type";
				break;
			}
			
			case ImpulseResponseType :
			{
				return "Impulse response type";
				break;
			}
			
			case LikelihoodType :
			{
				return "Likelihood type";
				break;
			}
			
			case IQType :
			{
				return "IQ type";
				break;
			}
			
			case SignalTimeType :
			{
				return "Signal time type";
				break;
			}
			
			case SignalTimeWaterfallType :
			{
				return "Signal time waterfall type";
				break;
			}
			
			case SpectrumType :
			{
				return "Spectrum type";
				break;
			}
			
			case SerialType :
			{
				return "Serial type";
				break;
			}
			
			case ScatterType :
			{
				return "Scatter type";
				break;
			}
			
			case WaterfallType :
			{
				return "Waterfall type";
				break;
			}
			
			default :
			{
				return std::string();
				break;
			}
		}
	}
}