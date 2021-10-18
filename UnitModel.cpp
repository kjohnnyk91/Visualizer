#include "UnitModel.hpp"

namespace ModelView
{
	UnitModel::UnitModel(QObject* parent):
		QStringListModel(parent)
	{}

	UnitModel::~UnitModel()
	{}

	int UnitModel::rowCount(const QModelIndex& index) const
	{
		Q_UNUSED(index);
		
		return 10;
	}

	QVariant UnitModel::data(const QModelIndex& index, int role) const
	{
		Q_UNUSED(index)
		
		std::map<int, Unit> unitMap = 
		{
			{0, No}, 
			{1, second}, 
			{2, Volt}, 
			{3, Ampere}, 
			{4, Watt},
			{5, Hertz},
			{6, meter},
			{7, decibel},
			{8, decibelm},
			{9, bytesPerSecond}
		};
		
		if(role == Qt::DisplayRole)
		{
			return QString("1");
			// return QString(unitString(unitMap.at(index.row())).c_str());
		}
		else
		{
			return QString();
		}
	}
}