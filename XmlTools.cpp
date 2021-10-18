#include "XmlTools.hpp"
#include <cstdlib>
#include <iostream>

namespace ModelView
{
	using namespace std;

	QwtPlotCurve::CurveStyle stringToStyle(string sstyle)
	{
		if(sstyle == "nocurve")
			return QwtPlotCurve::NoCurve;
		
		if(sstyle == "sticks")
			return QwtPlotCurve::Sticks;
	
		if(sstyle == "steps")
			return QwtPlotCurve::Steps;
	
		if(sstyle == "dots")
			return QwtPlotCurve::Dots;
		
		if(sstyle == "usercurve")
			return QwtPlotCurve::UserCurve;
		
		const invalid_argument ia(sstyle);
		
		throw ia;
	}
	
	QwtPlotCurve::CurveAttribute stringToAttribute(string attribute)
	{
		if(attribute == "fitted")
			return QwtPlotCurve::Inverted;
		
		if(attribute == "inverted")
			return QwtPlotCurve::Inverted;
		
		const invalid_argument ia(attribute);
		
		throw ia;
	}
	
	QwtPlotCurve::LegendAttribute stringToLegend(string legend)
	{
		if(legend == "no")
			return QwtPlotCurve::LegendNoAttribute;
		
		if(legend == "line")
			return QwtPlotCurve::LegendShowLine;
		
		if(legend == "symbol")
			return QwtPlotCurve::LegendShowSymbol;
			
		if(legend == "brush")
			return QwtPlotCurve::LegendShowBrush;
		
		const std::invalid_argument ia(legend);
		
		throw ia;
	}
	
	QwtPlotCurve::PaintAttribute stringToPaint(string paint)
	{
		if(paint == "clip")
			return QwtPlotCurve::ClipPolygons;
		
		if(paint == "filter")
			return QwtPlotCurve::FilterPoints;
		
		if(paint == "minimize")
			return QwtPlotCurve::MinimizeMemory;
		
		if(paint == "image")
			return QwtPlotCurve::ImageBuffer;
		
		const std::invalid_argument ia(paint);
		
		throw ia;
	}
	
	QwtSymbol::Style stringToSymbol(string symbol)
	{
		if(symbol == "no")
			return QwtSymbol::NoSymbol;
		
		if(symbol == "ellipse")
			return QwtSymbol::Ellipse;
		
		if(symbol == "rectangle")
			return QwtSymbol::Rect;
		
		if(symbol == "diamond")
			return QwtSymbol::Diamond;
		
		if(symbol == "triangle")
			return QwtSymbol::Triangle;
		
		if(symbol == "dtriangle")
			return QwtSymbol::DTriangle;
		
		if(symbol == "utriangle")
			return QwtSymbol::UTriangle;
		
		if(symbol == "ltriangle")
			return QwtSymbol::LTriangle;
		
		if(symbol == "rtriangle")
			return QwtSymbol::RTriangle;
		
		if(symbol == "cross")
			return QwtSymbol::Cross;
		
		if(symbol == "xcross")
			return QwtSymbol::XCross;
		
		if(symbol == "hline")
			return QwtSymbol::HLine;
		
		if(symbol == "vline")
			return QwtSymbol::VLine;
		
		if(symbol == "star1")
			return QwtSymbol::Star1;
		
		if(symbol == "star2")
			return QwtSymbol::Star2;
		
		if(symbol == "hexagon")
			return QwtSymbol::Hexagon;
		
		if(symbol == "path")
			return QwtSymbol::Path;
		
		if(symbol == "pixmap")
			return QwtSymbol::Pixmap;
		
		if(symbol == "graphic")
			return QwtSymbol::Graphic;
		
		if(symbol == "svgdocument")
			return QwtSymbol::SvgDocument;
		
		if(symbol == "userstyle")
			return QwtSymbol::UserStyle;
		
		const invalid_argument ia(symbol);
		
		throw ia;
	}
	
	Qt::GlobalColor stringToColor(string color)
	{
		if(color == "black")
			return Qt::black;
		
		if(color == "white")
			return Qt::white;
		
		if(color == "red")
			return Qt::red;
		
		if(color == "dred")
			return Qt::darkRed;
		
		if(color == "green")
			return Qt::green;
		
		if(color == "dgreen")
			return Qt::darkGreen;
		
		if(color == "blue")
			return Qt::darkBlue;
		
		if(color == "dblue")
			return Qt::darkBlue;
		
		if(color == "cyan")
			return Qt::cyan;
		
		if(color == "dcyan")
			return Qt::darkCyan;
		
		if(color == "magenta")
			return Qt::magenta;
		
		if(color == "dmagenta")
			return Qt::darkMagenta;
		
		if(color == "yellow")
			return Qt::yellow;
		
		if(color == "dyellow")
			return Qt::darkYellow;
		
		if(color == "gray")
			return Qt::gray;
		
		if(color == "dgray")
			return Qt::darkGray;
		
		if(color == "lgray")
			return Qt::lightGray;
		
		if(color == "transparent")
			return Qt::transparent;
		
		if(color == "color0")
			return Qt::color0;
		
		if(color == "color1")
			return Qt::color1;
		
		const invalid_argument ia(color);
		
		throw ia;
	}
	
