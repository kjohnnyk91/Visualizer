#include <iostream>
#include <complex>
// #include <vector>
#include <string>
#include <stdexcept>
#include "Graph.hpp"

namespace ModelView
{
	Graph::Graph():
		QObject(NULL),
		datas_(),
		xQuantity_(None),
		yQuantity_(None),
		xUnit_(No),
		yUnit_(No),
		xMultiple_(unity),
		yMultiple_(unity),
		chart_(NULL),
		serie_(SERIE_UNDEFINED)
	{}
	
	Graph::Graph(const Graph& graph) :
		QObject(NULL),
		datas_(graph.datas()),
		xQuantity_(graph.xQuantity()),
		yQuantity_(graph.yQuantity()),
		xUnit_(graph.xUnit()),
		yUnit_(graph.yUnit()),
		xMultiple_(graph.xMultiple()),
		yMultiple_(graph.yMultiple()),
		chart_(NULL),
		serie_(SERIE_UNDEFINED)
	{}
	
	Graph::Graph(const Graph* graph) :
		QObject(NULL),
		datas_(graph-> datas()),
		xQuantity_(graph-> xQuantity()),
		yQuantity_(graph-> yQuantity()),
		xUnit_(graph-> xUnit()),
		yUnit_(graph-> yUnit()),
		xMultiple_(graph-> xMultiple()),
		yMultiple_(graph-> yMultiple()),
		chart_(NULL),
		serie_(SERIE_UNDEFINED)
	{}
	
	Graph::Graph(Quantity xQuantity, Quantity yQuantity, Unit xUnit, Unit yUnit, Multiple xMultiple, Multiple yMultiple, unsigned int serie):
		QObject(NULL),
		datas_(),
		xQuantity_(xQuantity),
		yQuantity_(yQuantity),
		xUnit_(xUnit),
		yUnit_(yUnit),
		xMultiple_(xMultiple),
		yMultiple_(yMultiple),
		chart_(NULL),
		serie_(serie)
	{}
	
	Graph::Graph(vector<complex<double>> datas, Quantity xQuantity, Quantity yQuantity, Unit xUnit, Unit yUnit, Multiple xMultiple, Multiple yMultiple, unsigned int serie, BaseChart* chart):
		datas_(datas),
		xQuantity_(xQuantity),
		yQuantity_(yQuantity),
		xUnit_(xUnit),
		yUnit_(yUnit),
		xMultiple_(xMultiple),
		yMultiple_(yMultiple),
		chart_(chart),
		serie_(serie)
	{}

	Graph::~Graph()
	{
		cout << "Graph destructor" << endl;
	}

	void Graph::setData(unsigned int index, complex<double> data)
	{
		if(index >= datas_.size())
		{
			datas_.resize(index + 1);
		}
		
		datas_[index] = data;
		
		emit dataChanged(index, serie_);
	}

	void Graph::setDatas(vector<complex<double>> datas)
	{
		datas_ = datas;
	}

	void Graph::setDatas(unsigned int start, vector<complex<double>> datas)
	{
		if( (start + datas.size()) > datas_.size() )
		{
			datas_.resize(start + datas.size());
		}
		
		for(unsigned int i = 0; i < datas.size(); i++)
			{
				datas_.at(start + i) = datas.at(i);
			}
	}

	void Graph::setDatas(vector<unsigned int> indexes, vector<complex<double>> datas)
	{
		vector<complex<double>> Vout;
		
		if(indexes.size() == datas.size())
		{
			unsigned int max = indexes.at(0);
			
			for(unsigned int i = 0; i < indexes.size(); i++)
			{
				if(max < indexes.at(i))
				{
					max = indexes.at(i);
				}
			}
			
			if(max + 1 > datas_.size())
			{
				datas_.resize(max + 1);
			}
			
			for(unsigned int i = 0; i < indexes.size(); i++)
			{
				datas_.at(indexes.at(i)) = datas.at(i);
				Vout.push_back(datas.at(i));
			}
		}
		else
		{
			throw "The size of indexes and datas don't match";
		}
	}

	std::vector<complex<double>> Graph::datas() const
	{
		return datas_;
	}

	vector<complex<double>> Graph::datas(unsigned int start, unsigned int stop) const
	{
		vector<complex<double>> Vout;
		
		if(start > datas_.size())
		{
			throw "Start index is out of range";
		}
		
		if(stop > datas_.size())
		{
			throw "Stop index is out of range";
		}
		
		if(start < stop)
		{
			Vout.assign(datas_.begin() + start, datas_.begin() + stop + 1);
		}
		else
		{
			throw "Start index is greater than stop index";
		}
		
		return Vout;
	}

	vector<complex<double>> Graph::datas(vector<unsigned int> indexes) const
	{
		vector<complex<double>> Vout(indexes.size(),0);
		
		for(unsigned int i = 0; i < indexes.size(); i++)
		{
			Vout[i] = datas_.at(indexes[i]);
		}
		
		return Vout;
	}
	
	vector<complex<double>> Graph::addDatas(vector<complex<double>> datas)
	{
		unsigned int size_(datas_.size());
		unsigned int size(datas.size());
		
		datas_.resize(size_ + size);
		
		for(unsigned int i = size_; i < size_ + size; i++)
		{
			datas_[i] = abs(datas[i]);
		}
		
		return datas;
	}
	
	bool Graph::toXml(std::fstream& file)
	{
		if(file.good())
		{
			file << indent << "<graph xqty=\"" << quantityString(xQuantity_)
					<< "\" yqty=\"" << quantityString(yQuantity_)
					<< "\" xunit=\"" << unitString(xUnit_)
					<< "\" yunit=\"" << unitString(yUnit_)
					<< "\" xmultiple" << multipleString(xMultiple_)
					<< "\" ymultiple" << multipleString(yMultiple_)
					<< "\" serie=\"" << serie_
					<< "\" >" << std::endl;
			
			++indent;
			
			for(std::vector<std::complex<double>>::iterator it = datas_.begin(); it != datas_.end(); it++)
			{
				file << indent << "<point x=\"" << it-> real()
										<< "\" y=\"" << it-> imag()
										<< "\"/>"<< std::endl;
			}
			
			file << --indent << "</graph>" << std::endl;
			
			return true;
		}
		else
		{
			return false;
		}
	}
}
