#include "NewDialog.hpp"

namespace ModelView
{
	QWidget* NewDialog::buildButtonsWidget()
	{
		QWidget* buttonWidget(new QWidget(this));
		QPushButton* ok(new QPushButton("Ok", this));
		QPushButton* cancel(new QPushButton("Cancel", this));
		QHBoxLayout* buttonLayout(new QHBoxLayout(buttonWidget));
		
		buttonLayout-> addStretch();
		buttonLayout-> addWidget(ok);
		buttonLayout-> addStretch();
		buttonLayout-> addWidget(cancel);
		buttonLayout-> addStretch();
		
		buttonWidget-> setLayout(buttonLayout);
		
		QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
		QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
		
		return buttonWidget;
	}
	
	void NewDialog::buildTypeCombo()
	{
		typeCombo_-> addItem("Select a chart type", UndefinedType);
		typeCombo_-> addItem("Data rate chart", DataRateType);
		typeCombo_-> addItem("Frequency response chart", FrequencyResponseType);
		typeCombo_-> addItem("Impulse response chart", ImpulseResponseType);
		typeCombo_-> addItem("Likelihood Ratio chart", LikelihoodType);
		typeCombo_-> addItem("IQ chart", IQType);
		typeCombo_-> addItem("Signal Time chart", SignalTimeType);
		typeCombo_-> addItem("Signal Time waterfall chart", SignalTimeWaterfallType);
		typeCombo_-> addItem("Spectrum chart", SpectrumType);
		typeCombo_-> addItem("Serial chart", SerialType);
		typeCombo_-> addItem("Scatter chart", ScatterType);
		typeCombo_-> addItem("Waterfall chart", WaterfallType);
		
		QObject::connect(typeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(update(int)));
	}
	
	void NewDialog::buildMultipleCombo()
	{
		xMultipleCombo_-> addItem("femto", femto);
		xMultipleCombo_-> addItem("pico", pico);
		xMultipleCombo_-> addItem("nano", nano);
		xMultipleCombo_-> addItem("micro", micro);
		xMultipleCombo_-> addItem("milli", milli);
		xMultipleCombo_-> addItem("unity", unity);
		xMultipleCombo_-> addItem("kilo", kilo);
		xMultipleCombo_-> addItem("mega", mega);
		xMultipleCombo_-> addItem("giga", giga);
		xMultipleCombo_-> addItem("tera", tera);
		
		yMultipleCombo_-> addItem("femto", femto);
		yMultipleCombo_-> addItem("pico", pico);
		yMultipleCombo_-> addItem("nano", nano);
		yMultipleCombo_-> addItem("micro", micro);
		yMultipleCombo_-> addItem("milli", milli);
		yMultipleCombo_-> addItem("unity", unity);
		yMultipleCombo_-> addItem("kilo", kilo);
		yMultipleCombo_-> addItem("mega", mega);
		yMultipleCombo_-> addItem("giga", giga);
		yMultipleCombo_-> addItem("tera", tera);
		
		xMultipleCombo_-> setCurrentText(QString("unity"));
		yMultipleCombo_-> setCurrentText(QString("unity"));
	}
	
	void NewDialog::buildUnitCombo()
	{
		xUnitCombo_-> addItem("No unit", No);
		xUnitCombo_-> addItem("second", second);
		xUnitCombo_-> addItem("Volt", Volt);
		xUnitCombo_-> addItem("Ampere", Ampere);
		xUnitCombo_-> addItem("Watt", Watt);
		xUnitCombo_-> addItem("Hertz", Hertz);
		xUnitCombo_-> addItem("meter", meter);
		xUnitCombo_-> addItem("bytes per second", bytesPerSecond);
		
		yUnitCombo_-> addItem("No unit", No);
		yUnitCombo_-> addItem("second", second);
		yUnitCombo_-> addItem("Volt", Volt);
		yUnitCombo_-> addItem("Ampere", Ampere);
		yUnitCombo_-> addItem("Watt", Watt);
		yUnitCombo_-> addItem("Hertz", Hertz);
		yUnitCombo_-> addItem("meter", meter);
		yUnitCombo_-> addItem("bytes per second", bytesPerSecond);
		
		QObject::connect(xUnitCombo_, SIGNAL(currentIndexChanged(int)), xQuantityText_, SLOT(setText(int)));
		QObject::connect(yUnitCombo_, SIGNAL(currentIndexChanged(int)), yQuantityText_, SLOT(setText(int)));
	}
	
