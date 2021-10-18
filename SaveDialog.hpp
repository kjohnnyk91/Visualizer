#ifndef HPP_SAVEDIALOG
#define HPP_SAVEDIALOG

#include <iostream>
#include <QDialog>
#include "Viewer.hpp"

namespace ModelView
{
	class SaveDialog : public QDialog
	{
		Q_OBJECT
		
		private :
			Viewer* viewer_;
			QFileDialog* fileDialog_;
			QLineEdit* addressLine_;
			
		public :
			SaveDialog(Viewer*, QWidget* = NULL);
			~SaveDialog();
			
			void build();
			
		signals :
			void fileSelected(string);
			
		public slots :
			void saveFileDialog();
			void accept();
			// void reject();
	};
}

#endif