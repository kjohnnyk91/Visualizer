#ifndef HPP_RENAMEDIALOG
#define HPP_RENAMEDIALOG

#include <QtWidgets>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>

namespace ModelView
{
	class RenameDialog : public QDialog
	{
		Q_OBJECT
		
		private :
			QLineEdit newName_;
		
		public :
			RenameDialog(QString oldName, QWidget* parent = NULL);
			~RenameDialog();
			
		signals :
			void accepted(QString);
				
		public slots :
			void accept();
			// void reject();
	};
}

#endif