	void NewDialog::buildSpin()
	{
		xStepSpin_-> setSingleStep(0.01);
		xStepSpin_-> setRange(0,100);
		xStepSpin_-> setDecimals(3);
		xStepSpin_-> setValue(1);
		
		yStepSpin_-> setSingleStep(0.01);
		yStepSpin_-> setRange(0,100);
		yStepSpin_-> setDecimals(3);
		yStepSpin_-> setValue(1);
		
		// numDataSpin_-> setSingleStep(1);
		// numDataSpin_-> setRange(0,10000000);
		// numDataSpin_-> setValue(100);
		
		xDivisionSpin_-> setRange(0,100);
		yDivisionSpin_-> setRange(0,100);
		
		xDivisionSpin_-> setSingleStep(1);
		xDivisionSpin_-> setRange(1,100);
		yDivisionSpin_-> setSingleStep(1);
		yDivisionSpin_-> setRange(1,100);
		
		xmSpin_-> setSingleStep(1);
		xmSpin_-> setRange(-100,100);
		xmSpin_-> setValue(0);
		
		xMSpin_-> setSingleStep(1);
		xMSpin_-> setRange(-100,100);
		xMSpin_-> setValue(1);
		
		ymSpin_-> setSingleStep(1);
		ymSpin_-> setRange(-100,100);
		ymSpin_-> setValue(0);
		
		yMSpin_-> setSingleStep(1);
		yMSpin_-> setRange(-100,100);
		yMSpin_-> setValue(1);
	}
	
	void NewDialog::buildDivisionSpin()
	{
		// xDivisionSpin_-> setRange(0,100);
		// yDivisionSpin_-> setRange(0,100);
		
		// xDivisionSpin_-> setSingleStep(1);
		// yDivisionSpin_-> setSingleStep(1);
	}
	
	void NewDialog::buildQuantityText()
	{
		xQuantityText_-> setReadOnly(true);
		xQuantityText_-> setFixedHeight(27);
		
		yQuantityText_-> setReadOnly(true);
		yQuantityText_-> setFixedHeight(27);
	}
	
	QWidget* NewDialog::buildXBoundsWidget()
	{
		QWidget* xBoundsWidget(new QWidget(this));
		QHBoxLayout* xBoundsLayout(new QHBoxLayout(xBoundsWidget));
		
		xBoundsLayout-> addWidget(xmSpin_);
		xBoundsLayout-> addWidget(xMSpin_);
		
		xBoundsWidget-> setLayout(xBoundsLayout);
		
		return xBoundsWidget;
	}
	
	QWidget* NewDialog::buildYBoundsWidget()
	{
		QWidget* yBoundsWidget(new QWidget(this));
		QHBoxLayout* yBoundsLayout(new QHBoxLayout(yBoundsWidget));
		
		yBoundsLayout-> addWidget(ymSpin_);
		yBoundsLayout-> addWidget(yMSpin_);
		
		yBoundsWidget-> setLayout(yBoundsLayout);
		
		return yBoundsWidget;
	}
	
