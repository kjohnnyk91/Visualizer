#ifndef HPP_INTERFACETYPE
#define HPP_INTERFACETYPE

#include <string>

namespace ModelView
{
	enum DataFormat
	{
		FormatUndefined,
		Short,
		Int,
		Float,
		Double
	};
	
	enum DataType
	{
		UndefinedType,
		DataRateType,
		FrequencyResponseType,
		ImpulseResponseType,
		LikelihoodType,
		IQType,
		SignalTimeType,
		SignalTimeWaterfallType,
		SpectrumType,
		SerialType,
		ScatterType,
		WaterfallType
	};
	
	enum State
	{
		Begin,
		Send,
		End,
		BeginEnd
	};
	
	enum ProcessID
	{
		InvalidID,
		NoProcessID,
		AverageID,
		DownsamplingID,
		RefreshID
	};
	
	std::string typeString(DataType type);
}

#endif