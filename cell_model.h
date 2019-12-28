// Cell model with visibility state
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef CELL_MODEL_H
#define CELL_MODEL_H

#include <QAbstractListModel>

class QSettings;

class CellModel : public QAbstractListModel
{
Q_OBJECT
public:
	enum CellRoles {
		VisibleRole = Qt::UserRole + 1,
	};

	CellModel(QObject* parent = 0);
	virtual ~CellModel();

	void setCells(int cellsX, int cellsY);

	QHash<int, QByteArray> roleNames() const override;

	int rowCount(const QModelIndex & parent) const override;
	QVariant data(const QModelIndex & index, int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	void save(QSettings& settings);
	void restore(const QSettings& settings);

public Q_SLOTS:
	void uncoverAll();
	void coverAll();

private:
	struct Data
	{
		bool visible = false;
	};

	int m_cellsX = 0;
	int m_cellsY = 0;
	QVector<Data> m_data;
};

#endif
