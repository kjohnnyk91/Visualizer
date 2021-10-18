#ifndef HPP_VIEWERTEXTEDIT
#define HPP_VIEWERTEXTEDIT

#include <QTextEdit>
#include "Graph.hpp"
#include "NewDialog.hpp"

using namespace std;

namespace ModelView
{
	class ViewerTextEdit : public QTextEdit
	{
		Q_OBJECT
		
		public :
			ViewerTextEdit(QWidget* = NULL);
			ViewerTextEdit(const QString&, QWidget* = NULL);
			~ViewerTextEdit();
		
		public slots :
			void setText(int);
	};
}

#endif