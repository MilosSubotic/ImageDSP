
#ifndef PROJECT_H_
#define PROJECT_H_

#include <QObject>
#include <QString>
#include <QModelIndex>
#include <QVector>

class QAbstractTableModel;
class TableModel;
class QSortFilterProxyModel;

class Project : public QObject {
	Q_OBJECT

public:
	explicit Project(const QString& fileName, QObject* parent = 0);
	~Project() {}

	QAbstractItemModel* getProgsModel() const;
	QAbstractItemModel* getParamsModel() const;

public slots:
	void currentProgChanged(const QModelIndex& index);

private:
	QString fileName;

	TableModel* progsModel;
	struct ProgSetup {
		TableModel* paramsModel; // double min, double max, int current.
	};
	QVector<ProgSetup> progSetups;
	QSortFilterProxyModel* paramsProxyModel;

	void appendProg(const QString& name);
};

#endif // PROJECT_H_
