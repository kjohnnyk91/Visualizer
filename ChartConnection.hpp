#ifndef HPP_CHARTCONNECTION
#define HPP_CHARTCONNECTION

#include <QAbstractSocket>
#include "Viewer.hpp"
#include "GraphView.hpp"
#include "DataRequest.hpp"
#include <unistd.h>

namespace ModelView
{
	class ChartConnection;
	
	class RequestConnection : public QObject
	{
		Q_OBJECT
		
		enum ReadingState
		{
			ReadingHeader,
			ReadingData,
			ReadingEnd,
			ReadingError
		};
		
		private :
			ChartConnection* chartConnection_;
			QHostAddress ip_;
			unsigned int port_;
			QTcpSocket* socket_;
			DataRequest request_;
			unsigned long numLoaded_;
			
			ReadingState state_;
			DataHeader header_;
			
		public :
			RequestConnection(ChartConnection* chartConnection, QHostAddress ip, unsigned int port, DataRequest request, QObject* parent = NULL);
			~RequestConnection();
			
			inline unsigned int port() const;
			inline QHostAddress ip() const;
			inline DataRequest request() const;
			inline GraphSeries series();
			inline unsigned long numLoaded() const;
			
			inline BaseChart* chart();
			
			inline void setRequest(DataRequest request);
			bool setSerie(SerieSignal);
			inline void setSeries(GraphSeries);
			
			unsigned int addSignal(unsigned int signal);
			bool addSerie(SerieSignal serie);
			void addSeries(GraphSeries series);
			void updateRequest(DataRequest request);
			
			inline unsigned int seriesNumber();
			
			inline bool sendRequest();
			
			bool fullwrite(char*, unsigned int);
			bool fullread(char*, unsigned int);
			
			inline void close();
			inline void abort();
			
			bool connection();
			
		public slots:
			void connected();
			void readData();
			void displayError(QAbstractSocket::SocketError socketError);
	};
	
	class ChartConnection : public QObject
	{
		Q_OBJECT
		
		private :
			Viewer* viewer_;
			std::map<unsigned int, RequestConnection*> connections_;
			GraphSeries series_;
			BaseChart* chart_;
			
			unsigned int numData_;
			ProcessID process_;
			
			bool dataAcquisition_;
			
			QWidget* panel_;
			
		public :
			ChartConnection(Viewer* viewer, BaseChart* chart, unsigned int numData, ProcessID process, QObject* parent = NULL);
			~ChartConnection();
			
			inline Viewer* viewer();
			inline BaseChart* chart();
			
			inline unsigned int id() const;
			
			inline std::map<unsigned int, RequestConnection*> connections() const;
			
			inline unsigned int seriesNumber() const;
			inline unsigned int numData() const;
			inline ProcessID process() const;
			inline bool dataAcquisition() const;
			
			inline void setNumData(unsigned int numData);
			inline void setProcess(ProcessID process);
			
			inline void setDataAcquisition(bool);
			
			RequestConnection* createRequestConnection(QHostAddress ip, unsigned int port, DataRequest request);
			bool addRequestConnection(RequestConnection* requestConnection);
			inline bool sendRequests();
			
			inline void close();
			inline void abort();
			
			inline void clearGraphes();
			
			bool setSerie(SerieSignal serie);
			bool addSerie(SerieSignal serie);
			
			inline std::map<unsigned int, RequestConnection*>::iterator find(unsigned int port);
			inline std::map<unsigned int, RequestConnection*>::iterator end();
				
		public slots:
			void addData(complex<short> cs, unsigned int serie);
			void addData(complex<int> ci, unsigned int serie);
			void addData(complex<float> cf, unsigned int serie);
			void addData(complex<double> cs, unsigned int serie);
			void stopAcquisition();
			void resumeAcquisition();
	};
	
	inline unsigned int RequestConnection::port() const
	{
		return port_;
	}
	
	inline QHostAddress RequestConnection::ip() const
	{
		return ip_;
	}
	
