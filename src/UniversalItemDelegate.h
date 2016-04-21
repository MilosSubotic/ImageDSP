#ifndef UNIVERSALITEMDELEGATE_H_
#define UNIVERSALITEMDELEGATE_H_

#include <QItemDelegate>

class UniversalItemDelegate : public QItemDelegate {
Q_OBJECT

public:
	explicit UniversalItemDelegate(QObject* parent = 0);
	~UniversalItemDelegate() {
	}
};

///////////////////////////////////////////////////////////////////////////////

#endif // UNIVERSALITEMDELEGATE_H_
