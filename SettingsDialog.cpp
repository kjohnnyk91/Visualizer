#include "SettingsDialog.hpp"

namespace ModelView
{
	void SettingsDialog::buildProcessCombo()
	{
		processCombo_-> addItem("No process", NoProcessID);
		processCombo_-> addItem("Average", AverageID);
		processCombo_-> addItem("Downsampling", DownsamplingID);
	}
	
	SettingsDialog::SettingsDialog(Viewer* viewer, QWidget* parent):
		QDialog(parent),
		viewer_(viewer),
		portSpin_(NULL),
		ipLine_(NULL),
		idCombo_(NULL),
		// serieSpin_(NULL),
		numDataSpin_(NULL),
		processCombo_(NULL),
		seriesWidget_(NULL)
	{
		try
		{
			portSpin_ = new QSpinBox(this);
			ipLine_ = new QLineEdit(QString("127.0.0.1"));
			idCombo_ = new QComboBox(this);
			// serieSpin_ = new QSpinBox(this);
			numDataSpin_ = new QSpinBox(this);
			processCombo_ = new QComboBox(this);
			seriesWidget_ = new SeriesWidget(this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "SettingsDialog::SettingsDialog() : " << ba.what() << std::endl;
		}
		
		setAttribute(Qt::WA_DeleteOnClose);
		buildProcessCombo();
		
		portSpin_-> setSingleStep(1);
		portSpin_-> setRange(0,65535);
		portSpin_-> setValue(8888);
		
		std::map<unsigned int, BaseChart*> charts = this-> charts();
		
		for(std::map<unsigned int, BaseChart*>::iterator it = charts.begin(); it != charts.end(); it++)
		{
			BaseChart* chart(it-> second);
			std::cout << "SettingsDialog::SettingsDialog() : The title is " << chart-> title() << std::endl;
			QString qtitle((chart-> title()).c_str());
			QVariant qid(chart-> id());
			
			QString qtext(qtitle += QString(" (") += QString(QString::number(chart-> id())) += QString(')'));
			
			idCombo_-> addItem(qtext, qid);
		}
		
		numDataSpin_-> setSingleStep(1);
		numDataSpin_-> setRange(0,10000000);
		numDataSpin_-> setValue(1000);
		
		QLabel* settingsLabel(new QLabel(QString("Settings"),this));
		QLabel* portLabel(new QLabel(QString("Port"),this));
		QLabel* ipLabel(new QLabel(QString("IP"),this));
		QLabel* dataRequestLabel(new QLabel(QString("Data request"), this));
		QLabel* idLabel(new QLabel(QString("Id"),this));
		QLabel* seriesLabel(new QLabel(QString("Series"),this));
		QLabel* numDataLabel(new QLabel(QString("Amount of data"),this));
		QLabel* processLabel(new QLabel(QString("Process"), this));
		
		QPushButton* ok(new QPushButton(QString("Ok"), this));
		QPushButton* cancel(new QPushButton(QString("Cancel"), this));
		
		QVBoxLayout* layout(new QVBoxLayout(this));
		
		QWidget* portWidget(new QWidget(this));
		QWidget* ipWidget(new QWidget(this));
		QWidget* idWidget(new QWidget(this));
		QWidget* seriesWidget(new QWidget(this));
		QWidget* numDataWidget(new QWidget(this));
		QWidget* processWidget(new QWidget(this));
		QWidget* buttonWidget(new QWidget(this));
		
		QHBoxLayout* portLayout(new QHBoxLayout(portWidget));
		QHBoxLayout* ipLayout(new QHBoxLayout(ipWidget));
		QHBoxLayout* idLayout(new QHBoxLayout(idWidget));
		QHBoxLayout* seriesLayout(new QHBoxLayout(idWidget));
		QHBoxLayout* numDataLayout(new QHBoxLayout(numDataWidget));
		QHBoxLayout* processLayout(new QHBoxLayout(processWidget));
		QHBoxLayout* buttonLayout(new QHBoxLayout(buttonWidget));
		
		// setModal(true);
		
		portLayout-> addWidget(portLabel);
		portLayout-> addWidget(portSpin_);
		
		ipLayout-> addWidget(ipLabel);
		ipLayout-> addWidget(ipLine_);
		
		idLayout-> addWidget(idLabel);
		idLayout-> addWidget(idCombo_);
		
		numDataLayout-> addWidget(numDataLabel);
		numDataLayout-> addWidget(numDataSpin_);
		
		seriesLayout-> addWidget(seriesLabel);
		seriesLayout-> addWidget(seriesWidget_);
		
		processLayout-> addWidget(processLabel);
		processLayout-> addWidget(processCombo_);
		
		buttonLayout-> addWidget(ok);
		buttonLayout-> addWidget(cancel);
		
		portWidget-> setLayout(portLayout);
		ipWidget-> setLayout(ipLayout);
		idWidget-> setLayout(idLayout);
		seriesWidget-> setLayout(seriesLayout);
		numDataWidget-> setLayout(numDataLayout);
		numDataWidget-> setLayout(seriesLayout);
		processWidget-> setLayout(processLayout);
		buttonWidget-> setLayout(buttonLayout);
		
		layout-> addWidget(settingsLabel);
		layout-> addWidget(portWidget);
		layout-> addWidget(ipWidget);
		layout-> addWidget(dataRequestLabel);
		layout-> addWidget(idWidget);
		layout-> addWidget(seriesWidget);
		layout-> addWidget(numDataWidget);
		layout-> addWidget(seriesWidget);
		layout-> addWidget(processWidget);
		layout-> addWidget(buttonWidget);
		
		setLayout(layout);
		
		QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
		QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	SettingsDialog::~SettingsDialog()
	{
		if(portSpin_ != NULL)
			delete portSpin_;
		
		if(ipLine_ != NULL)
			delete ipLine_;
		
		if(numDataSpin_ != NULL)
			delete numDataSpin_;
	}

	std::map<unsigned int, BaseChart*> SettingsDialog::charts() const
	{
		return viewer_-> charts();
	}

	void SettingsDialog::openSettings()
	{
		show();
	}

	void SettingsDialog::accept()
	{
		bool ip_ok(false);
		
		QString str_ip(ipLine_-> text());
		QHostAddress ip(str_ip);
		unsigned int port(static_cast<unsigned int>(portSpin_-> value()));
		QVariant qid(idCombo_-> currentData());
		unsigned int id(static_cast<unsigned int>(qid.toInt()));
		GraphSeries series(seriesWidget_-> series());
		// unsigned int serie(static_cast<unsigned int>(serieSpin_-> value()));
		unsigned int numData(static_cast<unsigned int>(numDataSpin_-> value()));
		QVariant qProcessID(processCombo_-> currentData());
		ProcessID processID(static_cast<ProcessID>(qProcessID.toInt()));
		
		if(ip.protocol() == QAbstractSocket::IPv4Protocol)
		{
			cout << "The IP address is " << ip.toString().toStdString() << endl;
			
			ip_ok = true;
		}
		else
		{
			cerr << "The input address is not an IPv4 address" << endl;
		}
		
		cout << "The port is " << port << endl;
		cout << "The id is " << id << endl;
		cout << "The series are ";
		
		for(unsigned int i=0; i < series.end(); i++)
		{
			if(i != 0)
			{
				cout << ',';
			}
			
			cout << series.serie(i).serie();
		}
		
		cout << endl;
		
		cout << "The amount of data is " << numData << endl;
		
		cout << "The process is ";
		
		switch(processID)
		{
			case NoProcessID :
			{
				cout << "no process" << endl;
				break;
			}
			
			case AverageID :
			{
				cout << "average" << endl;
				break;
			}
			
			case DownsamplingID :
			{
				cout << "downsampling" << endl;
				break;
			}
			
			default :
			{
				cout << "unknown" << endl;
			}
		}
		
		if(ip_ok)
		{
			emit accepted(ip, port, id, series, numData, processID);
			
			close();
		}
		else
		{
			cerr << "Error" << endl;
			
			parameterError(ip_ok);
		}
	}
	
	void SettingsDialog::parameterError(bool ip_ok)
	{
		QWidget* widget(new QWidget);
		QVBoxLayout* layout(new QVBoxLayout(widget));
		QLabel* label(new QLabel(widget));
		QPushButton* ok(new QPushButton(QString("Ok")));
		
		layout-> addWidget(label);
		layout-> addWidget(ok);
		widget-> setWindowTitle(QString("Error"));
		widget-> setLayout(layout);
		
		if(not ip_ok)
		{
			if(not ip_ok)
			{
				label-> setText(QString("The ip address is incorrect"));
			}
			
			widget-> show();
		}
		
		QObject::connect(ok, SIGNAL(clicked()), widget, SLOT(close()));
	}
	
	// void SettingsDialog::addNewChart(BaseChart* chart)
	// {
		// idCombo_-> addItem((chart-> title()), chart-> id());
	// }
}