	QWidget* NewDialog::buildFormWidget()
	{
		QWidget* formWidget(new QWidget(this));
		QWidget* xBoundsWidget(buildXBoundsWidget());
		QWidget* yBoundsWidget(buildYBoundsWidget());
		buildTypeCombo();
		// buildBoundsWidget();
		buildSpin();
		buildMultipleCombo();
		buildUnitCombo();
		buildDivisionSpin();
		buildQuantityText();
		// buildProcessCombo();
		// colorsLabel_-> hide();
		formLayout_-> addRow(tr("Type"), typeCombo_);
		formLayout_-> addRow(titleLabel_, titleLine_);
		formLayout_-> addRow(xStepLabel_, xStepSpin_);
		formLayout_-> addRow(yStepLabel_, yStepSpin_);
		formLayout_-> addRow(xBoundsLabel_, xBoundsWidget);
		formLayout_-> addRow(yBoundsLabel_, yBoundsWidget);
		formLayout_-> addRow(xMultipleLabel_, xMultipleCombo_);
		formLayout_-> addRow(xUnitLabel_, xUnitCombo_);
		formLayout_-> addRow(yMultipleLabel_, yMultipleCombo_);
		formLayout_-> addRow(yUnitLabel_, yUnitCombo_);
		formLayout_-> addRow(xDivisionLabel_, xDivisionSpin_);
		formLayout_-> addRow(yDivisionLabel_, yDivisionSpin_);
		formLayout_-> addRow(tr("X unit"), xQuantityText_);
		formLayout_-> addRow(tr("Y unit"), yQuantityText_);
		// formLayout_-> addRow(processLabel_, processCombo_);
		// formLayout_-> addRow(numDataLabel_, numDataSpin_);
		
		formWidget-> setLayout(formLayout_);
		
		return formWidget;
	}
	
	NewDialog::NewDialog(Viewer* viewer, QWidget* parent):
		QDialog(parent),
		viewer_(viewer),
		formLayout_(new QFormLayout(this)),
		titleLabel_(NULL),
		xStepLabel_(NULL),
		yStepLabel_(NULL),
		xMultipleLabel_(NULL),
		yMultipleLabel_(NULL),
		xUnitLabel_(NULL),
		yUnitLabel_(NULL),
		xDivisionLabel_(NULL),
		yDivisionLabel_(NULL),
		xBoundsLabel_(NULL),
		yBoundsLabel_(NULL),
		titleLine_(new QLineEdit(this)),
		typeCombo_(new QComboBox(this)),
		xStepSpin_(new QDoubleSpinBox(this)),
		yStepSpin_(new QDoubleSpinBox(this)),
		xMultipleCombo_(new QComboBox(this)),
		yMultipleCombo_(new QComboBox(this)),
		xUnitCombo_(new QComboBox(this)),
		yUnitCombo_(new QComboBox(this)),
		xDivisionSpin_(new QSpinBox(this)),
		yDivisionSpin_(new QSpinBox(this)),
		xQuantityText_(new ViewerTextEdit(QString("None"),this)),
		yQuantityText_(new ViewerTextEdit(QString("None"),this)),
		xmSpin_(new QSpinBox(this)),
		xMSpin_(new QSpinBox(this)),
		ymSpin_(new QSpinBox(this)),
		yMSpin_(new QSpinBox(this)),
		colorsWidget_(new WaterfallColorsWidget(this, parent))
	{
		QVBoxLayout* layout(new QVBoxLayout(this));
		// buildLabels();
		
		try
		{
			titleLabel_ = new QLabel(tr("Title"), this, Qt::Widget);
			xStepLabel_ = new QLabel(tr("X step"), this, Qt::Widget);
			yStepLabel_ = new QLabel(tr("Y step"), this, Qt::Widget);
			xMultipleLabel_ = new QLabel(tr("X multiple"), this, Qt::Widget);
			yMultipleLabel_ = new QLabel(tr("Y multiple"), this, Qt::Widget);
			xUnitLabel_ = new QLabel(tr("X unit"), this, Qt::Widget);
			yUnitLabel_ = new QLabel(tr("Y unit"), this, Qt::Widget);
			xDivisionLabel_ = new QLabel(tr("X division"));
			yDivisionLabel_ = new QLabel(tr("Y division"));
			xBoundsLabel_ = new QLabel(tr("X bounds"), this, Qt::Widget);
			yBoundsLabel_ = new QLabel(tr("Y bounds"), this, Qt::Widget);
			colorsLabel_ = new QLabel(tr("Waterfall colors"), this, Qt::Widget);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "NewDialog::buildLabels() : bad allocation " << ba.what() << std::endl;
		}
		
		colorsLabel_-> hide();
		
		QWidget* formWidget(buildFormWidget());
		QWidget* buttonWidget(buildButtonsWidget());
		setModal(true);
		setWindowTitle("New chart");
		
		layout-> addStretch();
		layout-> addWidget(formWidget);
		layout-> addStretch();
		layout-> addWidget(buttonWidget);
		// layout-> addStretch();
		
		setLayout(layout);
		
		// QObject::connect(this, SIGNAL(paramValid(string, int, Unit, double, Multiple, double, double, unsigned int, Unit, double, Multiple, double, double, unsigned int)), viewer_, SLOT(createNewChart(string, int, Unit, double, Multiple, double, double, unsigned int, Unit, double, Multiple, double, double, unsigned int)));
	}
	