	inline DataRequest RequestConnection::request() const
	{
		return request_;
	}
	
	inline GraphSeries RequestConnection::series()
	{
		return request_.series();
	}
	
	inline unsigned long RequestConnection::numLoaded() const
	{
		return numLoaded_;
	}
	
	BaseChart* RequestConnection::chart()
	{
		return chartConnection_-> chart();
	}
	
	inline void RequestConnection::setRequest(DataRequest request)
	{
		request_ = request;
	}
	
	inline void RequestConnection::setSeries(GraphSeries series)
	{
		request_.setSeries(series);
	}
	
	inline unsigned int RequestConnection::seriesNumber()
	{
		return (request_.series()).end();
	}
	
	inline bool RequestConnection::sendRequest()
	{
		return fullwrite((char*) &request_, sizeof(request_));
	}
	
	inline void RequestConnection::close()
	{
		socket_-> close();
	}
	
	inline void RequestConnection::abort()
	{
		socket_-> abort();
	}
	
	inline Viewer* ChartConnection::viewer()
	{
		return viewer_;
	}
	
	inline BaseChart* ChartConnection::chart()
	{
		return chart_;
	}
	
	inline unsigned int ChartConnection::id() const
	{
		return chart_-> id();
	}
	
	inline unsigned int ChartConnection::seriesNumber() const
	{
		unsigned int count(0);
		
		for(std::map<unsigned int, RequestConnection*>::const_iterator it = connections_.begin(); it != connections_.end(); it++)
		{
			count += ((it-> second)-> seriesNumber());
		}
		
		return count;
	}
	
	inline std::map<unsigned int, RequestConnection*> ChartConnection::connections() const
	{
		return connections_;
	}
	
	inline unsigned int ChartConnection::numData() const
	{
		return numData_;
	}
	
	inline ProcessID ChartConnection::process() const
	{
		return process_;
	}
	
	inline bool ChartConnection::dataAcquisition() const
	{
		return dataAcquisition_;
	}
	
	inline void ChartConnection::setNumData(unsigned int numData)
	{
		numData_ = numData;
	}
	
	inline void ChartConnection::setProcess(ProcessID process)
	{
		process_ = process;
	}
	
	inline void ChartConnection::setDataAcquisition(bool dataAcquisition)
	{
		dataAcquisition_ = dataAcquisition;
		
		if(dataAcquisition_ == true)
		{
			cout << "ChartConnection::setDataAcquisition() : true" << endl;
		}
		else
		{
			cout << "ChartConnection::setDataAcquisition() : false" << endl;
		}
	}
	
	inline bool ChartConnection::sendRequests()
	{
		cout << "Request " << endl;
		// cout << "id : " << request_.id() << endl;
		// cout << "series : " << request_.serie() << endl;
		// cout << "numData " << request_.numData() << endl;
		// cout << "processID " << request_.processID() << endl;
		
		// return fullwrite((char*) &request_, sizeof(request_));
		
		for(std::map<unsigned int, RequestConnection*>::iterator it = connections_.begin(); it != connections_.end(); it++)
		{
			if(not ((it-> second)-> sendRequest()))
				return false;
		}
		
		return true;
	}
	
	inline void ChartConnection::close()
	{
		for(std::map<unsigned int, RequestConnection*>::iterator it = connections_.begin(); it != connections_.end(); it++)
		{
			(it-> second)-> close();
		}
		
		delete this;
	}
	
	inline void ChartConnection::abort()
	{
		for(std::map<unsigned int, RequestConnection*>::iterator it = connections_.begin(); it != connections_.end(); it++)
		{
			(it-> second)-> abort();
		}
	}
	
	inline void ChartConnection::clearGraphes()
	{
		chart_-> clearData();
	}
	
	inline std::map<unsigned int, RequestConnection*>::iterator ChartConnection::find(unsigned int port)
	{
		return connections_.find(port);
	}
	
	inline std::map<unsigned int, RequestConnection*>::iterator ChartConnection::end()
	{
		return connections_.end();
	}
}

#endif