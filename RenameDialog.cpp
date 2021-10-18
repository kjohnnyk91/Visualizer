#include "RenameDialog.hpp"

namespace ModelView
{
	RenameDialog::RenameDialog(QString oldName, QWidget* parent):
		QDialog(parent),
		newName_(oldName, this)
	{
		setWindowTitle(QString("Rename as ..."));
		
		QVBoxLayout* layout(NULL);
		QWidget* buttonWidget(NULL);
		QHBoxLayout* buttonLayout(NULL);
		QPushButton* ok(NULL);
		QPushButton* cancel(NULL);
		
		try
		{
			buttonWidget = new QWidget(this);
			buttonLayout = new QHBoxLayout(buttonWidget);
			layout = new QVBoxLayout(this);
			ok = new QPushButton(QString("Ok"), this);
			cancel = new QPushButton(QString("Cancel"), this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "RenameDialog::RenameDialog() : bad allocation " << ba.what() << std::endl;
			
			return;
		}
		
		buttonLayout-> addWidget(ok);
		buttonLayout-> addWidget(cancel);
		buttonWidget-> setLayout(buttonLayout);
		layout-> addWidget(&newName_);
		layout-> addWidget(buttonWidget);
		
		setLayout(layout);
		
		// setAttribute(Qt::WA_DeleteOnClose);
		// setModal(true);
		
		QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
		QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
	}
	
	RenameDialog::~RenameDialog()
	{}
	
	void RenameDialog::accept()
	{
		std::cout << "RenameDialog::accept()" << std::endl;
		// setResult(QDialog::Accepted);
		emit accepted(newName_.text());
		
		std::cout << "close() == " << std::boolalpha << this-> close() << std::endl;
	}
	
	// void RenameDialog::reject()
	// {
		// setResult(QDialog::Rejected);
		// this-> close();
	// }
}