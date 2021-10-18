#include "OpenDialog.hpp"

namespace ModelView
{
	void OpenDialog::build()
	{
		QLabel* label(new QLabel(QString("Choose a chart file to open"), this));
		QVBoxLayout* layout(new QVBoxLayout(this));
		QWidget* addressWidget(new QWidget(this));
		QHBoxLayout* addressLayout(new QHBoxLayout(addressWidget));
		QWidget* buttonWidget(new QWidget(this));
		QHBoxLayout* buttonLayout(new QHBoxLayout(buttonWidget));
		QPushButton* ok(new QPushButton(QString("Ok"), this));
		QPushButton* cancel(new QPushButton(QString("Cancel"), this));
		QPushButton* browse(new QPushButton(QString("Browse"), this));
		
		setWindowTitle("Opening chart");
		
		addressLayout-> addWidget(addressLine_);
		addressLayout-> addWidget(browse);
		addressWidget-> setLayout(addressLayout);
		
		layout-> addWidget(label);
		layout-> addWidget(addressWidget);
		
		buttonLayout-> addWidget(ok);
		buttonLayout-> addWidget(cancel);
		buttonWidget-> setLayout(buttonLayout);
		
		layout-> addWidget(buttonWidget);
		
		setLayout(layout);
		
		setAttribute(Qt::WA_DeleteOnClose);
		
		QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
		QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
		QObject::connect(browse, SIGNAL(clicked()), this, SLOT(openFileDialog()));
		
	}
	
	OpenDialog::OpenDialog(Viewer* viewer, QWidget* parent):
		QDialog(parent),
		viewer_(viewer),
		addressLine_(new QLineEdit(QString("Select a file"), parent))
	{
		setAttribute(Qt::WA_DeleteOnClose);
		build();
		
		setModal(true);
	}
	
	OpenDialog::~OpenDialog()
	{
		cout << "Destructeur de OpenDialog" << endl;
	}
	
	void OpenDialog::openFileDialog()
	{
		QString filename(QFileDialog::getOpenFileName(this, tr("Open chart ..."), QString(), tr("Xml chart (*.xml)")));
		
		addressLine_-> setText(filename);
	}
	
	void OpenDialog::accept()
	{
		QString path(addressLine_-> text());
		
		if(not path.isEmpty())
		{
			emit fileSelected(path.toStdString());
			
			close();
		}
		else
		{
			QMessageBox error;
			error.setText(QString("The file name is undefined"));
			error.exec();
		}
	}
}