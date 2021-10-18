#ifndef TREEITEM_HPP
#define TREEITEM_HPP

#include <QVariant>
#include <QVector>
#include "Viewer.hpp"
#include "InterfaceTypeHeader.hpp"

namespace ModelView
{
	class TreeItem
	{
		public:
			explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = NULL);
			explicit TreeItem(Graph* graph, TreeItem *parent = NULL);
			explicit TreeItem(BaseChart* chart, TreeItem *parent = NULL);
			explicit TreeItem(Viewer* viewer, TreeItem *parent = NULL);
			~TreeItem();

			void append(TreeItem *child);

			TreeItem *child(int row);
			int childCount() const;
			int columnCount() const;
			QVariant data(int column) const;
			int row() const;
			TreeItem *parent();

		private:
			QVector<TreeItem*> childs_;
			QVector<QVariant> data_;
			TreeItem* parent_;
	};
}

#endif // TREEITEM_H