// Cell model with visibility state
// Author: Max Schwarz <max.schwarz@online.de>

#include "cell_model.h"

#include <QDebug>
#include <QSettings>

CellModel::CellModel(QObject* parent)
 : QAbstractListModel{parent}
{
	setCells(10, 10);
}

CellModel::~CellModel()
{
}

void CellModel::setCells(int cellsX, int cellsY)
{
	if(cellsX == m_cellsX && cellsY == m_cellsY)
		return;

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

void CellModel::coverAll()
{
	for(auto& cell : m_data)
		cell.visible = false;

	dataChanged(index(0), index(m_data.size()-1));
}

void CellModel::uncoverAll()
{
	for(auto& cell : m_data)
		cell.visible = true;

	dataChanged(index(0), index(m_data.size()-1));
}

void CellModel::save(QSettings& settings)
{
	QString v;
	for(auto& cell : m_data)
		v += cell.visible ? '1' : '0';

	settings.setValue("cells", v);
}

void CellModel::restore(const QSettings& settings)
{
	QString v = settings.value("cells", "").toString();
	if(v.size() != m_data.size())
	{
		qDebug() << "Ignoring wrong cells (expected " << m_data.size() << ")";
		return;
	}

	for(int i = 0; i < m_data.size(); ++i)
	{
		m_data[i].visible = (v[i] == '1');
	}
	dataChanged(index(0), index(m_data.size()-1));
}


