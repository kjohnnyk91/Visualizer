#include "ViewerWidget.hpp"

using namespace std;

namespace ModelView
{
	ViewerWidget::ViewerWidget(Viewer* viewer, QWidget* parent) :
		QWidget(parent),
		viewer_(viewer)
	{
		setMouseTracking(true);
	}

	ViewerWidget::~ViewerWidget()
	{}

	// void ViewerWidget::mouseMoveEvent(QMouseEvent* event)
	// {
		// Q_UNUSED(event)
	// }
		
	void ViewerWidget::mousePressEvent(QMouseEvent* event)
	{
		if(event-> button() == Qt::LeftButton)
		{
			cout << "ViewerWidget::Mouse local position : (" << event-> x() << ',' << event-> y() << ')' << endl;
		}
	}
}