#include "Viewer.hpp"
#include <QApplication>
#include <iostream>
#include <QColorDialog>
#include "WaterfallColorsWidget.hpp"
#include "RenameDialog.hpp"
#include "TreeModel.hpp"

using namespace ModelView;

int main(int argc, char** argv)
{
	if(getenv("LANG") != NULL)
		cout << "The locale environment is " << getenv("LANG") << endl;
	
	setenv("LANG", "en_US.utf8",1);
	
	cout << "The locale environment has been set to " << getenv("LANG") << endl;
	
	QApplication app(argc, argv);

	Viewer viewer(&app);
	viewer.show();
	
	// TreeModel model(QString("Hi \t hi \t hi \n Ho \t ho \t ho \n Hu \t hu \t hu "), NULL);
	
	// QTreeView view;
	// view.setModel(&model);
	// view.show();
	
	// TreeItem* root = new TreeItem({QString("Hi"), QString("Hi"), QString("Hi")});
	// TreeItem* child = new TreeItem({QString("Ho"), QString("Ho"), QString("Ho")}, root);
	// TreeItem* child2 = new TreeItem({QString("Hu"), QString("Hu"), QString("Hu")}, child);
	// TreeItem* child3 = new TreeItem({QString("Ha"), QString("Ha"), QString("Ha")}, child2);
	
	// TreeModel model(root);
	
	// QTreeView view;
	// view.setModel(&model);
	// view.show();
	
	return app.exec();
}