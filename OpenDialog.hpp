#ifndef HPP_OPENDIALOG
#define HPP_OPENDIALOG

// #include <QtWidgets>
#include <QFileDialog>
#include "Graph.hpp"
#include "Viewer.hpp"

namespace ModelView
{
	class Viewer;
	
	class OpenDialog : public QDialog
	{
		Q_OBJECT
		
		private :
			Viewer* viewer_;
			QFileDialog* fileDialog_;
			QLineEdit* addressLine_;
			
		public :
			OpenDialog(Viewer*, QWidget* = NULL);
			~OpenDialog();
			
			void build();
			
		signals :
			void fileSelected(string);
			
		public slots :
			void openFileDialog();
			void accept();
	};
}

#endif