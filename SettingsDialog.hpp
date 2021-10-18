#ifndef HPP_SETTINGSDIALOG
#define HPP_SETTINGSDIALOG

#include <iostream>
#include <QDialog>
#include <QtNetwork>
#include <QSpinBox>
#include "Viewer.hpp"
#include "DataRequest.hpp"
#include "SeriesWidget.hpp"

namespace ModelView
{
	class SeriesWidget;
	
	class SettingsDialog : public QDialog
	{
		Q_OBJECT
		
		private :
			Viewer* viewer_;
			QSpinBox* portSpin_;
			QLineEdit* ipLine_;
			// QSpinBox* idSpin_;
			QComboBox* idCombo_;
			// QSpinBox* serieSpin_;
			QSpinBox* numDataSpin_;
			QComboBox* processCombo_;
			SeriesWidget* seriesWidget_;
			
		public :
			SettingsDialog(Viewer*, QWidget* = NULL);
			~SettingsDialog();
		
		private :
			void buildProcessCombo();
			std::map<unsigned int, BaseChart*> charts() const;
		
		signals :
			void accepted(QHostAddress, unsigned int, unsigned int, GraphSeries, unsigned int, ProcessID);
			
		public slots :
			void openSettings();
			void accept();
			void parameterError(bool);
			// void addNewChart(BaseChart*);
			// void addSerie(unsigned int);
	};
}
#endif