#include "ViewerTextEdit.hpp"

namespace ModelView
{
	ViewerTextEdit::ViewerTextEdit(QWidget* parent) :
		QTextEdit(parent)
	{}
	
	ViewerTextEdit::ViewerTextEdit(const QString& text, QWidget* parent) :
		QTextEdit(text, parent)
	{}
	
	ViewerTextEdit::~ViewerTextEdit()
	{}
	
	void ViewerTextEdit::setText(int index)
	{
		Quantity quantity(static_cast<Quantity>(index));
		const QString text(quantityString(quantity).c_str());
		
		QTextEdit::setText(text);
	}
}