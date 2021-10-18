#ifndef HPP_VIEWERWIDGET
#define HPP_VIEWERWIDGET

#include <QtWidgets>
#include <iostream>
#include "Viewer.hpp"

namespace ModelView
{
	class Viewer;
	
	// enum Edge
	// {
		// Not = 0,
		// Left = 1,
		// Right = 2,
		// Top = 3,
		// Bottom = 4
	// };
	
	class ViewerWidget : public QWidget
	{
		private :
			Viewer* viewer_;
			// Edge edge_;
			
			// inline Edge edge() const;
			// inline void setEdge(Edge);
			
		public :
			ViewerWidget(Viewer*, QWidget* = NULL);
			~ViewerWidget();
			
		protected :
			// void mouseMoveEvent(QMouseEvent*);
			void mousePressEvent(QMouseEvent*);
	};
	
	// inline Edge ViewerWidget::edge() const
	// {
		// return edge_;
	// }
	
	// inline void ViewerWidget::setEdge(Edge edge)
	// {
		// edge_ = edge;
	// }
}
#endif