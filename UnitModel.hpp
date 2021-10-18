#ifndef HPP_UNITMODEL
#define HPP_UNITMODEL

#include <QStringListModel>
#include "Graph.hpp"

namespace ModelView
{
	class UnitModel : public QStringListModel
	{
		Q_OBJECT
		
		public :
			UnitModel(QObject* = NULL);
			~UnitModel();
			
			int rowCount(const QModelIndex&) const;
			QVariant data(const QModelIndex&, int = Qt::DisplayRole) const;
	};
}
#endif