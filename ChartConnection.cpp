#include "ChartConnection.hpp"

namespace ModelView
{
	RequestConnection::RequestConnection(ChartConnection* chartConnection, QHostAddress ip, unsigned int port, DataRequest request, QObject* parent):
		QObject(parent),
		chartConnection_(chartConnection),
		ip_(ip),
		port_(port),
		socket_(NULL),
		request_(request),
		numLoaded_(0),
		state_(ReadingHeader),
		header_()
	{
		try
		{
			socket_ = new QTcpSocket(this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "RequestConnection::RequestConnection() : bad allocation of " << ba.what() << std::endl;
			
			return;
		}
		
		QObject::connect(socket_, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(socket_, SIGNAL(readyRead()), this, SLOT(readData()));
		QObject::connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this,
						 SLOT(displayError(QAbstractSocket::SocketError)));
	}
	
	RequestConnection::~RequestConnection()
	{}
	
	bool RequestConnection::setSerie(SerieSignal serieSignal)
	{
		GraphSeries series(request_.series());
		unsigned int serie(serieSignal.serie());
		
		unsigned int ret(series.find(serie));
		bool ret2;
		
		if(ret != MAX_SERIE)
		{
			chartConnection_-> setSerie(serieSignal);
			ret2 = request_.setSerie(serieSignal);
		}
		else
		{
			ret2 = addSerie(serieSignal);
		}
		
		std::cout << "RequestConnection::setSerie() : The series of the request are now ";
		
		for(unsigned int i = 0; i < request_.size(); i++)
		{
			std::cout << request_.serie(i) << " ";
		}
		
		std::cout << std::endl;
		
		return ret2;
	}
	
	unsigned int RequestConnection::addSignal(unsigned int signal)
	{
		std::cout << "ChartConnection::addSignal()" << std::endl;
		
		unsigned int i(0);
		std::set<unsigned int> series_tmp;
		GraphSeries series = request_.series();
		std::pair<std::set<unsigned int>::iterator, bool> ret;
		
		// loop to look for the existing series
		
		while(((series.serie(i)).serie() != UNDEFINED_SERIE) and (i < MAX_SERIE))
		{
			ret = series_tmp.insert((series.serie(i)).serie());
			
			if(ret.second == false)
			{
				std::cerr << "ChartConnection::addSignal() : A GraphSeries contains twice, the same serie " << std::endl;
				
				return MAX_SERIE;
			}
			else
			{
				i++;
			}
		}
		
		// if the GraphSeries (list of series) isn't full, we assign the signal to the smallest index serie that doesn't exist
		
		if(i != MAX_SERIE)
		{
			unsigned int j(0);
			std::set<unsigned int>::iterator ret2;
			
			for(j = 0; j < MAX_SERIE; j++)
			{
				ret2 = series_tmp.find(j);
				
				if(ret2 == series_tmp	.end())
					break;
			}
			
			std::cout << "The serie " << j << " isn't assigned yet" << std::endl;
			
			SerieSignal serie(j, signal);
			
			addSerie(serie);
			
			return j;
		}
		else
		{
			return MAX_SERIE;
		}
	}
	
	bool RequestConnection::addSerie(SerieSignal serie)
	{
		/*  
			Before adding a serie, the method check if this serie has already been assigned to 
			another signal from a different server, if it hasn't, then the serie is added on the 
			list of the chart connection then the one of the request connection
		*/
		
		std::cout << "RequestConnection::addSerie() : The size of request_ is " << request_.size() << std::endl;
		
		bool ret;		// bool to keep the return value of ChartConnection::addSerie()
		
		ret = chartConnection_-> addSerie(serie);
		
		if(ret)
		{
			std::cout << "RequestConnection::addSerie() : The serie " << serie.serie() << " has been added to the chart connection" << std::endl;
			
			try
			{
				ret = request_.addSerie(serie);
			}
			catch(const std::out_of_range& oor)
			{
				std::cerr << "ChartConnection::addSerie() : out of range " << oor.what() << std::endl;
				
				return false;
			}
			
			if(ret)
			{
				std::cout << "RequestConnection::addSerie() : The serie has been added to the request" << std::endl;
			}
			else
			{
				std::cerr << "RequestConnection::addSerie() : The serie hasn't been added to the request" << std::endl;
			}
		}
		
		return ret;
	}
	
	/*
		The function addSeries adds series to a RequestConnection.
		It starts by browsing all the SerieSignal of the GraphSeries and check if the serie of 
		SerieSignal already exists, 
	
	*/
	
	void RequestConnection::addSeries(GraphSeries series)
	{
		std::cout << "ChartConnection::addSeries()" << std::endl;
		
		BaseChart* chart(chartConnection_-> chart());
		GraphSeries requestSeries(request_.series());
		
		for(unsigned int i = 0; i < series.end(); i++)
		{
			unsigned int serie((series.serie(i)).serie());
			unsigned int signal((series.serie(i)).signal());
			
			if(addSerie(serie) == false)
			{
				std::cout << "RequestConnection::addSeries() : The serie " << series.serie(i).serie() << " has already been assigned to a signal" << std::endl;
				
				unsigned int serie2(addSignal(signal));
				
				std::cerr << "RequestConnection::addSeries() : The serie " << serie << " already exist" << std::endl;
				std::cout << "RequestConnection::addSeries() : Displaying the signal " << signal << " in another serie" << std::endl;
				
				if(serie2 == MAX_SERIE)
				{
					std::cerr << "RequestConnection::addSeries() : There is no more serie available for the chart " << chart-> id() << std::endl;
				}
				else
				{
					std::cout << "RequestConnection::addSeries() : The signal " << signal << " has been assigned to the serie " << serie2 << std::endl;
					
					// request_.setSeries(series_);
				}
				
				break;
			}
			else
			{
				std::cout << "RequestConnection::addSeries() : The serie " << series.serie(i).serie() << " was still unassigned" << std::endl;
				
				// request_.setSeries(series_);
			}
		}
	}
	
	void RequestConnection::updateRequest(DataRequest request)
	{
		GraphSeries series(request.series());
		GraphSeries series2(request_.series());
		
		for(unsigned int i = 0; i < series.end(); i++)
		{
			SerieSignal serieSignal(series.serie(i));
			unsigned int serie(serieSignal.serie());
			// unsigned int signal(serieSignal.signal());
			
			unsigned int ret(series2.find(serie));
			
			if(ret != MAX_SERIE)
			{
				std::cout << "RequestConnection::updateRequest() : The serie " << serieSignal << " already exist" << std::endl;
				
				if(not setSerie(serieSignal))
				{
					std::cerr << "RequestConnection::updateRequest() : The serie " << serieSignal << " has not been added" << std::endl;
				}
				else
				{
					std::cout << "RequestConnection::updateRequest() : The serie has been changed" << std::endl;
				}
			}
			else
			{
				if(not addSerie(serieSignal))
				{
					std::cerr << "RequestConnection::updateRequest() : The serie " << serieSignal << " has not been added" << std::endl;
				}
			}
		}
		
		// request_.updateRequest(series);
		
		// for(unsigned int i = 0; i < series.end(); i++)
		// {
			// SerieSignal serie(series.serie(i));
			// unsigned int index(serie.serie());
			// unsigned int signal(serie.signal());
			
			// std::cout << "RequestConnection::updateRequest() : The serie " << serie;
			
			// if(ret)
			// {
				// std::cout << " has been added" << std::endl;
			// }
			// else
			// {
				// std::cout << " hasn't been added" << std::endl;
			// }
		// }
	}
	
	bool RequestConnection::fullwrite(char* buff, unsigned int maxSize)
	{
		// cout << "ChartConnection::fullwrite()" << endl;
		
		char* buf(buff);
		int n;
		
		while(maxSize)
		{
			n = socket_-> write(buf, maxSize);
			
			if(n <= 0)
				return false;
			
			maxSize -= n;
			buf += n;
			
			// cout << n << " bytes have been written" << endl;
			// cout << maxSize << " bytes remainings" << endl; 
		}
		
		return true;
	}
	
	bool RequestConnection::fullread(char* buff, unsigned int maxSize)
	{
		char* buf(buff);
		int n;
		
		while(maxSize)
		{
			n = socket_-> read(buf, maxSize);
			
			if(n <= 0)
			{
				if(n == 0)
				{
					// cout << "No more available data" << endl;
				}
				
				if(n == -1)
				{
					// cout << "The socket is closed or the process has died" << endl;
				}
				
				return false;
			}
			
			maxSize -= n;
			buf += n;
			
			// cout << "read " << n << " bytes" << endl;
			// cout << "remains " << maxSize << endl;
		}
		
		return true;
	}
	
	void RequestConnection::connected()
	{
		std::cout << "The connection has been established" << endl;
		std::cout << "The socket descriptor is " << (socket_-> socketDescriptor()) << std::endl;
		chartConnection_-> setDataAcquisition(true);
		// chartConnection_-> addConnection(this);
		
		if(not fullwrite((char*) &request_, sizeof(request_)))
			return;
		
		// panel_-> show();
		
		std::cout << "The sent data request is " << std::endl;
		std::cout << "id = " << request_.id() << std::endl;
		std::cout << "type = " << request_.type() << std::endl;
		std::cout << "series.end() = " << (request_.series()).end() << std::endl;
		std::cout << "series = ";
		
		for(unsigned int i = 0; i < (request_.series()).end(); i++)
		{
			if(i != 0)
			{
				std::cout << ',';
			}
			
			std::cout << ((request_.series()).serie(i)).serie();
		}
		
		std::cout << "numData = " << request_.numData() << std::endl;
		std::cout << "process = " << request_.processID() << std::endl;
		
		// std::cout << "The size of the chart is (" << (chart_-> width()) << ',' << (chart_-> height()) << ')' << std::endl;
		// std::cout << "The size of the widget is (" << (panel_-> width()) << ',' << (panel_-> height()) << ')' << std::endl;
	}
	
	void RequestConnection::displayError(QAbstractSocket::SocketError socketError)
	{
		std::cout << "RequestConnection::displayError() " << std::endl;
		
		Viewer* viewer(chartConnection_-> viewer());
		
		switch (socketError)
		{
			case QAbstractSocket::RemoteHostClosedError:
				break;

			case QAbstractSocket::HostNotFoundError:
				QMessageBox::information(viewer, tr("Viewer"), tr("The host was not found. Please check the " "host name and port settings."));
				break;

			case QAbstractSocket::ConnectionRefusedError:
				QMessageBox::information(viewer, tr("Viewer"), tr("The connection was refused by the peer. " "Be sure the server is running " "and check that the host name and port " "settings are correct."));
				break;

			default:
				QMessageBox::information(viewer, tr("Viewer"), tr("The following error occurred: %1.").arg(socket_->errorString()));
		}
	}
	
	bool RequestConnection::connection()
	{
		std::cout << "RequestConnection::connection()" << std::endl;
		
		socket_-> abort();
		socket_-> connectToHost(ip_, port_);
		
		return true;
	}
	
	void RequestConnection::readData()
	{
		bool readSocket(false);
		State headerState(Begin);
		DataType headerType(UndefinedType);
		unsigned int index;
		
		Graph* graph(NULL);
		
		while(state_ != ReadingError)
		{
			switch(state_)
			{
				case ReadingHeader :
				{
					// cout << "Reading header" << endl;
					// cout << "The bytes available are " << static_cast<unsigned int>(socket_-> bytesAvailable()) << endl;
					
					if(static_cast<unsigned int>(socket_-> bytesAvailable()) <= sizeof(header_))
					{
						// cout << "The number of available bytes is " << static_cast<unsigned int>(socket_-> bytesAvailable()) << endl;
						// cout << "The size of the header is " << sizeof(header_) << endl;
						// cout << "Not enough available bytes" << endl;
						return;
					}

					readSocket = fullread((char*) &header_, sizeof(header_));
					headerState = header_.state();
					
					if(readSocket == false)
					{
						// cout << "The number of available bytes is " << bytesAvailable << endl;
						// cerr << "Failed to read the header" << endl;
						
						return;
					}
					
					// std::cout << "RequestConnection::readData() : the header is " << std::endl;
					// std::cout << "id : " << header_.id() << std::endl;
					// std::cout << "type : " << header_.type() << std::endl;
					// std::cout << "format : " << header_.format() << std::endl;
					// std::cout << "serie: " << header_.serie() << std::endl;
					// std::cout << "number : " << header_.number() << std::endl;
					
					headerType = header_.type();
					
					if(headerType != chart()-> type())
					{
						std::cerr << "RequestConnection::readData() : Wrong data type" << std::endl;
						
						std::cout << "The header's type is " << headerType << std::endl;
						std::cout << "The chart's type is " << (chart()-> type()) << std::endl;
						
						return;
					}
					
					switch(headerType)
					{
						case UndefinedType :
						{
							std::cerr << "The type is undefined" << std::endl;
							
							return;
							
							break;
						}
						
						case SignalTimeWaterfallType :
						case WaterfallType :
						{
							BaseChart* chart(this-> chart());
							WaterfallChart* waterfall = dynamic_cast<WaterfallChart*>(chart);
							unsigned int rows(waterfall-> rows());
							unsigned int columns(waterfall-> columns());
							index = waterfall-> index();
							
							// QRectF pixel((chart-> datas())-> pixelHint(QRectF()));
							
							// std::cout << pixel.width() << " " << pixel.height() << std::endl;
							
							// std::cout << "index = " << index << std::endl;
							
							if(index >= (rows * columns))
							{
								std::cout << "RequestConnection::readData() : index = " << index << std::endl;
								std::cout << "Clearing waterfall plot" << std::endl;
								waterfall-> clear();
								waterfall-> replot();
							}
							
							break;
						}
						
						default :
						{
							BaseChart* chart(this-> chart());
							CurveChart* curve = dynamic_cast<CurveChart*>(chart);
							CurveGraph* curveGraph = curve-> curveGraph(header_.serie());
							
							if(curveGraph == NULL)
							{
								std::cerr << "RequestConnection::readData() : The serie " << header_.serie() << " hasn't been created yet" << std::endl;
								
								return;
							}
							
							graph = curveGraph-> graph();
							graph-> clearData();
							
							curveGraph-> clearDatas();
						}
					}
						
					// cout << "The state will be set to ReadingData" << endl;
					state_ = ReadingData;
					
					break;
				}
		
				case ReadingData :
				{
					// cout << "Reading data : " << std::endl;
					
					switch(header_.format())
					{
						case Short :
						{
							// cout << "short" << endl;
							// cout << "The bytes available are " << static_cast<unsigned int>(socket_-> bytesAvailable()) << endl;
							
							complex<short> cshort;

							// cout << "The number of available bytes is " << static_cast<unsigned int>(socket_-> bytesAvailable()) << endl;
							
							if(static_cast<unsigned int>(socket_-> bytesAvailable()) < sizeof(cshort))
							{
								// cout << "Not enough available bytes" << endl;
								// cout << "The size of cshort is " << sizeof(cshort) << endl;
								
								return;
							}

							// bytesRead = socket_-> read((char *) &cshort, 2*sizeof(short));
							
							readSocket = fullread((char*) &cshort, sizeof(cshort));
							
							if(readSocket == false)
							{
								cerr << "Failed to read cshort" << endl;
								
								return;
							}
							// else
							// {
								// cout << "The socket has been read" << endl;
							// }
							
							// cout << "The received data is " << cshort << endl;
							chartConnection_-> addData(cshort, header_.serie());
							break;
						}

						case Int :
						{
							complex<int> cint;

							if(static_cast<unsigned int>(socket_-> bytesAvailable()) < sizeof(cint))
							{
								// cout << "The number of available bytes is " << socket_-> bytesAvailable() << endl;
								// cout << "Not enough available bytes" << endl;
								return;
							}

							// bytesRead = socket_-> read((char *) &cint, 2*sizeof(int));
							
							readSocket = fullread((char*) &cint, sizeof(cint));
							
							if(readSocket == false)
							{
								cerr << "Failed to read cint" << endl;
								
								return;
							}
							// else
							// {
								// cout << "The socket has been read" << endl;
							// }
							
							// cout << "The received data is " << cint << endl;
							chartConnection_-> addData(cint, header_.serie());
							break;
						}

						case Float :
						{
							complex<float> cfloat;

							if(static_cast<unsigned int>(socket_-> bytesAvailable()) < sizeof(cfloat))
							{
								// cout << "The number of available bytes is " << socket_-> bytesAvailable() << endl;
								// cout << "Not enough available bytes" << endl;
								return;
							}

							// bytesRead = socket_-> read((char *) &cfloat, 2*sizeof(float));
							
							readSocket = fullread((char*) &cfloat, sizeof(cfloat));
							
							if(readSocket == false)
							{
								cerr << "Failed to read cfloat" << endl;
								
								return;
							}
							// else
							// {
								// cout << "The socket has been read" << endl;
							// }
							
							chartConnection_-> addData(cfloat, header_.serie());
							break;
						}

						case Double :
						{
							complex<double> cdouble;

							if(static_cast<unsigned int>(socket_-> bytesAvailable()) < sizeof(cdouble))
							{
								// cout << "The number of available bytes is " << socket_-> bytesAvailable() << endl;
								// cout << "Not enough available bytes" << endl;
								return;
							}

							// bytesRead = socket_-> read((char *) &cdouble, 2*sizeof(double));
							
							readSocket = fullread((char*) &cdouble, sizeof(cdouble));
							
							if(readSocket == false)
							{
								cerr << "Failed to read cdouble" << endl;
								
								return;
							}
							// else
							// {
								// cout << "The socket has been read" << endl;
							// }
							
							chartConnection_-> addData(cdouble, header_.serie());
							break;
						}

						default :
						{
							cerr << "The format is unknown" << endl;
							break;
						}
					}

					numLoaded_++;
					// cout << "numLoaded_ = " << numLoaded_ << endl;
					
					if(numLoaded_ == header_.number())
					{
						// cout << "All data have been loaded" << endl;
						numLoaded_ = 0;
					
						if((headerState == End) or (headerState == BeginEnd))
						{
							// std::cout << "Reploting " << std::endl;
							// std::cout << "index = " << index << std::endl;
							BaseChart* chart(this-> chart());
							chart-> replot();
							
							if(chartConnection_-> dataAcquisition())
							{
								if(not fullwrite((char*) &request_, sizeof(request_)))
								{
									return;
								}
							}
						}
						else
						{
							// cout << "No reploting" << endl;
						}
						// cout << "There are still " << bytesAvailable << " bytes in the socket" << endl;
						
						state_ = ReadingHeader;
					}
					
					if(numLoaded_ > header_.number())
					{
						// cerr << "The number of loaded data is greater than the number in the header" << endl;
					}

					break;
				}

				case ReadingEnd :
				{
					// cout << "The state is End" << endl;
					
					state_ = ReadingHeader;
					
					if(chartConnection_-> dataAcquisition())
					{
						// cout << "Sending another data request" << endl;
						if(not fullwrite((char*) &request_, sizeof(request_)))
						{
							cerr << "Failed to fullwrite on the socket" << endl;
							
							return;
						}
					}
					else
					{
						// cout << "dataAcquisition_ is false" << endl;
					}
					
					return;
					
					break;
				}
				
				case ReadingError:
				default :
				{
					// cerr << "An error occured" << endl;
					socket_-> abort();
					break;
				}
			}
		}

		// state_ = ReadingHeader;
	}

	ChartConnection::ChartConnection(Viewer* viewer, BaseChart* chart, unsigned int numData, ProcessID process, QObject* parent):
		QObject(parent),
		viewer_(viewer),
		connections_(),
		series_(),
		chart_(chart),
		numData_(numData),
		process_(process),
		dataAcquisition_(false),
		panel_(NULL)
	{
		cout << "ChartConnection::ChartConnection()" << endl;
		
		QHBoxLayout* layout(NULL);
		QPushButton* resume(NULL);
		QPushButton* stop(NULL);
				
		try
		{
			panel_ = new QWidget(chart_);
			layout = new QHBoxLayout(panel_);
			resume = new QPushButton(QString("Resume"), panel_);
			stop = new QPushButton(QString("Stop"), panel_);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "ChartConnection::ChartConnection() : " << ba.what() << std::endl;
			
			return;
		}
		
		layout-> addWidget(resume);
		layout-> addWidget(stop);
		panel_-> setLayout(layout);
		
		// panel_-> move(5, (chart_-> height() - panel_-> height()));
		panel_-> move(0, -10);
		panel_-> show();
		
		// viewer_-> addConnection(this);
		
		QObject::connect(resume, SIGNAL(clicked()), this, SLOT(resumeAcquisition()));
		QObject::connect(stop, SIGNAL(clicked()), this, SLOT(stopAcquisition()));
	}
	
	ChartConnection::~ChartConnection()
	{
		// delete socket_;
		
		for(std::map<unsigned int, RequestConnection*>::iterator it = connections_.begin(); it != connections_.end(); it++)
			delete it-> second;
	}
	
	void ChartConnection::addData(complex<int> ci, unsigned int serie)
	{
		complex<double> c(static_cast<double>(ci.real()), static_cast<double>(ci.imag()));

		try
		{
			chart_-> addData(c, serie);
		}
		catch(const out_of_range &oor)
		{
			cerr << "The chart wasn't found" << endl;
		}
	}

	void ChartConnection::addData(complex<float> cf, unsigned int serie)
	{
		complex<double> c(static_cast<complex<double>>(cf));

		try
		{
			chart_-> addData(c, serie);
		}
		catch(const out_of_range &oor)
		{
			cerr << "The chart wasn't found" << endl;
		}
	}

	void ChartConnection::addData(complex<double> c, unsigned int serie)
	{
		try
		{
			chart_-> addData(c, serie);
		}
		catch(const out_of_range &oor)
		{
			cerr << "The chart wasn't found" << endl;
		}
	}
	
	void ChartConnection::addData(complex<short> cs, unsigned int serie)
	{
		complex<double> c(static_cast<double>(cs.real()), static_cast<double>(cs.imag()));
		
		try
		{
			chart_-> addData(c, serie);
		}
		catch(const out_of_range &oor)
		{
			// cerr << "The chart wasn't found" << endl;
		}
	}
	
	void ChartConnection::stopAcquisition()
	{
		setDataAcquisition(false);
	}
	
	void ChartConnection::resumeAcquisition()
	{
		setDataAcquisition(true);
		
		for(std::map<unsigned int, RequestConnection*>::iterator it = connections_.begin(); it != connections_.end(); it++)
		{
			(it-> second)-> sendRequest();
			// fullwrite((char*) &request_, sizeof(request_));
		}
	}
	
	bool ChartConnection::addRequestConnection(RequestConnection* requestConnection)
	{
		// QHostAddress ip(requestConnection-> ip());
		unsigned int port(requestConnection-> port());
		std::pair<unsigned int, RequestConnection*> element(port, requestConnection);
		std::pair<std::map<unsigned int, RequestConnection*>::iterator, bool> ret;
		ret = connections_.insert(element);
		
		if(ret.second == false)
		{
			std::cerr << "A connection using the port " << port << " already exist" << std::endl;
		}
		else
		{
			std::cout << "The connection has been added" << std::endl;
			requestConnection-> connection();
		}
		
		return ret.second;
	}
	
	/*
		This method add a RequestConnection to the ChartConnection
		It first checks if the parameters are correct
		Then it
	*/
	
	RequestConnection* ChartConnection::createRequestConnection(QHostAddress ip, unsigned int port, DataRequest request)
	{
		std::cout << "ChartConnection::addRequestConnection()" << std::endl;
		std::map<unsigned int, RequestConnection*>::iterator it(connections_.find(port));
		RequestConnection* ret(NULL);
		
		unsigned int id(chart_-> id());
		DataType type(chart_-> type());
		
		if((request.id() != id) or (request.type() != type))
		{
			std::cout << "The request id is " << request.id() << " and the chart id is " << id << std::endl;
			std::cout << "The request type is " << request.type() << " and the chart type is " << type << std::endl;
			
			return NULL;
		}
		
		switch(type)
		{
			case UndefinedType :
			{
				std::cerr << "RequestConnection::connection(): The type of the chart is undefined" << std::endl;
			
				return NULL;
				
				break;
			}
			
			case SignalTimeWaterfallType :
			case WaterfallType :
			{
				unsigned int seriesSize(request.size());
			
				if(seriesSize > 1)
				{
					std::cout << "The size of the series is " << seriesSize << std::endl;
					std::cout << "RequestConnection::connection() : A waterfall chart can only display one serie" << std::endl;
					std::cout << "RequestConnection::connection() : Only the first serie will be kept" << std::endl;
					
					while((request.size()) >= 1)
						request.removeSerie();
				}
				
				WaterfallChart* waterfallChart(dynamic_cast<WaterfallChart*>(chart_));
				
				waterfallChart-> clear();
				
				break;
			}
			
			default :
			{
				CurveChart* curve(dynamic_cast<CurveChart*>(chart_));
				std::map<unsigned int, CurveGraph*> graphes(curve-> graphes());
				
				for(unsigned int i = 0; i < request.size(); i++)
				{
					SerieSignal serie(request.serie(i));
					unsigned int index(serie.serie());
					
					std::map<unsigned int, CurveGraph*>::iterator it2(graphes.find(index));
					
					if(it2 == graphes.end())
					{
						curve-> createSerie(index);
					}
				}
				
				break;
			}
		}
		
		if(it == connections_.end())
		{
			std::cout << "The connection to the port " << port << " is inactive" << std::endl;
			
			try
			{
				ret = new RequestConnection(this, ip, port, request, this);
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "ChartConnection::createRequestConnection() : bad allocation of " << ba.what() << std::endl;
				
				return NULL;
			}
			
			addRequestConnection(ret);
		}
		else
		{
			ret = it-> second;
			ret-> updateRequest(request);
		}
		
		return ret;
	}
	
	bool ChartConnection::setSerie(SerieSignal serie)
	{
		if(serie.serie() >= MAX_SERIE)
			return false;
		
		return series_.setSerie(serie);
	}
	
	bool ChartConnection::addSerie(SerieSignal serie)
	{
		bool ret(series_.addSerie(serie));
		
		std::cout << "ChartConnection::addSerie() : The serie " << serie.serie();
		
		if(ret == true)
		{
			std::cout  << " has been added" << std::endl;
			chart_-> createSerie(serie.serie());
		}
		else
		{
			std::cout << " hasn't been added" << std::endl;
		}
		
		return ret;
	}
}