	NewDialog::~NewDialog()
	{
		delete yQuantityText_;
		delete xQuantityText_;
		delete yUnitCombo_;
		delete xUnitCombo_;
		delete yMultipleCombo_;
		delete xMultipleCombo_;
		delete typeCombo_;
		delete titleLine_;
		delete formLayout_;
	}
	
	void NewDialog::accept()
	{
		QString qtitle(titleLine_-> text());
		QVariant qType(typeCombo_-> currentData());
		QVariant qxMultiple(xMultipleCombo_-> currentData());
		QVariant qyMultiple(yMultipleCombo_-> currentData());
		QVariant qxUnit(xUnitCombo_-> currentData());
		QVariant qyUnit(yUnitCombo_-> currentData());
		
		double xStep(xStepSpin_-> value());
		double yStep(yStepSpin_-> value());
		
		unsigned int xDivision(static_cast<unsigned int>(xDivisionSpin_-> value()));
		unsigned int yDivision(static_cast<unsigned int>(yDivisionSpin_-> value()));
		
		int xm(xmSpin_-> value());
		int xM(xMSpin_-> value());
		int ym(ymSpin_-> value());
		int yM(yMSpin_-> value());
		
		string title(qtitle.toStdString());
		DataType type(static_cast<DataType>(qType.toInt()));
		Multiple xMultiple(static_cast<Multiple>(qxMultiple.toInt()));
		Multiple yMultiple(static_cast<Multiple>(qyMultiple.toInt()));
		Unit xUnit(static_cast<Unit>(qxUnit.toInt()));
		Unit yUnit(static_cast<Unit>(qyUnit.toInt()));
		
		QColor minColor(colorsWidget_-> minColor());
		QColor maxColor(colorsWidget_-> maxColor());
		
		if((not qtitle.isEmpty()) and (type != UndefinedType) and (xm < xM) and (ym < yM))
		{
			emit paramValid(title, type, 
										xUnit, xStep, xMultiple, xm, xM, xDivision, 
										yUnit, yStep, yMultiple, ym, yM, yDivision, 
										minColor, maxColor);
			close();
		}
		else
		{
			if(type == UndefinedType)
			{
				QMessageBox error;
				error.setText(QString("The chart type is not set"));
				error.exec();
			}
			else
			{
				QMessageBox error;
				error.setText(QString("The title is not set"));
				error.exec();
			}
		}
	}
	
