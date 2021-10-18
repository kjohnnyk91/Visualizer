#ifndef HPP_WATERFALLCOLORSWIDGET
#define HPP_WATERFALLCOLORSWIDGET

#include "WaterfallChart.hpp"
#include "NewDialog.hpp"
#include <QColorDialog>

namespace ModelView
{
	class NewDialog;
	
	class WaterfallColorsWidget : public QWidget
	{
		Q_OBJECT
		
		private :
			NewDialog* newDialog_;
			QColor minColor_;
			QColor maxColor_;
			QColorDialog minColorDialog_;
			QColorDialog maxColorDialog_;
			QLabel minColorLabel_;
			QLabel maxColorLabel_;
			QPixmap minColorPixmap_;
			QPixmap maxColorPixmap_;
			
		public :
			WaterfallColorsWidget(NewDialog* newDialog_, QWidget* parent = NULL);
			~WaterfallColorsWidget();
			
			QColor minColor() const;
			QColor maxColor() const;
			
		signals :
			void minColorSet(QColor);
			void maxColorSet(QColor);
			
		public slots :
			inline void setMinColor(const QColor&);
			inline void setMaxColor(const QColor&);
			inline void showMinColorDialog();
			inline void showMaxColorDialog();
	};
	
	inline void WaterfallColorsWidget::setMinColor(const QColor& minColor)
	{
		minColor_ = minColor;
		minColorPixmap_.fill(minColor_);
		minColorLabel_.setPixmap(minColorPixmap_);
	}
	
	inline void WaterfallColorsWidget::setMaxColor(const QColor& maxColor)
	{
		maxColor_ = maxColor;
		maxColorPixmap_.fill(maxColor_);
		maxColorLabel_.setPixmap(maxColorPixmap_);
	}
	
	inline void WaterfallColorsWidget::showMinColorDialog()
	{
		minColorDialog_.show();
	}
	
	inline void WaterfallColorsWidget::showMaxColorDialog()
	{
		maxColorDialog_.show();
	}
}

#endif