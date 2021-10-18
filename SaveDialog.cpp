#include "SaveDialog.hpp"

namespace ModelView
{
	void SaveDialog::build()
	{
		QLabel* label(new QLabel(QString("Save the chart as ..."), this));
		QVBoxLayout* layout(new QVBoxLayout(this));
		QWidget* addressWidget(new QWidget(this));
		QHBoxLayout* addressLayout(new QHBoxLayout(addressWidget));
		QWidget* buttonWidget(new QWidget(this));
		QHBoxLayout* buttonLayout(new QHBoxLayout(buttonWidget));
		QPushButton* ok(new QPushButton(QString("Ok"), this));
		QPushButton* cancel(new QPushButton(QString("Cancel"), this));
		QPushButton* browse(new QPushButton(QString("Browse"), this));
		
		setWindowTitle("Saving chart");
		
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
		QObject::connect(browse, SIGNAL(clicked()), this, SLOT(saveFileDialog()));
	}

	SaveDialog::SaveDialog(Viewer* viewer, QWidget* parent):
		QDialog(parent),
		viewer_(viewer),
		addressLine_(new QLineEdit(QString(), this))
	{
		build();
		
		setModal(true);
	}
	
	SaveDialog::~SaveDialog()
	{}
	
	void SaveDialog::saveFileDialog()
	{
		QString filename(QFileDialog::getSaveFileName(this, tr("Save chart as ..."), QString(), tr("Xml chart (*.xml)")));
		
		addressLine_-> setText(filename);
	}
	
	void SaveDialog::accept()
	{
		QString filename(addressLine_-> text());
		
		if(not filename.isEmpty())
		{
			viewer_-> saveChart(filename.toStdString());
			
			close();
		}
		else
		{
			QMessageBox error;
			error.setText(QString("The file name is undefined"));
			error.exec();
		}
	}
	
	// void SaveDialog::reject()
	// {
		// close();
	// }
}