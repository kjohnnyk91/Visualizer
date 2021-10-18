#ifndef HPP_TYPEHEADER
#define HPP_TYPEHEADER

#include <string>
#include <stdexcept>
#include "IDHeader.hpp"
#include "InterfaceTypeHeader.hpp"

namespace ModelView
{
	enum Multiple
	{
		femto,
		pico,
		nano,
		micro,
		milli,
		unity,
		kilo,
		mega,
		giga,
		tera
	};
	
	enum Unit
	{
		No,
		second,
		Volt,
		Ampere,
		Watt,
		Hertz,
		meter,
		decibel,
		decibelm,
		bytesPerSecond
	};
	
	enum Quantity
	{
		None,
		Time,
		Voltage,
		Current,
		Power,
		Frequency,
		Length,
		DataRate,
		Likelihood,
		Gain,
		Magnitude
	};
	
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
	
	// enum ProcessID
	// {
		// InvalidID,
		// NoProcessID,
		// AverageID,
		// DownsamplingID,
		// RefreshID
	// };
	
	std::string unitString(Unit);
	std::string multipleString(Multiple);
	std::string quantityString(Quantity);
	std::string dataTypeString(DataType);
	
	Unit stringUnit(std::string);
	Multiple stringMultiple(std::string);
	Quantity stringQuantity(std::string);
	DataType stringDataType(std::string);
	
	std::string unitSymbol(Unit);
	std::string multipleSymbol(Multiple);
	
	Quantity unitToQuantity(Unit);
}

#endif