	void NewDialog::update(int typeIndex)
	{
		DataType type(static_cast<DataType>(typeIndex));
		Quantity xquantity(None);
		Quantity yquantity(None);
		
		switch(type)
		{
			case SignalTimeWaterfallType :
			case WaterfallType :
			{
				if(colorsLabel_-> isHidden())
				{
					colorsLabel_-> show();
					formLayout_-> addRow(colorsLabel_, colorsWidget_);
				}
				
				break;
			}
			
			default :
			{
				if(not colorsLabel_-> isHidden())
				{
					colorsLabel_-> hide();
					formLayout_-> takeRow(colorsWidget_);
				}
				
				break;
			}
		}
		
		switch(type)
		{
			case DataRateType :
			{
				titleLine_-> setText(QString("Data rate"));
				xquantity = Time;
				yquantity = DataRate;
				xUnitCombo_-> setCurrentText(QString("second"));
				yUnitCombo_-> setCurrentText(QString("bytes per second"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case FrequencyResponseType :
			{
				titleLine_-> setText(QString("Frequency response"));
				xquantity = Frequency;
				yquantity = Gain;
				xUnitCombo_-> setCurrentText(QString("Hertz"));
				yUnitCombo_-> setCurrentText(QString("No unit"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case ImpulseResponseType :
			{
				titleLine_-> setText(QString("Impulse response"));
				xquantity = Frequency;
				yquantity = Gain;
				xUnitCombo_-> setCurrentText(QString("Hertz"));
				yUnitCombo_-> setCurrentText(QString("No unit"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case LikelihoodType :
			{
				titleLine_-> setText(QString("Likelihood ratio"));
				xquantity = Time;
				yquantity = Likelihood;
				xUnitCombo_-> setCurrentText(QString("second"));
				yUnitCombo_-> setCurrentText(QString("No unit"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case IQType :
			{
				titleLine_-> setText(QString("IQ constellation"));
				xquantity = Voltage;
				yquantity = Voltage;
				xUnitCombo_-> setCurrentText(QString("Volt"));
				yUnitCombo_-> setCurrentText(QString("Volt"));
				xmSpin_-> setValue(-5);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(-5);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case SerialType :
			{
				titleLine_-> setText(QString("Serial chart"));
				xquantity = None;
				yquantity = None;
				xUnitCombo_-> setCurrentText(QString("No unit"));
				yUnitCombo_-> setCurrentText(QString("No unit"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case ScatterType :
			{
				titleLine_-> setText(QString("Scatter chart"));
				xquantity = None;
				yquantity = None;
				xUnitCombo_-> setCurrentText(QString("No unit"));
				yUnitCombo_-> setCurrentText(QString("No unit"));
				xmSpin_-> setValue(-5);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(-5);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case SignalTimeType :
			{
				titleLine_-> setText(QString("Signal time"));
				xquantity = Time;
				yquantity = Power;
				xUnitCombo_-> setCurrentText(QString("second"));
				yUnitCombo_-> setCurrentText(QString("Watt"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case SpectrumType :
			{
				titleLine_-> setText(QString("Spectrum"));
				xquantity = Frequency;
				yquantity = Voltage;
				xUnitCombo_-> setCurrentText(QString("Hertz"));
				yUnitCombo_-> setCurrentText(QString("Volt"));
				xmSpin_-> setValue(0);
				xMSpin_-> setValue(5);
				ymSpin_-> setValue(0);
				yMSpin_-> setValue(5);
				
				break;
			}
			
			case SignalTimeWaterfallType :
			{
				titleLine_-> setText(QString("Waterfall"));
				xquantity = Time;
				yquantity = Time;
				xUnitCombo_-> setCurrentText(QString("second"));
				yUnitCombo_-> setCurrentText(QString("second"));
				
				break;
			}
			
			case WaterfallType :
			{
				titleLine_-> setText(QString("Waterfall"));
				xquantity = Time;
				yquantity = Power;
				xUnitCombo_-> setCurrentText(QString("second"));
				yUnitCombo_-> setCurrentText(QString("Watt"));
				// formLayout_-> addRow(colorsWidget_);
				
				break;
			}
			
			default :
			{
				titleLine_-> clear();
				xquantity = None;
				yquantity = None;
				xUnitCombo_-> setCurrentText(QString("No unit"));
				yUnitCombo_-> setCurrentText(QString("No unit"));
				
				break;
			}
		}
		
		xQuantityText_-> setText(QString(quantityString(xquantity).c_str()));
		yQuantityText_-> setText(QString(quantityString(yquantity).c_str()));
	}
}