#include "Viewer.hpp"

#define UNDEFINED "undefined"

namespace ModelView
{
	static bool noCaseEqual(const string &str1, const string &str2)
	{
		if (str1.size() != str2.size())
		{
			return false;
		}

		for (auto c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2)
		{
			if (tolower(*c1) != tolower(*c2))
			{
				return false;
			}
		}

		return true;
	}

	static const bool debug_read_xml=1;

	static string xmlGetAttr(xmlDocPtr reader, xmlXPathContextPtr xpathCtx, string xpath)
	{
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST xpath.c_str(), xpathCtx);

		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "xmlGetAttr::Can't read path:" << xpath << endl;
			return UNDEFINED;
		}

		xmlNodeSetPtr nodeset=xpathObj->nodesetval;

		if ( nodeset->nodeNr != 1 )
		{
			cerr << "Must have 1 value in " << xpath << " but values=" << nodeset->nodeNr << endl;
			xmlXPathFreeObject(xpathObj);
			return UNDEFINED;
		}

		xmlChar *val=xmlNodeListGetString(reader, nodeset->nodeTab[0]->xmlChildrenNode, 1);

		if (!val)
		{
			cerr << "xmlNodeListGetString should not fail" << endl;
			return UNDEFINED;
		}

		string ret2((char *)val);

		// if (debug_read_xml)
			// cout << "read: " << xpath << "=" << ret2 <<endl;

		xmlFree(val);
		xmlXPathFreeObject(xpathObj);
		return ret2;
	}

	static complex<double> readPoint(xmlNodeSetPtr nodeset, int index)
	{
		complex<double> c(0,0);
		xmlChar *x=xmlGetProp(nodeset->nodeTab[index],BAD_CAST "x");
		xmlChar *y=xmlGetProp(nodeset->nodeTab[index],BAD_CAST "y");

		if ( x==NULL || y==NULL)
		{
			cerr << "lack of x or y in point" <<endl;
		}
		else
		{
			c=complex<double>(stod((char *)x), stod((char *)y));
			xmlFree(x);
			xmlFree(y);
		}
		return c;
	}

	void Viewer::buildIntro()
	{
		QVBoxLayout *layout(new QVBoxLayout(introWidget_));
		QLabel *title(new QLabel(QString("Signal viewer"), introWidget_));
		QPushButton *newButton(new QPushButton(QString("New chart"), introWidget_));
		QPushButton *chartButton(new QPushButton(QString("Open chart"), introWidget_));
		// QPushButton* saveButton(new QPushButton(QString("Save chart")));
		QPushButton *chartsButton(new QPushButton(QString("Open charts"), introWidget_));
		QPushButton *settingsButton(new QPushButton(QString("Settings"), introWidget_));
		
		title-> setAlignment(Qt::AlignHCenter);
		// title-> setGeometry(0,0,200,50);
		newButton-> setFixedHeight(100);
		chartButton-> setFixedHeight(100);
		chartsButton-> setFixedHeight(100);
		settingsButton-> setFixedHeight(100);
		introWidget_-> setLayout(layout);
		layout-> setSpacing(0);
		layout-> addWidget(title);
		layout-> addStretch();
		layout-> addWidget(newButton);
		layout-> addStretch();
		layout-> addWidget(chartButton);
		layout-> addStretch();
		// layout-> addWidget(saveButton);
		// layout-> addStretch();
		layout-> addWidget(chartsButton);
		layout-> addStretch();
		layout-> addWidget(settingsButton);
		QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(newChart()));
		QObject::connect(chartButton, SIGNAL(clicked()), this, SLOT(openChart()));
		// QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveChart()));
		QObject::connect(chartsButton, SIGNAL(clicked()), this, SLOT(openCharts()));
		QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(openSettings()));
	}

	Viewer::Viewer(QApplication *app, QWidget *parent) :
		QMainWindow(parent, Qt::Window),
		app_(app),
		charts_(),
		activeChart_(NULL),
		introWidget_(NULL),
		chartsWidget_(NULL),
		saveAction_(NULL),
		ip_(QHostAddress(QString("127.0.0.1"))),
		port_(8888),
		id_(INVALID_ID),
		series_(),
		numData_(0),
		process_(InvalidID)
	{
		setGeometry(20,20,800,600);
		
		try
		{
			introWidget_ = new ViewerWidget(this, parent);
			chartsWidget_ = new ViewerWidget(this, parent);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::Viewer() : bad allocation of " << ba.what() << std::endl;

			return;
		}
		
		
		buildIntro();
		setCentralWidget(introWidget_);
		QGridLayout *layout(new QGridLayout(chartsWidget_));
		unsigned int i(0);

		for(map<unsigned int, BaseChart *>::iterator it = charts_.begin(); it != charts_.end(); it++)
		{
			layout-> addWidget(it-> second, i / 4, i % 4, Qt::AlignHCenter);
			(it-> second)-> show();
			i++;
		}

		QMenuBar* windowMenuBar(menuBar());
		QMenu* fileMenu(windowMenuBar-> addMenu(tr("&File")));
		QMenu* editMenu(windowMenuBar-> addMenu(tr("&Edit")));
		QMenu* toolMenu(windowMenuBar-> addMenu("&Tool"));
		QMenu* zoomMenu(editMenu-> addMenu(tr("&Zoom")));
		QMenu* configMenu(windowMenuBar-> addMenu(tr("&Configuration")));
		QMenu* helpMenu(windowMenuBar-> addMenu(tr("&Help")));

		QAction* newAction(NULL);
		// QAction* saveAction(NULL);
		QAction* openAction(NULL);
		QAction* opensAction(NULL);
		QAction* quitAction(NULL);
		
		QAction* treeAction(NULL);
		
		QAction* zoomInAction(NULL);
		QAction* zoomOutAction(NULL);
		QAction* zoomFitAction(NULL);
		QAction* renameAction(NULL);
		QAction* moveAction(NULL);
		QAction* removeAction(NULL);
		
		QAction* settingsAction(NULL);
		QAction* closeAction(NULL);
		QAction* abortAction(NULL);
		QAction* stopAction(NULL);
		QAction* resumeAction(NULL);
		QAction* aboutAction(NULL);

		try
		{
			newAction = new QAction(tr("&New chart"), fileMenu);
			saveAction_ = new QAction(tr("&Save chart"), fileMenu);
			openAction = new QAction(tr("&Open chart"), fileMenu);
			opensAction = new QAction(tr("&Open charts"), fileMenu);
			quitAction = new QAction(tr("&Quit"), fileMenu);
			treeAction = new QAction(tr("&Tree view"), toolMenu);
			zoomInAction = new QAction(tr("Zoom &In"), zoomMenu);
			zoomOutAction = new QAction(tr("Zoom &Out"), zoomMenu);
			zoomFitAction = new QAction(tr("Zoom &Fit"), zoomMenu);
			renameAction = new QAction(tr("Re&name"), editMenu);
			moveAction = new QAction(tr("&Move"), editMenu);
			removeAction = new QAction(tr("&Remove"), editMenu);
			settingsAction = new QAction(tr("&Settings"), configMenu);
			closeAction = new QAction(tr("&Close"), configMenu);
			abortAction = new QAction(tr("Abort"), configMenu);
			stopAction = new QAction(tr("Stop"), configMenu);
			resumeAction = new QAction(tr("Resume"), configMenu);
			aboutAction = new QAction(tr("&About"), helpMenu);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::Viewer() : bad allocation of " << ba.what() << std::endl;
			
			return;
		}
		
		newAction-> setStatusTip(tr("Create a new chart"));
		newAction-> setShortcut(QKeySequence(tr("Ctrl+N")));
		newAction-> setVisible(true);
		
		openAction-> setStatusTip(tr("Load a chart"));
		openAction-> setShortcut(QKeySequence(tr("Ctrl+O")));
		openAction-> setVisible(true);
		
		opensAction-> setStatusTip(tr("Load charts"));
		openAction-> setShortcut(QKeySequence(tr("Ctrl+C")));
		opensAction-> setVisible(true);
		
		saveAction_-> setStatusTip(tr("Save the chart"));
		saveAction_-> setShortcut(QKeySequence(tr("Ctrl+S")));
		saveAction_-> setVisible(false);
		
		quitAction-> setStatusTip(tr("Quit the viewer"));
		quitAction-> setShortcut(QKeySequence(tr("Ctrl+Q")));
		quitAction-> setVisible(true);
			
		fileMenu-> addAction(newAction);
		fileMenu-> addAction(openAction);
		fileMenu-> addAction(opensAction);
		fileMenu-> addAction(saveAction_);
		fileMenu-> addAction(quitAction);
		
		treeAction-> setStatusTip(tr("Open tree view"));
		treeAction-> setShortcut(QKeySequence(tr("CTRL+V")));
		treeAction-> setVisible(true);
		
		toolMenu-> addAction(treeAction);
		
		zoomInAction-> setStatusTip(tr("Zoom in"));
		zoomInAction-> setShortcut(QKeySequence(tr("CTRL+I")));
		zoomInAction-> setVisible(true);
		
		zoomOutAction-> setStatusTip(tr("Zoom out"));
		zoomOutAction-> setShortcut(QKeySequence(tr("CTRL+P")));
		zoomOutAction-> setVisible(true);
		
		zoomFitAction-> setStatusTip(tr("Zoom fit"));
		zoomFitAction-> setShortcut(QKeySequence(tr("CTRL+F")));
		zoomFitAction-> setVisible(true);
		
		zoomMenu-> addAction(zoomInAction);
		zoomMenu-> addAction(zoomOutAction);
		zoomMenu-> addAction(zoomFitAction);
		
		renameAction-> setStatusTip(tr("Rename a chart"));
		renameAction-> setShortcut(QKeySequence(tr("Ctrl+R")));
		renameAction-> setVisible(true);
		
		moveAction-> setStatusTip(tr("Move a chart"));
		moveAction-> setShortcut(QKeySequence(tr("Ctrl+M")));
		moveAction-> setVisible(true);
		
		removeAction-> setStatusTip(tr("Remove a chart"));
		removeAction-> setShortcut(QKeySequence(tr("Ctrl+D")));
		removeAction-> setVisible(true);
		
		editMenu-> addAction(renameAction);
		editMenu-> addAction(moveAction);
		editMenu-> addAction(removeAction);
		
		settingsAction-> setStatusTip(tr("Edit settings"));
		settingsAction-> setShortcut(QKeySequence(tr("Ctrl+T")));
		settingsAction-> setVisible(true);
		
		closeAction-> setStatusTip(tr("Close data acquisition"));
		closeAction-> setVisible(true);
		
		abortAction-> setStatusTip("Abort data acquisition");
		abortAction-> setVisible(true);
		
		stopAction-> setStatusTip("Stop data acquisition");
		stopAction-> setVisible(true);
		
		resumeAction-> setStatusTip("Resume data acquisition");
		resumeAction-> setVisible(true);
		
		configMenu-> addAction(settingsAction);
		configMenu-> addAction(closeAction);
		configMenu-> addAction(abortAction);
		configMenu-> addAction(stopAction);
		configMenu-> addAction(resumeAction);
				
		aboutAction-> setStatusTip(tr("About this viewer"));
		aboutAction-> setShortcut(QKeySequence("Ctrl+A"));
		aboutAction-> setVisible(true);
		
		helpMenu-> addAction(aboutAction);
		
		setMouseTracking(true);
		
		QObject::connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));
		QObject::connect(closeAction, SIGNAL(triggered()), this, SLOT(closeAcquisition()));
		QObject::connect(abortAction, SIGNAL(triggered()), this, SLOT(abortAcquisition()));
		QObject::connect(stopAction, SIGNAL(triggered()), this, SLOT(stopAcquisition()));
		QObject::connect(resumeAction, SIGNAL(triggered()), this, SLOT(resumeAcquisition()));
		
		QObject::connect(newAction, SIGNAL(triggered()), this, SLOT(newChart()));
		QObject::connect(openAction, SIGNAL(triggered()), this, SLOT(openChart()));
		QObject::connect(opensAction, SIGNAL(triggered()), this, SLOT(openCharts()));
		QObject::connect(saveAction_, SIGNAL(triggered()), this, SLOT(openSave()));
		QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
		
		QObject::connect(treeAction, SIGNAL(triggered()), this, SLOT(openTree()));
		
		QObject::connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
		QObject::connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
		QObject::connect(zoomFitAction, SIGNAL(triggered()), this, SLOT(zoomFit()));
		
		QObject::connect(renameAction, SIGNAL(triggered()), this, SLOT(openRename()));
		QObject::connect(moveAction, SIGNAL(triggered()), this, SLOT(moveChart()));
		QObject::connect(removeAction, SIGNAL(triggered()), this, SLOT(removeChart()));
		
		QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(openAbout()));
	}

	Viewer::~Viewer()
	{
		cout << "~Viewer" << endl;
	}

	void Viewer::addChart(BaseChart* chart)
	{
		// std::cout << "Viewer::addChart() " << std::endl;
		
		int i(0);
		int j(0);
		int chartNum(0);
		int layoutRows(0);
		int layoutColumns(0);
		bool ok(false);
		QGridLayout* layout(static_cast<QGridLayout *>(chartsWidget_-> layout()));
		layoutRows = layout-> rowCount();
		layoutColumns = layout-> columnCount();
		chartNum = charts_.size();
		pair<map<unsigned int, BaseChart *>::iterator, bool> ret;
		ret = charts_.insert(pair<unsigned int, BaseChart *>(chart-> id(), chart));

		// cout << "The geometry of the viewer is (" << width() << ',' << height() << ')' << endl;

		if(ret.second == false)
		{
			cerr << "The chart to add already exist" << endl;
			delete chart;
			return;
		}
		else
		{
			if(chartNum == 0)
			{
				layout-> addWidget(chart,0,0);
			}
			else
			{
				if(chartNum == layoutRows*layoutColumns)
				{
					if(layoutRows == layoutColumns)
					{
						layout-> addWidget(chart, 0, layoutColumns);
					}
					else
					{
						layout-> addWidget(chart, layoutRows, 0);
					}
				}
				else
				{
					for(i = 0; i < layoutRows; i++)
					{
						for(j = 0; j < layoutColumns; j++)
						{
							if(layout-> itemAtPosition(i,j) == NULL)
							{
								layout-> addWidget(chart, i, j, Qt::AlignHCenter);
								ok = true;
								
								break;
							}

							if(ok == true)
								break;
						}
					}
				}
			}
			
			// TreeItem* parent(model_-> root());
			// TreeItem* child(NULL);
			
			// try
			// {
				// child = new TreeItem(chart, item_);
			// }
			// catch(const std::bad_alloc& ba)
			// {
				// std::cerr << "Viewer::addChart() : bad allocation of " << ba.what() << std::endl;
				
				// return;
			// }
			
			// parent-> append(child);

			chart-> show();
		}

		setCentralWidget(chartsWidget_);
	}

	bool Viewer::isNumber(string str)
	{
		bool dot(false);

		for(string::iterator it = str.begin(); it < str.end(); it++)
		{
			if(it == str.begin())
			{
				if(*it == '-')
				{
					continue;
				}
				else
				{
					if(isdigit(*it))
					{
						continue;
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				if(isdigit(*it))
				{
					continue;
				}

				if(*it == '.')
				{
					if(dot == false)
					{
						dot = true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}

		return true;
	}

	void Viewer::addConnection(ChartConnection* session)
	{
		pair<map<unsigned int, ChartConnection*>::iterator, bool> ret;
		ret = chartConnections_.insert(pair<unsigned int, ChartConnection*>(session-> id(), session));
		
		if(ret.second == false)
		{
			cerr << "This chart ID has already been added" << endl;
		}
	}

	void Viewer::newChart()
	{
		NewDialog* newDialog(NULL);
		
		try
		{
			newDialog = new NewDialog(this, NULL);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::newChart() : " << ba.what() << std::endl;
			
			return;
		}
		catch(...)
		{
			std::cerr << "Viewer::newChart() : Unexpected exception thrown" << std::endl;
			
			return;
		}
		
		newDialog-> show();
		
		QObject::connect(newDialog, SIGNAL(paramValid(string, DataType, 
																Unit, double, Multiple, int, int, unsigned int, 
																Unit, double, Multiple, int, int, unsigned int,
																QColor, QColor)), 
			this, SLOT(createNewChart(string, DataType, 
																Unit, double, Multiple, int, int, unsigned int, 
																Unit, double, Multiple, int, int, unsigned int,
																QColor, QColor)));
	}

	void Viewer::openChart()
	{
		OpenDialog* chartDialog(NULL);
		
		try
		{
			chartDialog = new OpenDialog(this, NULL);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::openChart() : " << ba.what() << std::endl;
			
			return;
		}
		catch(...)
		{
			std::cerr << "Viewer::openChart() : Unexpected exception thrown" << std::endl;
			
			return;
		}
		
		chartDialog-> setWindowTitle(QString("Load a chart"));
		chartDialog-> show();
		
		QObject::connect(chartDialog, SIGNAL(fileSelected(string)), this, SLOT(loadChart(string)));
	}

	void Viewer::openCharts()
	{
		OpenDialog* chartsDialog(NULL);
		
		try
		{
			chartsDialog = new OpenDialog(this, NULL);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::openCharts() : " << ba.what() << std::endl;
			
			return;
		}
		catch(...)
		{
			std::cerr << "Viewer::openCharts() : Unexpected exception thrown" << std::endl;
			
			return;
		}
		
		chartsDialog-> setWindowTitle(QString("Load charts"));
		chartsDialog-> show();
		
		QObject::connect(chartsDialog, SIGNAL(fileSelected(string)), this, SLOT(loadCharts(string)));
	}

	void Viewer::openSave()
	{
		SaveDialog* saveDialog(NULL);
		
		try
		{
			saveDialog = new SaveDialog(this, NULL);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::openCharts() : " << ba.what() << std::endl;
			
			return;
		}
		catch(...)
		{
			std::cerr << "Viewer::openCharts() : Unexpected exception thrown" << std::endl;
			
			return;
		}
		
		saveDialog-> setWindowTitle(QString("Save chart"));
		saveDialog-> show();
		
		QObject::connect(saveDialog, SIGNAL(fileSelected(string)), this, SLOT(saveChart(string)));
	}

	void Viewer::openRename()
	{
		if(activeChart_ != NULL)
		{
			QString title((activeChart_-> title()).c_str());
			
			RenameDialog* renameDialog(NULL);
			
			try
			{
				renameDialog = new RenameDialog(title, this);
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "Viewer::renameChart() : bad allocation " << ba.what() << std::endl;
				
				return;
			}
			
			renameDialog-> show();
			
			QObject::connect(renameDialog, SIGNAL(accepted(QString)), this, SLOT(renameChart(QString)));
		}
		else
		{
			QMessageBox::information(this, tr("Viewer"), tr("No active chart has been set"));
		}
	}

	void Viewer::openSettings()
	{
		SettingsDialog* settingsDialog(NULL);
		
		try
		{
			settingsDialog = new SettingsDialog(this, NULL);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::openSettings() : " << ba.what() << std::endl;
			
			return;
		}
		catch(...)
		{
			std::cerr << "Viewer::openSettings() : Unexpected exception thrown" << std::endl;
			
			return;
		}
		
		settingsDialog-> show();
		
		QObject::connect(settingsDialog, SIGNAL(accepted(QHostAddress, unsigned int, 
											unsigned int, GraphSeries, unsigned int, ProcessID)), 
											this, SLOT(getParameters(QHostAddress, unsigned int, 
											unsigned int, GraphSeries, unsigned int, ProcessID)));
	}

	void Viewer::openTree()
	{
		QDialog* dialog(NULL);
		QVBoxLayout* layout(NULL);
		QPushButton* ok(NULL);
		QTreeView* view(NULL);
		TreeItem* root(NULL);
		TreeModel* model(NULL);
		
		try
		{
			dialog = new QDialog;
			layout = new QVBoxLayout(dialog);
			ok = new QPushButton(QString("Ok"), dialog);
			view = new QTreeView(dialog);
			// root = new TreeItem({QString("Id"), QString("Title"), QString("Type"), QString("Serie"), QString("X quantity"), QString("Y quantity"), QString("X unit"), QString("Y unit"), QString("X multiple"), QString("Y multiple")});
			root = new TreeItem(this, root);
			model = new TreeModel(root, this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::openTree() : bad allocation of " << ba.what() << std::endl;
			
			return;
		}
		
		std::cout << "Viewer::openTree() : root has " << root-> childCount() << " childs" << std::endl;
		
		dialog-> setAttribute(Qt::WA_DeleteOnClose);
		dialog-> setModal(true);
		dialog-> setGeometry(150,150,1050,300);
		layout-> addWidget(view);
		layout-> addWidget(ok);
		
		dialog-> setLayout(layout);
		
		view-> setAttribute(Qt::WA_DeleteOnClose);
		view-> setModel(model);
		
		// view-> setAttribute(Qt::WA_DeleteOnClose);
		// view-> setModel(model_);
		
		view-> show();
		dialog-> show();
		
		QObject::connect(ok, SIGNAL(clicked()), dialog, SLOT(close()));
	}

	void Viewer::openAbout()
	{
		QDialog* dialog(NULL);
		QVBoxLayout* layout(NULL);
		QLabel* label(NULL);
		QPushButton* ok(NULL);
		
		try
		{
			dialog = new QDialog(this);
			layout = new QVBoxLayout(dialog);
			label = new QLabel(QString("This signal viewer was developped by KUOCH Johnny,\n a student in Master 2 of electronics\n at the University Sorbonne-Universite \n during his final internship\n for the company Open-Cell project\n directed by Laurent THOMAS"));
			ok = new QPushButton(QString("Ok"), dialog);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::openAbout() : bad allocation of " << ba.what() << std::endl;
			
			return;
		}
		
		label-> setAlignment(Qt::AlignHCenter);
		dialog-> setAttribute(Qt::WA_DeleteOnClose);
		
		// textEdit-> setReadOnly(true);
		
		dialog-> setLayout(layout);
		layout-> addWidget(label);
		layout-> addWidget(ok);
		
		dialog-> show();
		
		QObject::connect(ok, SIGNAL(clicked()), dialog, SLOT(close()));
	}
	
	void Viewer::command(string com)
	{
		Q_UNUSED(com)
	}

	void Viewer::createNewChart(string title, DataType type, 
														Unit xUnit, double xStep, Multiple xMultiple, int xm, int xM, unsigned int xDivision, 
														Unit yUnit, double yStep, Multiple yMultiple, int ym, int yM, unsigned int yDivision,
														QColor minColor, QColor maxColor)
	{
		// cout << "Creating a new chart" << endl;
		// std::cout << "Viewer::createNewChart() : The title is " << title << std::endl;
		std::cout << "xM = " << xM << std::endl;
		std::cout << "yM = " << yM << std::endl;
		
		Quantity xQuantity(unitToQuantity(xUnit));
		Quantity yQuantity(unitToQuantity(yUnit));
		
		switch(type)
		{
			case UndefinedType :
			{
				std::cerr << "The type is undefined" << std::endl;
				
				break;
			}
			
			case DataRateType :
			{
				DataRateChart* chart(NULL);
				
				try
				{
					chart = new DataRateChart(this, title, chartsWidget_, xStep, yStep, xM, yM, xDivision, yDivision);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				
				addChart(chart);

				break;
			}

			case FrequencyResponseType :
			{
				FrequencyResponseChart* chart(NULL);
				
				try
				{
					chart = new FrequencyResponseChart(this, title, chartsWidget_, xStep, yStep, xM, yM, xDivision, yDivision);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				yQuantity = Gain;
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}
			
			case ImpulseResponseType :
			{
				ImpulseResponseChart* chart(NULL);
				
				try
				{
					chart = new ImpulseResponseChart(this, title, chartsWidget_, xStep, yStep, xM, yM, xDivision, yDivision);;
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				yQuantity = Gain;
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}

			case LikelihoodType :
			{
				LikelihoodChart* chart(NULL);
				
				try
				{
					chart = new LikelihoodChart(this, title, chartsWidget_, xStep, yStep, xM, yM, xDivision, yDivision);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);

				break;
			}

			case IQType :
			{
				IQChart* chart(NULL);
				
				try
				{
					chart = new IQChart(this, title, chartsWidget_, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}

			case ScatterType :
			{
				ScatterChart* chart(NULL);
				
				try
				{
					chart = new ScatterChart(this, title, chartsWidget_, xStep, yStep, xm, xM, ym, yM, xDivision, yDivision, true);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}

			case SerialType :
			{
				SerialChart* chart(NULL);
				
				try
				{
					chart = new SerialChart(this, title, chartsWidget_, xStep, yStep, 0, xM, 0, yM, xDivision, yDivision, true);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}

			case SignalTimeType :
			{
				SignalTimeChart* chart(NULL);
				
				try
				{
					chart = new SignalTimeChart(this, title, chartsWidget_, xStep, yStep, xM, yM, xDivision, yDivision);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}

			case SpectrumType :
			{
				SpectrumChart* chart(NULL);
				
				try
				{
					chart = new SpectrumChart(this, title, chartsWidget_, xStep, yStep, xM, yM, xDivision, yDivision);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
				}
				
				chart-> setTitle(title);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}
			
			case SignalTimeWaterfallType :
			{
				Graph* graph(NULL);
				
				WaterfallChart *chart(new WaterfallChart(this, title, 1, 400, 1000, minColor, maxColor, true, this));
		
				try
				{
					graph = new Graph(xQuantity, yQuantity, xUnit, yUnit, xMultiple, yMultiple, 0);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
					
					return;
				}
				
				chart-> setTitle(title);
				chart-> setGraph(graph);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}

			case WaterfallType :
			{
				Graph* graph(NULL);
				
				WaterfallChart *chart(new WaterfallChart(this, title, 1, 400, 1000, minColor, maxColor, false, this));
		
				try
				{
					graph = new Graph(xQuantity, yQuantity, xUnit, yUnit, xMultiple, yMultiple, 0);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::createNewChart() : " << ba.what() << std::endl;
					
					return;
				}
				
				chart-> setTitle(title);
				chart-> setGraph(graph);
				chart-> setXQuantity(xQuantity);
				chart-> setYQuantity(yQuantity);
				chart-> setXUnit(xUnit);
				chart-> setYUnit(yUnit);
				chart-> setXMultiple(xMultiple);
				chart-> setYMultiple(yMultiple);
				addChart(chart);
				
				break;
			}
		}

		return;
	}

	bool Viewer::saveChart(string filename)
	{
		if(activeChart_ == NULL)
		{
			QMessageBox error;
			error.setText(QString("No chart selected"));
			error.exec();
			return false;
		}
		else
		{
			std::fstream file(filename, std::fstream::out);
			
			if(not file.good())
			{
				std::cerr << "Viewer::saveChart() : Failed to open the file " << filename << std::endl;
				
				return false;
			}
			else
			{
				saveChart(file);
				
				return true;
			}
		}
	}
	
	bool Viewer::saveChart(std::fstream& file)
	{
		if(not file.good())
		{
			return false;
		}
		else
		{
			if(activeChart_)
			{
				char* locale;
		
				locale = getenv("LANG");
				
				file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
				
				file << "<chart locale=\"" << locale << "\">" << std::endl;
				
				if(activeChart_-> toXml(file))
				{
					file << "</chart>" << std::endl;
				}
			}
			else
			{
				return false;
			}
		}
		
		file.close();
		
		return true;
	}

	// bool Viewer::savingChart(string filename)
	// {
		// if(activeChart_ == NULL)
		// {
			// QMessageBox error;
			// error.setText(QString("No chart selected"));
			// error.exec();
			// return false;
		// }
		// else
		// {
			// QFile file(filename.c_str());
			// bool open = file.open(QIODevice::WriteOnly);

			// if(not open)
			// {
				// cerr << "The file " << filename << " has failed to be opened in Write only mode" << endl;
				// return false;
			// }

			// return true;
		// }
	// }

	void Viewer::renameChart(QString qtitle)
	{
		std::string title(qtitle.toStdString());
		
		activeChart_-> setTitle(title);
	}
	
	bool Viewer::loadChart(string filename)
	{
		xmlDocPtr reader = xmlParseFile(filename.c_str());

		if (!reader)
		{
			cerr << "The file hasn't been found or isn't readable" << endl;
			return false;
		}

		xmlXPathContextPtr xpathCtx = xmlXPathNewContext(reader);

		if(!xpathCtx)
		{
			cerr << "Error: unable to create new XPath context\n" << endl;
			return false;
		}

		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "/chart", xpathCtx);

		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "Can't read path: /chart" << endl;
			xmlFreeDoc(reader);
			return false;
		}

		xmlNodeSetPtr nodeset=xpathObj->nodesetval;
		xpathCtx->node=nodeset->nodeTab[0];
		bool ret=loadChart(reader,xpathCtx);
		xmlXPathFreeContext(xpathCtx);
		
		xmlFreeDoc(reader);
		
		return ret;
	}

	bool Viewer::loadChart(xmlDocPtr reader, xmlXPathContextPtr xpathCtx)
	{
		string slocale = xmlGetAttr(reader, xpathCtx, "./attribute::locale");
		// string sfamily = xmlGetAttr(reader, xpathCtx, "./attribute::family");

		if((slocale == UNDEFINED) or (slocale.empty()))
		{
			xmlXPathFreeContext(xpathCtx);
			return false;
		}

		string com("locale -a | grep -q '^" + slocale + "$'");
		int exist(system(com.c_str()));

		if(exist == 0)
		{
			// cout << "The locale has been found" << endl;
			setenv("LANG", slocale.c_str(), 1);
			std::locale::global(std::locale(slocale));
		}
		else
		{
			// cerr << "The locale hasn't been found" << endl;
			xmlXPathFreeContext(xpathCtx);
			return false;
		}

		// string sfamily=xmlGetAttr(reader, xpathCtx, "./attribute::family");

		// if (sfamily == UNDEFINED)
		// {
			// xmlXPathFreeContext(xpathCtx);
			// return false;
		// }
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "./base", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "Can't read path: ./base" << endl;
			
			return false;
		}

		xmlNodeSetPtr nodeset = xpathObj->nodesetval;
		xpathCtx->node = nodeset->nodeTab[0];
		
		bool ret=loadBase(reader,xpathCtx);
		// xmlXPathFreeContext(xpathCtx);
		
		return ret;
	}
	
	bool Viewer::loadCharts(string filename)
	{
		xmlDocPtr reader = xmlParseFile(filename.c_str());

		if(!reader)
		{
			cerr << "The file hasn't been found or isn't readable" << endl;
			return false;
		}

		xmlXPathContextPtr xpathCtx = xmlXPathNewContext(reader);

		if(!xpathCtx)
		{
			cerr << "Error: unable to create new XPath context\n" << endl;
			return false;
		}

		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "/charts", xpathCtx);

		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "Can't read path: /charts" << endl;
			xmlFreeDoc(reader);
			return false;
		}

		xmlNodeSetPtr nodeset=xpathObj->nodesetval;
		xpathCtx->node=nodeset->nodeTab[0];
		xpathObj = xmlXPathEval(BAD_CAST "./chart", xpathCtx);

		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "No chart in a multi-chart xml" << endl;
			return false;
		}

		xmlNodeSetPtr nodeset2=xpathObj->nodesetval;

		for (int i=0; i < nodeset2->nodeNr ; i++ )
		{
			xpathCtx->node=nodeset2->nodeTab[i];

			if ( !loadChart(reader,xpathCtx) )
			{
				cerr << "A chart loading failed" <<endl;
			}
		}

		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(reader);
		return true;
	}

	bool Viewer::loadBase(xmlDocPtr reader, xmlXPathContextPtr xpathCtx)
	{
		string sid(xmlGetAttr(reader, xpathCtx, "./attribute::id"));
		string stitle(xmlGetAttr(reader, xpathCtx, "./attribute::title"));
		string sxstep(xmlGetAttr(reader, xpathCtx, "./attribute::xstep"));
		string systep(xmlGetAttr(reader, xpathCtx, "./attribute::ystep"));
		string sxm(xmlGetAttr(reader, xpathCtx, "./attribute::xm"));
		string sxM(xmlGetAttr(reader, xpathCtx, "./attribute::xM"));
		string sym(xmlGetAttr(reader, xpathCtx, "./attribute::ym"));
		string syM(xmlGetAttr(reader, xpathCtx, "./attribute::yM"));
		string sxdiv(xmlGetAttr(reader, xpathCtx, "./attribute::xdiv"));
		string sydiv(xmlGetAttr(reader, xpathCtx, "./attribute::ydiv"));
		string sxqty(xmlGetAttr(reader, xpathCtx, "./attribute::xqty"));
		string syqty(xmlGetAttr(reader, xpathCtx, "./attribute::yqty"));
		string sxunit(xmlGetAttr(reader, xpathCtx, "./attribute::xunit"));
		string syunit(xmlGetAttr(reader, xpathCtx, "./attribute::yunit"));
		string sxmultiple(xmlGetAttr(reader, xpathCtx, "./attribute::xmultiple"));
		string symultiple(xmlGetAttr(reader, xpathCtx, "./attribute::ymultiple"));
		string stype(xmlGetAttr(reader, xpathCtx, "./attribute::type"));
		
		unsigned int id;
		double xstep;
		double ystep;
		unsigned int xm;
		unsigned int xM;
		unsigned int ym;
		unsigned int yM;
		unsigned int xdiv;
		unsigned int ydiv;
		Quantity xqty;
		Quantity yqty;
		Unit xunit;
		Unit yunit;
		Multiple xmultiple;
		Multiple ymultiple;
		DataType type;
		
		try
		{
			id = static_cast<unsigned int>(stoi(sid));
			xstep = static_cast<double>(stod(sxstep));
			ystep = static_cast<double>(stod(systep));
			xm = stoi(sxm);
			xM = stoi(sxM);
			ym = stoi(sym);
			yM = stoi(syM);
			xdiv = static_cast<unsigned int>(stoi(sxdiv));
			ydiv = static_cast<unsigned int>(stoi(sydiv));
			xqty = stringQuantity(sxqty);
			yqty = stringQuantity(syqty);
			xunit = stringUnit(sxunit);
			yunit = stringUnit(syunit);
			xmultiple = stringMultiple(sxmultiple);
			ymultiple = stringMultiple(symultiple);
			type = stringDataType(stype);
		}
		catch(const std::invalid_argument& ia)
		{
			std::cerr << "Viewer::loadBase() : invalid_argument for " << ia.what() << std::endl;
			
			return false;
		}
		catch(...)
		{
			std::cerr << "Viewer::loadBase() : An unknown exception occured" << std::endl;
			
			return false;
		}
		
		std::cout << "The type is " << dataTypeString(type) << std::endl;
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "../curve", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			// cerr << "Can't read path: ./curve" << endl;
			// xmlFreeDoc(reader);
			// return false;
			
			xpathObj = xmlXPathEval(BAD_CAST "../waterfall", xpathCtx);
			
			if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
			{
				std::cerr << "Can't read path: ../waterfall either" << std::endl;
				
				// xmlFreeDoc(reader);
				return false;
			}
			else
			{
				xmlNodeSetPtr nodeset = xpathObj->nodesetval;
				xpathCtx->node = nodeset->nodeTab[0];
				
				bool ret = loadWaterfall(reader, xpathCtx, id, stitle, xqty, yqty, xunit, yunit, xmultiple, ymultiple);
				
				// xmlXPathFreeContext(xpathCtx);
				// xmlFreeDoc(reader);
				
				return ret;
			}
		}
		else
		{
			xmlNodeSetPtr nodeset = xpathObj->nodesetval;
			xpathCtx->node = nodeset->nodeTab[0];
			
			bool ret=loadCurve(reader, xpathCtx, id, stitle, xstep, ystep, xm, xM, ym, yM, xdiv, ydiv, xqty, yqty, xunit, yunit, xmultiple, ymultiple, type);
			// xmlXPathFreeContext(xpathCtx);
			// xmlFreeDoc(reader);
			
			return ret;
		}
	}
	
	bool Viewer::loadCurve(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, double xstep, double ystep, int xm,
							int xM, int ym, int yM, unsigned int xdiv, unsigned int ydiv,
							Quantity xqty, Quantity yqty, Unit xunit, Unit yunit, Multiple xmultiple, 
							Multiple ymultiple, DataType type)
	{
		// std::cout << "Viewer::loadCurve()" << std::endl;
		string sfamily=xmlGetAttr(reader, xpathCtx, "./attribute::family");

		if ((sfamily != "serial") and (sfamily != "scatter"))
		{
			cerr << "call loadCurve on another family:" << sfamily << endl;
			return false;
		}

		switch(type)
		{
			case DataRateType:
			case FrequencyResponseType:
			case ImpulseResponseType:
			case LikelihoodType:
			case SerialType:
			case SignalTimeType:
			case SpectrumType:
			{
				return loadSerial(reader, xpathCtx, id, stitle, xstep, ystep, xm, xM, ym, yM, xdiv, ydiv, xqty, yqty, xunit, yunit, xmultiple, ymultiple, type);
			}
			
			case ScatterType:
			case IQType:
			{
				return loadScatter(reader, xpathCtx, id, stitle, xstep, ystep, xm, xM, ym, yM, xdiv, ydiv, xqty, yqty, xunit, yunit, xmultiple, ymultiple, type);
			}
			
			default:
			{
				std::cerr << "Viewer::loadCurve() : Not the right family" << std::endl;
				
				return false;
			}
		}
	}

	bool Viewer::loadSerial(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, double xstep, double ystep, int xm,
							int xM, int ym, int yM, unsigned int xdiv, unsigned int ydiv,
							Quantity xqty, Quantity yqty, Unit xunit, Unit yunit, Multiple xmultiple, 
							Multiple ymultiple, DataType type)
	{
		// std::cout << "Viewer::loadSerial() V2" << std::endl;
		
		string sfamily=xmlGetAttr(reader, xpathCtx, "./attribute::family");

		if (! noCaseEqual(sfamily,"serial"))
		{
			cerr << "call loadSerial on another family:" << sfamily << endl;
			return false;
		}
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "./graphes", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "Can't read path: ./graphes" << endl;
			// xmlFreeDoc(reader);
			return false;
		}

		xmlNodeSetPtr nodeset = xpathObj->nodesetval;
		xpathCtx->node = nodeset->nodeTab[0];
		
		switch(type)
		{
			case DataRateType:
			{
				DataRateChart* chart(NULL);
				
				try
				{
					chart = new DataRateChart(this, stitle, chartsWidget_, xstep, ystep, xM, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what() << std::endl;
					
					return false;
				}
				
				chart-> setID(id);
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			case FrequencyResponseType:
			{
				// std::cout << "The type is frequency" << std::endl;
				FrequencyResponseChart* chart(NULL);
				
				try
				{
					chart = new FrequencyResponseChart(this, stitle, chartsWidget_, xstep, ystep, xM, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what() << std::endl;
					
					return false;
				}
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			case ImpulseResponseType:
			{
				ImpulseResponseChart* chart(NULL);
				
				try
				{
					chart = new ImpulseResponseChart(this, stitle, chartsWidget_, xstep, ystep, xM, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what()<< std::endl;
					
					return false;
				}
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			case LikelihoodType:
			{
				LikelihoodChart* chart(NULL);
				
				try
				{
					chart = new LikelihoodChart(this, stitle, chartsWidget_, xstep, ystep, xM, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what()<< std::endl;
					
					return false;
				}
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			case SerialType:
			{
				SerialChart* chart(NULL);
				
				try
				{
					chart = new SerialChart(this, stitle, chartsWidget_, xstep, ystep, xm, xM, ym, yM, xdiv, ydiv, true);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what()<< std::endl;
					
					return false;
				}
				
				chart-> setID(id);
				chart-> setXQuantity(xqty);
				chart-> setYQuantity(yqty);
				chart-> setXUnit(xunit);
				chart-> setYUnit(yunit);
				chart-> setXMultiple(xmultiple);
				chart-> setYMultiple(ymultiple);
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			case SignalTimeType:
			{
				SignalTimeChart* chart(NULL);
				
				try
				{
					chart = new SignalTimeChart(this, stitle, chartsWidget_, xstep, ystep, xM, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what()<< std::endl;
					
					return false;
				}
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			case SpectrumType:
			{
				SpectrumChart* chart(NULL);
				
				try
				{
					chart = new SpectrumChart(this, stitle, chartsWidget_, xstep, ystep, xM, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what()<< std::endl;
					
					return false;
				}
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
					chart-> replot();
				}
				
				return ret;
				
				break;
			}
			
			default :
			{
				return false;
			}
		}
	}

	bool Viewer::loadScatter(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string stitle, double xstep, double ystep, int xm,
							int xM, int ym, int yM, unsigned int xdiv, unsigned int ydiv,
							Quantity xqty, Quantity yqty, Unit xunit, Unit yunit, Multiple xmultiple, 
							Multiple ymultiple, DataType type)
	{
		string sfamily=xmlGetAttr(reader, xpathCtx, "./attribute::family");

		if (! noCaseEqual(sfamily,"scatter"))
		{
			cerr << "call loadSerial on another family:" << sfamily << endl;
			return false;
		}
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "./graphes", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "Can't read path: ./graphes" << endl;
			// xmlFreeDoc(reader);
			return false;
		}

		xmlNodeSetPtr nodeset = xpathObj->nodesetval;
		xpathCtx->node = nodeset->nodeTab[0];
		
		switch(type)
		{
			case IQType:
			{
				IQChart* chart(NULL);
				
				try
				{
					chart = new IQChart(this, stitle, chartsWidget_, xstep, ystep, xm, xM, ym, yM, xdiv, ydiv);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what() << std::endl;
					
					return false;
				}
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
				}
				
				return ret;
				
				break;
			}
			
			case ScatterType:
			{
				ScatterChart* chart(NULL);
				
				try
				{
					chart = new ScatterChart(this, stitle, chartsWidget_, xstep, ystep, xm, xM, ym, yM, xdiv, ydiv, true);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "Viewer::loadSerial() : " << ba.what() << std::endl;
					
					return false;
				}
				
				chart-> setID(id);
				chart-> setXQuantity(xqty);
				chart-> setYQuantity(yqty);
				chart-> setXUnit(xunit);
				chart-> setYUnit(yunit);
				chart-> setXMultiple(xmultiple);
				chart-> setYMultiple(ymultiple);
				
				bool ret = loadGraphes(reader, xpathCtx, chart);
				
				if(ret == true)
				{
					addChart(chart);
				}
				
				return ret;
				
				break;
			}
			
			default :
			{
				return false;
			}
		}
		
		return true;
	}

	bool Viewer::loadWaterfall(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							unsigned int id, string title, Quantity xqty, Quantity yqty, 
							Unit xunit, Unit yunit, Multiple xmultiple, Multiple ymultiple)
	{
		cout << "Viewer::loadWaterfall()" << endl;
		
		std::string stime(xmlGetAttr(reader, xpathCtx, "./attribute::time"));
		std::string srows(xmlGetAttr(reader, xpathCtx, "./attribute::rows"));
		std::string scolumns(xmlGetAttr(reader, xpathCtx, "./attribute::columns"));
		// std::string smin(xmlGetAttr(reader, xpathCtx, "./attribute::min"));
		// std::string smax(xmlGetAttr(reader, xpathCtx, "./attribute::max"));
		std::string smincolor(xmlGetAttr(reader, xpathCtx, "./attribute::mincolor"));
		std::string smaxcolor(xmlGetAttr(reader, xpathCtx, "./attribute::maxcolor"));
		
		double time;
		unsigned int rows;
		unsigned int columns;
		// double min;
		// double max;
		QColor mincolor;
		QColor maxcolor;
		
		try
		{
			time = stod(stime);
			rows = static_cast<unsigned int>(stoi(srows));
			columns = static_cast<unsigned int>(stoi(scolumns));
			// min = stod(smin);
			// max = stod(smax);
			mincolor = QColor(stringToColor(smincolor));
			maxcolor = QColor(stringToColor(smaxcolor));
		}
		catch(const std::invalid_argument& ia)
		{
			std::cerr << "Viewer::loadWaterfall() : " << ia.what() << std::endl;
			
			return false;
		}
		catch(...)
		{
			std::cerr << "Viewer::loadWaterfall() : An exception occured" << std::endl;
			
			return false;
		}
		
		WaterfallChart* chart(NULL);
		Graph* graph(NULL);
		
		try
		{
			// graph = new Graph(xqty, yqty, xunit, yunit, xmultiple, ymultiple, 0);
			chart = new WaterfallChart(this, title, time, rows, columns, mincolor, maxcolor, chartsWidget_);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::loadWaterfall() : " << ba.what() << std::endl;
			
			return false;
		}
		
		graph = chart-> graph();
		graph-> setXQuantity(xqty);
		graph-> setYQuantity(yqty);
		graph-> setXUnit(xunit);
		graph-> setYUnit(yunit);
		graph-> setXMultiple(xmultiple);
		graph-> setYMultiple(ymultiple);
		graph-> setSerie(0);
		
		chart-> setXQuantity(xqty);
		chart-> setYQuantity(yqty);
		chart-> setXUnit(xunit);
		chart-> setYUnit(yunit);
		chart-> setXMultiple(xmultiple);
		chart-> setYMultiple(ymultiple);
		
		QwtPlotSpectrogram::DisplayMode display;
		
		std::string sdisplay(xmlGetAttr(reader, xpathCtx, "./plot/attribute::display"));
		
		try
		{
			display = WaterfallChart::stringDisplayMode(sdisplay);
		}
		catch(const std::invalid_argument& ia)
		{
			std::cerr << "Viewer::loadWaterfall() : " << ia.what() << std::endl;
			
			return false;
		}
		
		chart-> setID(id);
		chart-> setDisplayMode(display);
		// chart-> setGraph(graph);
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "./graph", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "Can't read path: ./graph" << endl;
			// xmlFreeDoc(reader);
			return false;
		}

		xmlNodeSetPtr nodeset = xpathObj->nodesetval;
		xpathCtx->node = nodeset->nodeTab[0];
		
		if(loadGraph(reader, xpathCtx, graph))
		{
			addChart(chart);
			
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool Viewer::loadGraph(xmlDocPtr reader, xmlXPathContextPtr xpathCtx, Graph* graph)
	{
		string sxqty(xmlGetAttr(reader, xpathCtx, "./attribute::xqty"));
		string syqty(xmlGetAttr(reader, xpathCtx, "./attribute::yqty"));
		string sxunit(xmlGetAttr(reader, xpathCtx, "./attribute::xunit"));
		string syunit(xmlGetAttr(reader, xpathCtx, "./attribute::yunit"));
		string sxmultiple(xmlGetAttr(reader, xpathCtx, "./attribute::xmultiple"));
		string symultiple(xmlGetAttr(reader, xpathCtx, "./attribute::ymultiple"));
		string sserie(xmlGetAttr(reader, xpathCtx, "./attribute::serie"));
		
		Quantity xqty;
		Quantity yqty;
		Unit xunit;
		Unit yunit;
		Multiple xmultiple;
		Multiple ymultiple;
		unsigned int serie;
		
		try
		{
			xqty = stringQuantity(sxqty);
			yqty = stringQuantity(syqty);
			xunit = stringUnit(sxunit);
			yunit = stringUnit(syunit);
			xmultiple = stringMultiple(sxmultiple);
			ymultiple = stringMultiple(symultiple);
			serie = static_cast<unsigned int>(stoi(sserie));
		}
		catch(const std::invalid_argument& ia)
		{
			std::cerr << "Viewer::loadGraph() : " << ia.what() << std::endl;

			return false;			
		}
		catch(...)
		{
			std::cerr << "Viewer::loadGraph() : An exception occured" << std::endl;
			
			return false;
		}
		
		graph-> setXQuantity(xqty);
		graph-> setYQuantity(yqty);
		graph-> setXUnit(xunit);
		graph-> setYUnit(yunit);
		graph-> setXMultiple(xmultiple);
		graph-> setYMultiple(ymultiple);
		graph-> setSerie(serie);
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "./point", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			cerr << "No points to load" << endl;
			
			return true;
		}
		else
		{
			xmlNodeSetPtr nodeset=xpathObj->nodesetval;

			for (int i=0; i < nodeset->nodeNr ; i++ )
			{
				graph-> addData(readPoint(nodeset, i));
				std::cout << "Data loaded" << std::endl;
			}
			
			return true;
		}
	}
	
	bool Viewer::loadCurveGraph(xmlDocPtr reader, xmlXPathContextPtr xpathCtx,
							CurveChart* chart)
	{
		CurveGraph*curvegraph(NULL);
		
		try
		{
			curvegraph = new CurveGraph;
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::loadCurveGraph() : " << ba.what() << std::endl;
			
			return false;
		}
		
		std::string sserie(xmlGetAttr(reader, xpathCtx, "./attribute::serie"));
		std::string sstyle(xmlGetAttr(reader, xpathCtx, "./attribute::style"));
		
		unsigned int serie(static_cast<unsigned int>(stoi(sserie)));
		QwtPlotCurve::CurveStyle style(stringToStyle(sstyle));
		
		curvegraph-> setSerie(serie);
		curvegraph-> setStyle(style);
		
		xmlXPathObjectPtr xpathObjPen = xmlXPathEval(BAD_CAST "./pen", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObjPen->nodesetval))
		{
			std::cerr << "Can't read path ./pen" << std::endl;
			
			return false;
		}
		else
		{
			xmlNodeSetPtr nodeset=xpathObjPen->nodesetval;
			
			xmlXPathContextPtr xpathCtxPen(xmlXPathNewContext(reader));
			
			xpathCtxPen-> node = nodeset-> nodeTab[0];
			
			std::string spenwidth(xmlGetAttr(reader, xpathCtxPen, "./attribute::width"));
			std::string spencolor(xmlGetAttr(reader, xpathCtxPen, "./attribute::color"));
			std::string spenstyle(xmlGetAttr(reader, xpathCtxPen, "./attribute::style"));
			
			unsigned int penwidth;
			Qt::GlobalColor pencolor;
			Qt::PenStyle penstyle;
			
			try
			{
				penwidth = static_cast<unsigned int>(stoi(spenwidth));
				pencolor = stringToColor(spencolor);
				penstyle = stringToPenStyle(spenstyle);
			}
			catch(const std::invalid_argument& ia)
			{
				std::cerr << "Viewer::loadCurveGraph() : " << ia.what() << std::endl;
				
				return false;
			}
			
			QPen* pen(NULL);
			
			try
			{
				pen = new QPen(pencolor);
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "Viewer::loadCurveGraph() : " << ba.what() << std::endl;
				
				return false;
			}
			
			pen-> setWidth(penwidth);
			pen-> setStyle(penstyle);
			
			std::string ssymbolStyle(xmlGetAttr(reader, xpathCtx, "./symbol/attribute::style"));
			std::string ssymbolHeight(xmlGetAttr(reader, xpathCtx, "./symbol/attribute::height"));
			std::string ssymbolWidth(xmlGetAttr(reader, xpathCtx, "./symbol/attribute::width"));
			
			QwtSymbol::Style symbolStyle;
			unsigned int symbolHeight;
			unsigned int symbolWidth;
			
			try
			{
				symbolStyle = stringToSymbol(ssymbolStyle);
				symbolHeight = static_cast<unsigned int>(stoi(ssymbolHeight));
				symbolWidth = static_cast<unsigned int>(stoi(ssymbolWidth));
			}
			catch(const std::invalid_argument& ia)
			{
				std::cerr << "Viewer::loadCurveGraph() : " << ia.what() << std::endl;
				
				return false;
			}
			
			QwtSymbol* symbol(NULL);
			
			try
			{
				symbol = new QwtSymbol(symbolStyle, QBrush(pencolor), *pen, QSize(symbolWidth, symbolHeight));
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "Viewer::loadCurveGraph() : " << ba.what() << std::endl;
				
				return false;
			}
			
			curvegraph-> setPen(pen);
			
			if(style == QwtPlotCurve::NoCurve)
			{
				curvegraph-> setSymbol(symbol);
			}
		}
		
		xmlXPathObjectPtr xpathObj = xmlXPathEval(BAD_CAST "./graph", xpathCtx);
		
		if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			std::cerr << "Can't read path ./graph" << std::endl;
			
			return false;
		}
		else
		{
			Graph* graph(NULL);
			
			Quantity xqty(chart-> xQuantity());
			Quantity yqty(chart-> yQuantity());
			Unit xunit(chart-> xUnit());
			Unit yunit(chart-> yUnit());
			Multiple xmultiple(chart-> xMultiple());
			Multiple ymultiple(chart-> yMultiple());
			
			try
			{
				graph = new Graph(xqty, yqty, xunit, yunit, xmultiple, ymultiple, serie);
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "Viewer::loadCurveGraph() : " << ba.what() << std::endl;
				
				return false;
			}
			
			curvegraph-> setGraph(graph);
			
			chart-> setCurveGraph(serie, curvegraph);
			
			curvegraph-> attach(chart);
			
			xmlNodeSetPtr nodeset=xpathObj->nodesetval;

			xmlXPathContextPtr xpathCtxGraph(xmlXPathNewContext(reader));
			
			xpathCtxGraph-> node = nodeset-> nodeTab[0];
			
			if(loadGraph(reader, xpathCtxGraph, graph))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	bool Viewer::loadGraphes(xmlDocPtr reader, xmlXPathContextPtr xpathCtx, CurveChart* chart)
	{
		xmlXPathObjectPtr xpathObj(xmlXPathEval(BAD_CAST "./curvegraph", xpathCtx));
		
		if(xmlXPathNodeSetIsEmpty(xpathObj-> nodesetval))
		{
			std::cerr << "Can't read path: ./curvegraph" << std::endl;
			// xmlFreeDoc(reader);
			
			return false;
		}
		else
		{
			xmlNodeSetPtr nodeset(xpathObj-> nodesetval);
			unsigned int unodeNr(static_cast<unsigned int>(nodeset-> nodeNr));
			
			// std::cout << "unodeNr = " << unodeNr << std::endl;
			
			// Quantity xqty(chart-> xQuantity());
			// Quantity yqty(chart-> yQuantity());
			// Unit xunit(chart-> xUnit());
			// Unit yunit(chart-> yUnit());
			// Multiple xmultiple(chart-> xMultiple());
			// Multiple ymultiple(chart-> yMultiple());
			
			for(unsigned int i = 0; i < unodeNr; i++)
			{
				xpathCtx-> node = nodeset-> nodeTab[i];
				
				if(not loadCurveGraph(reader, xpathCtx, chart))
					return false;
			}
			
			return true;
		}
	}
	
	void Viewer::moveChart()
	{
		QMessageBox::information(this, tr("Viewer"), tr("Not implemented yet"));
	}

	void Viewer::removeChart()
	{
		if(activeChart_ != NULL)
		{
			unsigned int id(activeChart_-> id());
			std::map<unsigned int, BaseChart*>::iterator it(charts_.find(id));
			std::map<unsigned int, ChartConnection*>::iterator itc(chartConnections_.find(id));
			charts_.erase(it);
			
			if(itc != chartConnections_.end())
				chartConnections_.erase(itc);
			
			QGridLayout* layout(dynamic_cast<QGridLayout*>(chartsWidget_-> layout()));
			// std::cout << "Before remove, count = " << layout-> count() << std::endl;
			layout-> removeWidget(activeChart_);
			// std::cout << "After remove, count = " << layout-> count() << std::endl;
			activeChart_-> hide();
		}
		else
		{
			QMessageBox::information(this, tr("Viewer"), tr("No active chart has been set"));
		}
	}
	
	ChartConnection* Viewer::createChartConnection(BaseChart* chart, QHostAddress ip, unsigned int port, GraphSeries series, unsigned int numData, ProcessID process)
	{
		std::cout << "Viewer::createChartConnection()" << std::endl;
		
		ChartConnection* session(NULL);
		
		try
		{
			session = new ChartConnection(this, chart, numData, process, this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "Viewer::connection() : bad allocation of " << ba.what() << std::endl;
			
			return NULL;
		}
		
		if(session)
		{
			std::cout << "Viewer::createChartConnection() : The chart connection has been created" << std::endl;
			
			unsigned int id(chart-> id());
			DataType type(chart-> type());
			DataRequest request(id, type, series, numData, process);
			
			session-> createRequestConnection(ip, port, request);
			addConnection(session);
		}
		else
		{
			std::cerr << "Viewer::createChartConnection() : The chart connection hasn't been created" << std::endl;
		}
		
		return session;
	}
	
	void Viewer::getParameters(QHostAddress ip, unsigned int port, unsigned int id, GraphSeries series, unsigned int numData, ProcessID process)
	{
		std::cout << "getParameters()" << endl;
		
		// ip_ = ip;
		// port_ = port,
		// id_ = id;
		// series_ = series;
		// numData_ = numData;
		// process_ = process;
		
		map<unsigned int, BaseChart*>::iterator it(charts_.find(id));
		
		if(it == charts_.end())
		{
			cerr << "The chart " << id << " doesn't exist" << endl;
			
			return;
		}
		else
		{
			std::map<unsigned int, ChartConnection*>::iterator it2(chartConnections_.find(id));
			ChartConnection* session(NULL);
			BaseChart* chart(it-> second);
			
			if(it2 == chartConnections_.end())
			{
				session = createChartConnection(chart, ip, port, series, numData, process);
				
				if(session)
				{
					std::cout << "Viewer::getParameters() : The connection has been established" << std::endl;
				}
				else
				{
					std::cerr << "Viewer::getParameters() : The connection failed to be established" << std::endl;
				}
			}
			else
			{
				unsigned int id(chart-> id());
				DataType type(chart-> type());
				DataRequest request(id, type, series, numData, process);
				
				session = it2-> second;
				
				std::map<unsigned int, RequestConnection*>::iterator it3(session-> find(port));
				
				if(it3 == session-> end())
				{
					session-> createRequestConnection(ip, port, request);
				}
				else
				{
					RequestConnection* requestConnection(it3-> second);
					requestConnection-> updateRequest(request);
				}
			}
		}
	}
	
	void Viewer::closeAcquisition()
	{
		for(map<unsigned int, ChartConnection*>::iterator it = chartConnections_.begin(); it != chartConnections_.end(); it++)
			(it-> second)-> close();
	}
	
	void Viewer::abortAcquisition()
	{
		for(map<unsigned int, ChartConnection*>::iterator it = chartConnections_.begin(); it != chartConnections_.end(); it++)
			(it-> second)-> abort();
	}
	
	void Viewer::resumeAcquisition()
	{
		cout << "Viewer::resumeAcquisition()" << endl;
		
		for(map<unsigned int, ChartConnection*>::iterator it = chartConnections_.begin(); it != chartConnections_.end(); it++)
		{
			ChartConnection* session(it-> second);
			
			if((session-> dataAcquisition()) == false)
			{
				session-> setDataAcquisition(true);
				
				if(not (session-> sendRequests()))
				{
					cerr << "Failed to write on the socket" << endl;
				}
			}
		}
	}
	
	void Viewer::stopAcquisition()
	{
		cout << "Viewer::stopAcquisition()" << endl;
		
		for(map<unsigned int, ChartConnection*>::iterator it = chartConnections_.begin(); it != chartConnections_.end(); it++)
			(it-> second)-> setDataAcquisition(false);
	}

	void Viewer::zoomIn()
	{
		QMessageBox::information(this, tr("Viewer"), tr("Not implemented yet"));
	}

	void Viewer::zoomOut()
	{
		QMessageBox::information(this, tr("Viewer"), tr("Not implemented yet"));
	}

	void Viewer::zoomFit()
	{
		QMessageBox::information(this, tr("Viewer"), tr("Not implemented yet"));
	}

	#define type_name(type) #type,
	static const char *help_names[]=
	{
		foreach_type(type_name)
	};

	static void displayHeader(DataHeader *h)
	{
		// cout << "The type is " << help_names[h-> type()] << endl;

		switch(h->format())
		{
		case FormatUndefined :
			cout << "The format is undefined" << endl;
			break;

		case Short :
			cout << "The format is Short" << endl;
			break;

		case Int :
			cout << "The format is Int" << endl;
			break;

		case Float :
			cout << "The format is Float" << endl;
			break;

		case Double :
			cout << "The format is Double" << endl;
			break;

		default :
			cerr << "The format is unknown" << endl;
		}

		cout << "The number of samples is " << h->number() << endl;
	}

	void Viewer::resizeEvent(QResizeEvent *event)
	{
		Q_UNUSED(event)
	}

	void Viewer::mouseMoveEvent(QMouseEvent *event)
	{
		Q_UNUSED(event)
	}

	void Viewer::mousePressEvent(QMouseEvent *event)
	{
		if(event-> button() == Qt::LeftButton)
		{
			cout << "Viewer::Mouse local position : (" << event-> x() << ',' << event-> y() << ')' << endl;
			// QWidget* widget(app_-> widgetAt(event-> globalX(), event-> globalY()));
			// if(widget == NULL)
			// cout << "The widget is NULL" << endl;
			// setActiveChart(widget);
		}
	}
}
