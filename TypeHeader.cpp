#include "TypeHeader.hpp"

namespace ModelView
{
	std::string unitString(Unit unit)
	{
		switch(unit)
		{
			case No :
				return "";
			
			case second :
				return "second";
			
			case Volt :
				return "Volt";
			
			case Ampere :
				return "Ampere";
			
			case Watt :
				return "Watt";
			
			case Hertz :
				return "Hertz";
			
			case meter :
				return "meter";
				
			case decibel :
				return "decibel";
				
			case decibelm :
				return "decibelm";
				
			case bytesPerSecond :
				return "bytes per second";
				
			default :
				return "?";
		}
	}
	
	std::string multipleString(Multiple multiple)
	{
		switch(multiple)
		{
			case femto :
				return "femto";
			
			case pico :
				return "pico";
			
			case nano :
				return "nano";
			
			case micro :
				return "micro";
			
			case milli :
				return "milli";
			
			case unity :
				return "";
			
			case kilo :
				return "kilo";
			
			case mega :
				return "mega";
			
			case giga :
				return "giga";
			
			case tera :
				return "tera";
				
			default :
				return "?";
		}
	}
	
	std::string quantityString(Quantity quantity)
	{
		switch(quantity)
		{
			case None :
				return "";
			
			case Time :
				return "Time";
			
			case Voltage :
				return "Voltage";
			
			case Current :
				return "Current";
			
			case Power :
				return "Power";
			
			case Frequency :
				return "Frequency";
				
			case Length :
				return "Length";
				
			case DataRate :
				return "Data rate";
				
			case Likelihood :
				return "Likelihood";
				
			case Gain :
				return "Gain";
				
			default :
				return "?";
		}
	}
	
	std::string dataTypeString(DataType type)
	{
		switch(type)
		{
			case UndefinedType :
			{
				return "UndefinedType";
			}
			
			case DataRateType :
			{
				return "DatarateType";
			}
			
			case FrequencyResponseType :
			{
				return "FrequencyResponseType";
			}
			
			case ImpulseResponseType :
			{
				return "ImpulseResponseType";
			}
			
			case LikelihoodType :
			{
				return "LikelihoodType";
			}
			
			case IQType :
			{
				return "IQType";
			}
			
			case ScatterType :
			{
				return "ScatterType";
			}
			
			case SerialType :
			{
				return "SerialType";
			}
			
			case SignalTimeType :
			{
				return "SignalTimeType";
			}
			
			case SpectrumType :
			{
				return "SpectrumType";
			}
			
			case WaterfallType :
			{
				return "WaterfallType";
			}
			
			default :
			{
				return "Unknown type";
			}
		}
	}
	
	Unit stringUnit(std::string sunit)
	{
		if(sunit == "")
			return No;
		
		if(sunit == "second" or sunit == "Second")
			return second;
		
		if(sunit == "volt" or sunit == "Volt")
			return Volt;
		
		if(sunit == "ampere" or sunit == "Ampere")
			return Ampere;
		
		if(sunit == "watt" or sunit == "Watt")
			return Watt;
		
		if(sunit == "hertz" or sunit == "Hertz")
			return Hertz;
		
		if(sunit == "meter" or sunit == "Meter")
		{
			return meter;
		}
		
		if(sunit == "decibel" or sunit == "Decibel")
		{
			return decibel;
		}
		
		if(sunit == "decibelm" or sunit == "Decibelm")
		{
			return decibelm;
		}
		
		if(sunit == "bytespersec")
		{
			return bytesPerSecond;
		}
		else
		{
			const std::invalid_argument ia(sunit);
		
			throw ia;
		}
	}
	
	Multiple stringMultiple(std::string smultiple)
	{
		if(smultiple == "femto")
			return femto;
		
		if(smultiple == "pico")
			return pico;
		
		if(smultiple == "nano")
			return nano;
		
		if(smultiple == "micro")
			return micro;
		
		if(smultiple == "milli")
			return milli;
		
		if(smultiple == "unity")
			return unity;
		
		if(smultiple == "kilo")
			return kilo;
		
		if(smultiple == "mega")
			return mega;
		
		if(smultiple == "giga")
			return giga;
		
		if(smultiple == "tera")
		{
			return tera;
		}
		else
		{
			const std::invalid_argument ia(smultiple);
		
			throw ia;
		}
	}
	
	Quantity stringQuantity(std::string squantity)
	{
		if((squantity == "None") or (squantity == "none"))
			return None;
		
		if((squantity == "Time") or (squantity == "time"))
			return Time;
		
		if((squantity == "Voltage") or (squantity == "voltage"))
			return Voltage;
		
		if((squantity == "Current") or (squantity == "current"))
			return Current;
		
		if((squantity == "Power") or (squantity == "power"))
			return Power;
		
		if((squantity == "Frequency") or(squantity == "frequency"))
			return Frequency;
		
		if((squantity == "Length") or (squantity == "length"))
			return Length;
		
		if((squantity == "Datarate") or(squantity == "datarate"))
			return DataRate;
		
		if((squantity == "Likelihood") or (squantity == "likelihood"))
			return Likelihood;
		
		const std::invalid_argument ia(squantity);
		
		throw ia;
	}
	
	DataType stringDataType(std::string stype)
	{
		if(stype == "undefined")
			return UndefinedType;
		
		if(stype == "datarate")
			return DataRateType;
		
		if(stype == "frequency")
			return FrequencyResponseType;
		
		if(stype == "impulse")
			return ImpulseResponseType;
		
		if(stype == "likelihood")
			return LikelihoodType;
		
		if(stype == "iq")
			return IQType;
		
		if(stype == "scatter")
			return ScatterType;
		
		if(stype == "serial")
			return SerialType;
		
		if(stype == "signaltime")
			return SignalTimeType;
		
		if(stype == "spectrum")
			return SpectrumType;
		
		if(stype == "waterfall")
			return WaterfallType;
		else
			return UndefinedType;
	}
	
	std::string unitSymbol(Unit unit)
	{
		switch(unit)
		{
			case No :
				return "";
			
			case second :
				return "s";
			
			case Volt :
				return "V";
			
			case Ampere :
				return "A";
			
			case Watt :
				return "W";
			
			case Hertz :
				return "Hz";
			
			case meter :
				return "m";
				
			case decibel :
				return "dB";
			
			default :
				return "?";
		}
	}
	
	std::string multipleSymbol(Multiple multiple)
	{
		switch(multiple)
		{
			case femto :
				return "f";
			
			case pico :
				return "p";
			
			case nano :
				return "n";
			
			case micro :
				return "u";
			
			case milli :
				return "m";
			
			case unity :
				return "";
			
			case kilo :
				return "k";
			
			case mega :
				return "M";
			
			case giga :
				return "G";
			
			case tera :
				return "T";
				
			default :
				return "?";
		}
	}
	
	Quantity unitToQuantity(Unit unit)
	{
		switch(unit)
		{
			case No :
			case decibel :
				return None;
				
			case second :
				return Time;
				
			case Volt :
				return Voltage;
				
			case Ampere :
				return Current;
				
			case Watt :
				return Power;
				
			case Hertz :
				return Frequency;
				
			case meter :
				return Length;
				
			case bytesPerSecond :
				return DataRate;
				
			default :
				return None;
		}
	}
	
}