#ifndef HPP_VIEWER
#define HPP_VIEWER

// #include <assert.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QMainWindow>
#include <qwt_plot.h>
#include "Indentation.hpp"
#include "SerialChart.hpp"
#include "ScatterChart.hpp"
#include "WaterfallChart.hpp"
#include "NewDialog.hpp"
#include "OpenDialog.hpp"
#include "SaveDialog.hpp"
#include "SettingsDialog.hpp"
#include "ViewerWidget.hpp"
#include "SettingsDialog.hpp"
#include <libxml/xmlreader.h>
#include "XmlTools.hpp"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "DataHeader.hpp"
#include "ChartConnection.hpp"
#include "DataRequest.hpp"
#include "RenameDialog.hpp"
#include "TreeModel.hpp"

using namespace std;

namespace ModelView
{
	class NewDialog;
	class OpenDialog;
	class SaveDialog;
	class ViewerWidget;
	class SerialChart;
	class SettingsDialog;
	class SignalTimeChart;
	class SpectrumChart;
	class LikelihoodChart;
	class FrequencyResponseChart;
	class ImpulseResponseChart;
	class DataRateChart;
	class ChartConnection;
	class TreeModel;
	class TreeItem;
	
	class Viewer : public QMainWindow
	{
		Q_OBJECT
		
		private:
			QApplication* app_;
			map<unsigned int, BaseChart*> charts_;
			map<unsigned int, ChartConnection*> chartConnections_;
			BaseChart* activeChart_;
			ViewerWidget* introWidget_;
			ViewerWidget* chartsWidget_;
			
			QAction* saveAction_;
			
			// TreeModel* model_;
			// TreeItem* item_;
			
			QHostAddress ip_;
			unsigned int port_;
			unsigned int id_;
			DataType type_;
			GraphSeries series_;
			unsigned int numData_;
			ProcessID process_;
			void buildIntro();
		
		public:
			Viewer(QApplication*, QWidget* = NULL);
			~Viewer();
			
			inline QApplication* app() const;
			inline map<unsigned int, BaseChart*> charts() const;
			
			void addChart(BaseChart*);
			static bool isNumber(string);
			
			void addConnection(ChartConnection*);
		
		public slots:
			void newChart();
			void openChart();
			void openCharts();
			void openSave();
			void openRename();
			void openSettings();
			void openTree();
			void openAbout();
			void command(string);
			
			void createNewChart(string title, DataType type, Unit xUnit, double xStep, 
													Multiple xMultiple, int xm, int xM, 
													unsigned int xDivision, Unit yUnit, double yStep, 
													Multiple yMultiple, int ym, int yM, unsigned int yDIvision,
													QColor minColor, QColor maxColor);
			ChartConnection* createChartConnection(BaseChart* chart, QHostAddress ip, unsigned int port, GraphSeries series, unsigned int numData, ProcessID process);
			bool loadChart(string);
			bool loadChart(xmlDocPtr, xmlXPathContextPtr);
			bool loadCharts(string);
			bool loadBase(xmlDocPtr, xmlXPathContextPtr);
			bool loadCurve(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, double xstep, double ystep, int xm,
							int xM, int ym, int yM, unsigned int xdiv, unsigned int ydiv,
							Quantity xqty, Quantity yqty, Unit xunit, Unit yunit, Multiple xmultiple, 
							Multiple ymultiple, DataType type);
			bool loadSerial(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, double xstep, double ystep, int xm,
							int xM, int ym, int yM, unsigned int xdiv, unsigned int ydiv,
							Quantity xqty, Quantity yqty, Unit xunit, Unit yunit, Multiple xmultiple, 
							Multiple ymultiple, DataType type);
			bool loadScatter(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, double xstep, double ystep, int xm,
							int xM, int ym, int yM, unsigned int xdiv, unsigned int ydiv,
							Quantity xqty, Quantity yqty, Unit xunit, Unit yunit, Multiple xmultiple, 
							Multiple ymultiple, DataType type);
			bool loadWaterfall(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, Quantity xqty, Quantity yqty, Unit xunit, 
							Unit yunit, Multiple xmultiple, Multiple ymultiple);
			bool loadGraph(xmlDocPtr reader, xmlXPathContextPtr xpathCtx, Graph* graph);
			bool loadCurveGraph(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							CurveChart* chart);
			bool loadGraphes(xmlDocPtr reader, xmlXPathContextPtr xpathCtx, CurveChart* chart);
			bool saveChart(string filename);
			bool saveChart(std::fstream& file);
			// bool savingChart(string);
			void renameChart(QString);
			void moveChart();
			void removeChart();
			
			void getParameters(QHostAddress, unsigned int, unsigned int, GraphSeries, unsigned int, ProcessID);
			// bool connection();
			void closeAcquisition();
			void abortAcquisition();
			void resumeAcquisition();
			void stopAcquisition();
			
			inline void setActiveChart(BaseChart*);
			void zoomIn();
			void zoomOut();
			void zoomFit();
			
		protected:
			void resizeEvent(QResizeEvent*);
			void mouseMoveEvent(QMouseEvent*);
			void mousePressEvent(QMouseEvent*);
	};
	
	inline QApplication* Viewer::app() const
	{
		return app_;
	}
	
	inline map<unsigned int, BaseChart*> Viewer::charts() const
	{
		return charts_;
	}
	
	inline void Viewer::setActiveChart(BaseChart* chart)
	{
		std::cout << "Viewer::setActiveChart()" << std::endl;
		
		activeChart_ = chart;
		saveAction_-> setVisible(true);
	}
}

#endif
