#include "TreeItem.hpp"

namespace ModelView
{
	TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent) :
		childs_(),
		data_(data),
		parent_(parent)
	{
		if(parent != NULL)
			parent-> append(this);
	}
	
	TreeItem::TreeItem(Graph* graph, TreeItem* parent) :
		childs_(),
		data_(),
		parent_(parent)
	{
		std::cout << "TreeItem::TreeItem(Graph*)" << std::endl;
		
		if(parent_ != NULL)
			parent_-> append(this);
		
		QVariant qserie(graph-> serie());
		QVariant qxquantity(quantityString(graph-> xQuantity()).c_str());
		QVariant qyquantity(quantityString(graph-> yQuantity()).c_str());
		QVariant qxmultiple(multipleString(graph-> xMultiple()).c_str());
		QVariant qymultiple(multipleString(graph-> yMultiple()).c_str());
		QVariant qxunit(unitString(graph-> xUnit()).c_str());
		QVariant qyunit(unitString(graph-> yUnit()).c_str());
		
		data_.append(QVariant());
		data_.append(QVariant());
		data_.append(QVariant());
		data_.append(qserie);
		data_.append(qxquantity);
		data_.append(qyquantity);
		data_.append(qxunit);
		data_.append(qyunit);
		data_.append(qxmultiple);
		data_.append(qymultiple);
	}
	
	TreeItem::TreeItem(BaseChart* chart, TreeItem *parent) :
		childs_(),
		data_(),
		parent_(parent)
	{
		std::cout << "TreeItem::TreeItem(BaseChart*)" << std::endl;
		
		if(parent_ != NULL)
			parent_-> append(this);
		
		unsigned int qid(chart-> id());
		QString qtitle((chart-> title()).c_str());
		QString qtype(typeString(chart-> type()).c_str());
		QString qxquantity(quantityString(chart-> xQuantity()).c_str());
		QString qyquantity(quantityString(chart-> yQuantity()).c_str());
		QString qxmultiple(multipleString(chart-> xMultiple()).c_str());
		QString qymultiple(multipleString(chart-> yMultiple()).c_str());
		QString qxunit(unitString(chart-> xUnit()).c_str());
		QString qyunit(unitString(chart-> yUnit()).c_str());
		
		data_.append(qid);
		data_.append(qtitle);
		data_.append(qtype);
		data_.append(QVariant());
		data_.append(qxquantity);
		data_.append(qyquantity);
		data_.append(qxunit);
		data_.append(qyunit);
		data_.append(qxmultiple);
		data_.append(qymultiple);
		
		std::cout << "ColumnCount = " << columnCount() << std::endl;
		
		switch(chart-> type())
		{
			case WaterfallType :
			case SignalTimeWaterfallType :
			{
				WaterfallChart* waterfall(dynamic_cast<WaterfallChart*>(chart));
				
				Graph* graph(waterfall-> graph());
				
				TreeItem* item(NULL);
				
				try
				{
					item = new TreeItem(graph, this);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "TreeItem::TreeItem() : bad allocation of " << ba.what() << std::endl;
					
					return;
				}
				
				break;
			}
			
			default :
			{
				CurveChart* curve(dynamic_cast<CurveChart*>(chart));
				
				std::map<unsigned int, CurveGraph*> curveGraphes(curve-> graphes());
				
				for(std::map<unsigned int, CurveGraph*>::iterator it = curveGraphes.begin(); it != curveGraphes.end(); it++)
				{
					TreeItem* item(NULL);
					
					try
					{
						item = new TreeItem((it-> second)-> graph(), this);
					}
					catch(const std::bad_alloc& ba)
					{
						std::cerr << "TreeItem::TreeItem() : bad allocation of " << ba.what() << std::endl;
						
						return;
					}
				}
				
				break;
			}
		}
	}
	
	TreeItem::TreeItem(Viewer* viewer, TreeItem* parent) :
		childs_(),
		data_(),
		parent_(parent)
	{
		std::cout << "TreeItem::TreeItem(Viewer*)" << std::endl;
		
		data_.append(QString("Id"));
		data_.append(QString("Title"));
		data_.append(QString("Type"));
		data_.append(QString("Serie"));
		data_.append(QString("X quantity"));
		data_.append(QString("Y quantity"));
		data_.append(QString("X unit"));
		data_.append(QString("Y unit"));
		data_.append(QString("X multiple"));
		data_.append(QString("Y multiple"));
		
		if(parent_ != NULL)
			parent_-> append(this);
		
		std::map<unsigned int, BaseChart*> charts(viewer-> charts());
		
		for(std::map<unsigned int, BaseChart*>::iterator it = charts.begin(); it != charts.end(); it++)
		{
			TreeItem* item(NULL);
			
			try
			{
				item = new TreeItem(it-> second, this);
			}
			catch(const std::bad_alloc& ba)
			{
				std::cerr << "TreeItem::TreeItem() : bad allocation of " << ba.what() << std::endl;
				
				return;
			}
		}
	}

	TreeItem::~TreeItem()
	{
		qDeleteAll(childs_);
	}

	void TreeItem::append(TreeItem *item)
	{
		childs_.append(item);
	}

	TreeItem *TreeItem::child(int row)
	{
		if (row < 0 || row >= childs_.size())
			return nullptr;
		return childs_.at(row);
	}

	int TreeItem::childCount() const
	{
		return childs_.count();
	}

	int TreeItem::row() const
	{
		if (parent_)
			return parent_->childs_.indexOf(const_cast<TreeItem*>(this));

		return 0;
	}

	int TreeItem::columnCount() const
	{
		return data_.count();
	}

	QVariant TreeItem::data(int column) const
	{
		if (column < 0 || column >= data_.size())
			return QVariant();
		return data_.at(column);
	}

	TreeItem *TreeItem::parent()
	{
		return parent_;
	}
}