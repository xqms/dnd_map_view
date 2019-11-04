// D&D map viewer
// Author: Max Schwarz <max.schwarz@online.de>

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include <iostream>
#include <memory>

#include "view_controller.h"
#include "image_view.h"

static const std::map<QString, int> EXTERNAL_SCREEN_SCORE{
	{"eDP", -100}, // Most notebook displays
	{"HDMI", 50},
	{"VGA", 100},
	{"DVI", 0},
	{"DP", -50},
};

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QQuickStyle::setStyle("Material");

	if(app.arguments().count() < 2)
	{
		std::cerr << "Usage: dnd_map_view <PDF or image>\n";
		return 1;
	}

	QString file = app.arguments().at(1);
	qDebug() << "Opening" << file;

	using ScoredScreen = std::pair<QScreen*, int>;
	std::vector<ScoredScreen> scoredScreens;

	for(auto& screen : app.screens())
	{
		int score = 0;
		for(auto& rule : EXTERNAL_SCREEN_SCORE)
		{
			if(screen->name().contains(rule.first))
			{
				score += rule.second;
			}
		}

		scoredScreens.emplace_back(screen, score);
	}

	if(scoredScreens.empty())
	{
		std::cerr << "Could not find presentation screen\n";
		return 1;
	}

	std::sort(scoredScreens.begin(), scoredScreens.end(), [](const ScoredScreen& a, const ScoredScreen& b){
		return a.second < b.second;
	});

	for(auto& sc : scoredScreens)
	{
		qDebug() << "Screen" << sc.first->name() << "has score" << sc.second;
	}

	QScreen* presentationScreen = scoredScreens.back().first;
	QScreen* consoleScreen = scoredScreens.front().first;

	ViewController controller{file};

	qmlRegisterType<ImageView>("presenter", 1, 0, "ImageView");

	QQuickView presenterView;
	presenterView.setScreen(presentationScreen);
	presenterView.setGeometry(presentationScreen->geometry());
	presenterView.setTitle("D&D Map View - Presenter");
	presenterView.showFullScreen();
	presenterView.engine()->rootContext()->setContextProperty("controller", &controller);
	presenterView.setSource(QUrl("../PresenterScreen.qml"));

	QObject::connect(&presenterView, &QQuickView::widthChanged, &controller, &ViewController::setPresenterWidth);
	QObject::connect(&presenterView, &QQuickView::heightChanged, &controller, &ViewController::setPresenterHeight);
	controller.setPresenterWidth(presentationScreen->geometry().width());
	controller.setPresenterHeight(presentationScreen->geometry().height());

	QQuickView consoleView;
	consoleView.setScreen(consoleScreen);

	consoleView.engine()->rootContext()->setContextProperty("controller", &controller);
	consoleView.setSource(QUrl("../ConsoleScreen.qml"));

	auto screenGeom = consoleScreen->geometry();
	qDebug() << "Console screen geometry: " << screenGeom;
	QSize size(800, 600);
	QRect geometry(
		screenGeom.center() - QPoint(size.width()/2, size.height()/2),
		size
	);
	qDebug() << "Console window: " << geometry;
	consoleView.setGeometry(geometry);
	consoleView.setTitle("D&D Map View - Console");
	consoleView.show();

	QObject::connect(&consoleView, SIGNAL(closing(QQuickCloseEvent*)), &app, SLOT(quit()));

	return app.exec();
}
