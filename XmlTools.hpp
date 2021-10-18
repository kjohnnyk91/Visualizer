// -*- explicit-buffer-name: "XmlUtil.h<M1-MOBJ/6>" -*-

#ifndef XMLTOOLS_HPP
#define XMLTOOLS_HPP

#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include <string>

namespace ModelView
{
	// std::string xmlCharToString( xmlChar* );
	// bool xmlGetIntAttribute( xmlTextReaderPtr, const std::string& attribute, int& value );
	
	QwtPlotCurve::CurveStyle stringToStyle(std::string sstyle);
	QwtPlotCurve::CurveAttribute stringToAttribute(std::string sattribute);
	QwtPlotCurve::LegendAttribute stringToLegend(std::string slegend);
	QwtPlotCurve::PaintAttribute stringToPaint(std::string spaint);
	QwtSymbol::Style stringToSymbol(std::string ssymbol);
	Qt::GlobalColor stringToColor(std::string scolor);
	Qt::PenStyle stringToPenStyle(std::string sstyle);
	std::string colorString(QColor color);
	std::string styleString(QwtPlotCurve::CurveStyle style);
	std::string penStyleString(Qt::PenStyle style);
	std::string symbolStyleString(QwtSymbol::Style);
}  // ModelView namespace.

#endif  // MODELVIEW_XMLUTIL_H
