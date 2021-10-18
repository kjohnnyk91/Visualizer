#ifndef HPP_NEWDIALOG
#define HPP_NEWDIALOG

#include <QtWidgets>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "Graph.hpp"
#include "ViewerTextEdit.hpp"
#include "Viewer.hpp"
#include "WaterfallColorsWidget.hpp"

// #define DATARATE_INDEX 1
// #define FREQRESPONSE_INDEX 2
// #define IMPULSERESPONSE 3
// #define LIKELIRATIO_INDEX 4
// #define IQ_INDEX 5
// #define SCATTER_INDEX 6
// #define SERIAL_INDEX 7
// #define SIGNALTIME_INDEX 8
// #define SPECTRUM_INDEX 9
// #define WATERFALL_INDEX 10

namespace ModelView
{
	class Viewer;
	class WaterfallColorsWidget;
	
	class NewDialog : public QDialog
	{
		Q_OBJECT
		
		private :
			Viewer* viewer_;
			QFormLayout* formLayout_;
			
			QLabel* titleLabel_;
			QLabel* xStepLabel_;
			QLabel* yStepLabel_;
			QLabel* xMultipleLabel_;
			QLabel* yMultipleLabel_;
			QLabel* xUnitLabel_;
			QLabel* yUnitLabel_;
			QLabel* xDivisionLabel_;
			QLabel* yDivisionLabel_;
			QLabel* xBoundsLabel_;
			QLabel* yBoundsLabel_;
			QLabel* colorsLabel_;
			// QLabel* processLabel_;
			// QLabel* numDataLabel_;
			
			QLineEdit* titleLine_;
			QComboBox* typeCombo_;
			QDoubleSpinBox* xStepSpin_;
			QDoubleSpinBox* yStepSpin_;
			QComboBox* xMultipleCombo_;
			QComboBox* yMultipleCombo_;
			QComboBox* xUnitCombo_;
			QComboBox* yUnitCombo_;
			QSpinBox* xDivisionSpin_;
			QSpinBox* yDivisionSpin_;
			QTextEdit* xQuantityText_;
			QTextEdit* yQuantityText_;
			QSpinBox* xmSpin_;
			QSpinBox* xMSpin_;
			QSpinBox* ymSpin_;
			QSpinBox* yMSpin_;
			WaterfallColorsWidget* colorsWidget_;
			// QComboBox* processCombo_;
			// QSpinBox* numDataSpin_;
			
		private :
			// void buildLabels();
			QWidget* buildButtonsWidget();
			void buildTypeCombo();
			void buildMultipleCombo();
			void buildUnitCombo();
			void buildSpin();
			void buildDivisionSpin();
			void buildQuantityText();
			// void buildProcessCombo();
			QWidget* buildXBoundsWidget();
			QWidget* buildYBoundsWidget();
			QWidget* buildFormWidget();
			
		public :
			NewDialog(Viewer*, QWidget* = NULL);
			~NewDialog();
			
		signals :
			void paramValid(string title, DataType type, 
						Unit xUnit, double xStep, Multiple xmultiple, int xm, int xM, unsigned int xDivision, 
						Unit yUnit, double yStep, Multiple yMultiple, int ym, int yM, unsigned int yDivision,
						QColor minColor, QColor maxColor);
			
		public slots :
			void accept();
			void update(int);
	};
}

#endif