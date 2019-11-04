// Cell model with visibility state
// Author: Max Schwarz <max.schwarz@online.de>

#include "cell_model.h"

CellModel::CellModel(QObject* parent)
 : QAbstractListModel{parent}
{
}

CellModel::~CellModel()
{
}

void CellModel::setCells(int cellsX, int cellsY)
{
	beginResetModel();
	m_cellsX = cellsX;
	m_cellsY = cellsY;
	m_data.clear();
	m_data.resize(m_cellsX * m_cellsY);
	endResetModel();
}

int CellModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
		return 0;

	return m_cellsX * m_cellsY;
}

QHash<int, QByteArray> CellModel::roleNames() const
{
	QHash<int, QByteArray> names;
	names[VisibleRole] = "cellVisible";
	return names;
}

QVariant CellModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return {};

	if(index.row() < 0 || index.row() >= m_data.size())
		return {};

	switch(role)
	{
		case VisibleRole:
			return m_data[index.row()].visible;
	}

	return {};
}

bool CellModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if(!index.isValid())
		return {};

	if(index.row() < 0 || index.row() >= m_data.size())
		return {};

	switch(role)
	{
		case VisibleRole:
			m_data[index.row()].visible = value.toBool();
			dataChanged(index, index);
			return true;
	}

	return {};
}

