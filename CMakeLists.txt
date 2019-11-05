
cmake_minimum_required(VERSION 3.0)
project(dnd_map_view)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

find_path(POPPLER_INCLUDE_DIRS poppler-qt5.h HINTS /usr/include/poppler/qt5)
find_library(POPPLER_LIBRARIES poppler-qt5)

include_directories(${POPPLER_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp")

qt5_wrap_cpp(MOC_SRCS
	image_view.h
	view_controller.h
	renderer.h
	cell_model.h
)

add_executable(dnd_map_view
	${MOC_SRCS}
	dnd_map_view.cpp
	image_view.cpp
	view_controller.cpp
	renderers/pdf_renderer.cpp
	renderers/image_renderer.cpp
	cell_model.cpp
)
target_link_libraries(dnd_map_view
	Qt5::Widgets
	Qt5::Quick
	Qt5::QuickControls2
	${POPPLER_LIBRARIES}
)