	Qt::PenStyle stringToPenStyle(string sstyle)
	{
		if(sstyle == "solid")
			return Qt::SolidLine;
		
		if(sstyle == "dash")
			return Qt::DashLine;
		
		if(sstyle == "dot")
			return Qt::DotLine;
		
		if(sstyle == "dashdot")
			return Qt::DashDotLine;
		
		if(sstyle == "dashdotdot")
			return Qt::DashDotDotLine;
		
		if(sstyle == "custom")
			return Qt::CustomDashLine;
		
		const invalid_argument ia(sstyle);
		
		throw ia;
	}
	
	std::string colorString(QColor color)
	{
		std::string red(std::to_string(color.red()));
		std::string green(std::to_string(color.green()));
		std::string blue(std::to_string(color.blue()));
		
		std::string ret("(" + red + "," + green + "," + blue + ")");
		
		return ret;
	}
	
	std::string styleString(QwtPlotCurve::CurveStyle style)
	{
		switch(style)
		{
			case QwtPlotCurve::NoCurve :
			{
				return "nocurve";
			}
			
			case QwtPlotCurve::Lines :
			{
				return "lines";
			}
			
			case QwtPlotCurve::Sticks :
			{
				return "sticks";
			}
			
			case QwtPlotCurve::Steps :
			{
				return "steps";
			}
			
			case QwtPlotCurve::Dots :
			{
				return "dots";
			}
			
			case QwtPlotCurve::UserCurve :
			{
				return "user";
			}
			
			default :
			{
				return "unknown";
			}
		}
	}
	
	std::string penStyleString(Qt::PenStyle style)
	{
		switch(style)
		{
			case Qt::NoPen :
			{
				return "nopen";
			}
			
			case Qt::SolidLine :
			{
				return "solid";
			}
			
			case Qt::DashLine :
			{
				return "dash";
			}
			
			case Qt::DotLine :
			{
				return "dot";
			}
			
			case Qt::DashDotLine :
			{
				return "dashdot";
			}
			
			case Qt::DashDotDotLine :
			{
				return "dashdotdot";
			}
			
			case Qt::CustomDashLine :
			{
				return "custom";
			}
			
			default :
			{
				return "unknown";
			}
		}
	}
	
	std::string symbolStyleString(QwtSymbol::Style style)
	{
		switch(style)
		{
			case QwtSymbol::NoSymbol :
			{
				return "nosymbol";
			}
			
			case QwtSymbol::Ellipse :
			{
				return "ellipse";
			}
			
			case QwtSymbol::Rect :
			{
				return "rect";
			}
			
			case QwtSymbol::Diamond :
			{
				return "diamond";
			}
			
			case QwtSymbol::Triangle :
			{
				return "triangle";
			}
			
			case QwtSymbol::DTriangle :
			{
				return "dtriangle";
			}
			
			case QwtSymbol::UTriangle :
			{
				return "utriangle";
			}
			
			case QwtSymbol::LTriangle :
			{
				return "ltriangle";
			}
			
			case QwtSymbol::RTriangle :
			{
				return "rtriangle";
			}
			
			case QwtSymbol::Cross :
			{
				return "cross";
			}
			
			case QwtSymbol::XCross :
			{
				return "xcross";
			}
			
			case QwtSymbol::HLine :
			{
				return "hline";
			}
			
			case QwtSymbol::VLine :
			{
				return "vline";
			}
			
			case QwtSymbol::Star1 :
			{
				return "star1";
			}
			
			case QwtSymbol::Star2 :
			{
				return "star2";
			}
			
			case QwtSymbol::Hexagon :
			{
				return "hexagon";
			}
			
			case QwtSymbol::Path :
			{
				return "path";
			}
			
			case QwtSymbol::Pixmap :
			{
				return "pixmap";
			}
			
			case QwtSymbol::Graphic :
			{
				return "graphic";
			}
			
			case QwtSymbol::SvgDocument :
			{
				return "svgdocument";
			}
			
			case QwtSymbol::UserStyle :
			{
				return "userstyle";
			}
			
			default :
			{
				return "unknown";
			}
		}
	}
}   // ModelView namespace.
