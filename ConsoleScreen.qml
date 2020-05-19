
import QtQuick 2.5
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.13
import presenter 1.0

Rectangle {
	anchors.fill: parent
	color: "black"
	id: container
	Material.theme: Material.Dark

// 	Rectangle {
// 		anchors.fill: buttonslayout
// 		color: "white"
// 	}

	RowLayout {
		id: buttonslayout
		width: parent.width
		RadioButton {
			id: phase_grid
			antialiasing: true
			checked: true
			text: "1) Grid"
		}
		RadioButton {
			id: phase_viewport
			antialiasing: true
			text: "2) Viewport"
		}
		RadioButton {
			id: phase_visibility
			antialiasing: true
			text: "3) Visibility"
		}

		Item {
			Layout.fillWidth: true
		}

		Button {
			text: "Uncover all"
			onClicked: function() {
				controller.cellModel.uncoverAll();
			}
		}
		Button {
			text: "Cover all"
			onClicked: function() {
				controller.cellModel.coverAll();
			}
		}

		CheckBox {
			text: "Display Grid"
			Component.onCompleted: {
				// Load saved values
				checked = controller.displayGrid;

				// From now on, we are driving!
				controller.displayGrid = Qt.binding(function() { return checked });
			}
		}

		Item {
			Layout.fillWidth: true
		}

		Button {
			text: "Reset view"
			onClicked: function() {
				imageFlick.zoom = 1.0;
				imageView.x = 0;
				imageView.y = 0;
			}
		}
	}

	property real imgScale: Math.max(imageView.mediaRect.width, imageView.mediaRect.height)

	Item {
		id: imageFlick
		anchors.top: buttonslayout.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		property real zoom: 1.0

		clip: true

		function zoomWithAt(zoomFactor, x, y) {
			var inImageSpaceX = x - imageView.x;
			var inImageSpaceY = y - imageView.y;

			zoom = zoomFactor * zoom;

			var newImageSpaceX = zoomFactor * inImageSpaceX;
			var newImageSpaceY = zoomFactor * inImageSpaceY;

			// The point at newImageSpaceX needs to appear at x,y
			imageView.x = x - newImageSpaceX
			imageView.y = y - newImageSpaceY
		}

		function zoomInAt(x,y) {
			zoomWithAt(1.0/0.8, x, y);
		}
		function zoomOutAt(x,y) {
			zoomWithAt(0.8, x, y);
		}

		MouseArea
		{
			z: -400
			anchors.fill: parent
			drag.target: imageView
			onWheel: function(wheel) {
				if(wheel.angleDelta.y > 0)
					imageFlick.zoomInAt(wheel.x, wheel.y);
				else
					imageFlick.zoomOutAt(wheel.x, wheel.y);
			}
		}

		MediaItem {
			id: imageView
			width: imageFlick.zoom * imageFlick.width
			height: imageFlick.zoom * imageFlick.height

			Item {
				id: imageSpace
				x: imageView.mediaRect.x
				y: imageView.mediaRect.y
				width: imageView.mediaWidth / Math.max(imageView.mediaWidth, imageView.mediaHeight)
				height: imageView.mediaHeight / Math.max(imageView.mediaWidth, imageView.mediaHeight)
				scale: imageView.mediaRect.width / width
				transformOrigin: Item.TopLeft

				// Cells
				Item {
					id: cellsRect

					property int cellsX: 10
					property int cellsY: 10
					property real cellWidth: width / cellsX
					property real cellHeight: height / cellsY
					property real cellBorder: 1.0 / imgScale

					property bool currentlyMarking: false

					x: 0.3
					y: 0.3
					width: 0.5
					height: 0.5

					Component.onCompleted: {
						// Load saved values
						x = controller.cellsRect.x;
						y = controller.cellsRect.y;
						width = controller.cellsRect.width;
						height = controller.cellsRect.height;
						cellsX = controller.cellsX;
						cellsY = controller.cellsY;

						// From now on, we are driving!
						controller.cellsRect = Qt.binding(function() { return Qt.rect(
							x, y, width, height
						) });
						controller.cellsX = Qt.binding(function() { return cellsX; });
						controller.cellsY = Qt.binding(function() { return cellsY; });
					}

					Repeater
					{
						id: cellsView
						model: controller.cellModel

						delegate: Rectangle {
							id: delegate
							width: cellsRect.cellWidth + cellsRect.cellBorder
							height: cellsRect.cellHeight + cellsRect.cellBorder
							x: (index % cellsRect.cellsX) * cellsRect.cellWidth - cellsRect.cellBorder/2
							y: Math.floor(index / cellsRect.cellsX) * cellsRect.cellHeight - cellsRect.cellBorder/2

							function mark(value) {
								cellVisible = value;
							}
							property bool cellVisibleRead: (typeof cellVisible !== 'undefined') ? cellVisible : false;

							color: {
								if(phase_visibility.checked)
									return cellVisible ? "green" : "red";
								else
									return "transparent";
							}
							opacity: {
								if(phase_visibility.checked)
									return cellVisible ? 0.3 : 0.5;
								else
									return 1.0;
							}
							border.width: phase_grid.checked ? (1.0 / imgScale) : 0
							border.pixelAligned: false
							border.color: Material.color(Material.Cyan)
							antialiasing: true
						}
					}

					// Top-right handle
					Rectangle
					{
						visible: phase_grid.checked
						width: 10 / imgScale
						height: 10 / imgScale
						x: -width/2
						y: -height/2
						radius: width/2

						MouseArea
						{
							property point topLeft
							property point bottomRight
							property point dragStart

							anchors.fill: parent
							cursorShape: Qt.SizeFDiagCursor

							onPressed: function(mouse) {
								dragStart = mapToItem(imageSpace, mouse.x, mouse.y);
								topLeft = Qt.point(cellsRect.x, cellsRect.y);
								bottomRight = Qt.point(cellsRect.x + cellsRect.width, cellsRect.y + cellsRect.height);
							}

							onPositionChanged: function(mouse) {
								var pos = mapToItem(imageSpace, mouse.x, mouse.y);
								var fpos = Qt.point(
									topLeft.x + (pos.x - dragStart.x),
									topLeft.y + (pos.y - dragStart.y)
								);

								cellsRect.x = fpos.x;
								cellsRect.y = fpos.y;
								cellsRect.width = bottomRight.x - fpos.x;
								cellsRect.height = bottomRight.y - fpos.y;
							}
						}
					}

					// Bottom-right handle
					Rectangle
					{
						visible: phase_grid.checked
						width: 10 / imgScale
						height: 10 / imgScale
						x: cellsRect.width - width/2
						y: cellsRect.height - height/2
						radius: width/2

						MouseArea
						{
							property point bottomRight
							property point dragStart

							anchors.fill: parent
							cursorShape: Qt.SizeFDiagCursor

							onPressed: function(mouse) {
								dragStart = mapToItem(imageSpace, mouse.x, mouse.y);
								bottomRight = Qt.point(cellsRect.x + cellsRect.width, cellsRect.y + cellsRect.height);
							}

							onPositionChanged: function(mouse) {
								var pos = mapToItem(imageSpace, mouse.x, mouse.y);

								var fpos = Qt.point(
									bottomRight.x + (pos.x - dragStart.x),
									bottomRight.y + (pos.y - dragStart.y)
								);

								cellsRect.width = fpos.x - cellsRect.x;
								cellsRect.height = fpos.y - cellsRect.y;
							}
						}
					}

					// Y +/-
					Rectangle
					{
						visible: phase_grid.checked
						anchors.right: cellsRect.left
						anchors.rightMargin: cellsRect.cellBorder/2
						anchors.verticalCenter: cellsRect.verticalCenter
						width: 30.0 / imgScale
						height: 60.0 / imgScale
						color: Material.background
						radius: 3.0 / imgScale

						Column
						{
							anchors.centerIn: parent
							id: column

							Item {
								width: 30.0 / imgScale
								height: 30.0 / imgScale
								Item {
									width: 30.0
									height: 30.0
									scale: 1.0 / imgScale
									transformOrigin: Item.TopLeft
									RoundButton {
										anchors.fill: parent
										text: "-"
										onClicked: { if(cellsRect.cellsY > 1) cellsRect.cellsY--; }
									}
								}
							}
							Item {
								width: 30.0 / imgScale
								height: 30.0 / imgScale
								Item {
									width: 30.0
									height: 30.0
									scale: 1.0 / imgScale
									transformOrigin: Item.TopLeft
									RoundButton {
										anchors.fill: parent
										text: "+"
										onClicked: { cellsRect.cellsY++; }
									}
								}
							}
						}
					}

					// X +/-
					Rectangle
					{
						visible: phase_grid.checked
						anchors.bottom: cellsRect.top
						anchors.bottomMargin: cellsRect.cellBorder/2
						anchors.horizontalCenter: cellsRect.horizontalCenter
						width: 60.0 / imgScale
						height: 30.0 / imgScale
						color: Material.background
						radius: 3.0 / imgScale

						Row
						{
							anchors.centerIn: parent

							Item {
								width: 30.0 / imgScale
								height: 30.0 / imgScale
								Item {
									width: 30.0
									height: 30.0
									scale: 1.0 / imgScale
									transformOrigin: Item.TopLeft
									RoundButton {
										anchors.fill: parent
										text: "-"
										onClicked: { if(cellsRect.cellsX > 1) cellsRect.cellsX--; }
									}
								}
							}
							Item {
								width: 30.0 / imgScale
								height: 30.0 / imgScale
								Item {
									width: 30.0
									height: 30.0
									scale: 1.0 / imgScale
									transformOrigin: Item.TopLeft
									RoundButton {
										anchors.fill: parent
										text: "+"
										onClicked: { cellsRect.cellsX++; }
									}
								}
							}
						}
					}
				}

				// This has to be external so that we can track mouse events
				// over multiple cells. It also has to be *outside* of cellsRect
				// so that cellsRect.childAt() works correctly.
				MouseArea
				{
					anchors.fill: cellsRect
					z: -1

					enabled: phase_visibility.checked
					visible: phase_visibility.checked

					acceptedButtons: Qt.LeftButton

					property bool markValue

					onPositionChanged: function(mouse) {
						var item = cellsRect.childAt(mouse.x, mouse.y);
						if(!item)
							return;

						item.mark(markValue);
					}
					onPressed: function(mouse) {
						var item = cellsRect.childAt(mouse.x, mouse.y);
						if(!item)
							return;

						markValue = !item.cellVisibleRead;
						item.mark(markValue);
					}
				}

				// Presenter viewport
				Item {
					id: presenterRect
					visible: phase_viewport.checked || phase_visibility.checked

					width: 0.5
					height: width / controller.presenterAspectRatio

					x: 0.1
					y: 0.1

					Component.onCompleted: {
						// Load saved values
						x = controller.rectPos.x;
						y = controller.rectPos.y;
						width = controller.rectWidth;

						// From now on, we are driving!
						controller.rectPos = Qt.binding(function(){ return Qt.point(presenterRect.x, presenterRect.y) });
						controller.rectWidth = Qt.binding(function(){ return presenterRect.width; });
					}

					Rectangle
					{
						id: presenterVis
						anchors.fill: parent
						color: phase_viewport.checked ? "white" : "transparent"
						opacity: 0.4

						border.width: 2.0 / imgScale
						border.color: "red"
						border.pixelAligned: false
					}

					MouseArea {
						property bool dragging: false

						enabled: phase_viewport.checked
						visible: phase_viewport.checked

						anchors.fill: parent
						cursorShape: dragging ? Qt.ClosedHandCursor : Qt.OpenHandCursor

						drag.target: presenterRect
						drag.axis: Drag.XAndYAxis
						drag.threshold: 1.0 / imgScale

						onPressed: {
							dragging = true;
						}

						onReleased: {
							dragging = false;
						}
					}

					Rectangle {
						id: resizeHandle
						visible: phase_viewport.checked

						width: 10.0 / imgScale
						height: width

						x: parent.width - presenterVis.border.width/2 - width/2
						y: parent.height - presenterVis.border.width/2 - height/2

						radius: 0.5 * width

						color: "white"

						MouseArea
						{
							property bool dragging: false
							property point startPos
							property real startWidth

							anchors.fill: parent
							cursorShape: Qt.SizeFDiagCursor

							onPressed: function(mouse) {
								startPos = mapToItem(imageSpace, mouse.x, mouse.y);
								startWidth = presenterRect.width;
								dragging = true;
							}
							onReleased: {
								dragging = false;
							}
							onPositionChanged: function(mouse) {
								var pos = mapToItem(imageSpace, mouse.x, mouse.y);

								var widthChange = pos.x - startPos.x;
								presenterRect.width = startWidth + widthChange;
							}
						}
					}
				}
			}
		}
	}
}
