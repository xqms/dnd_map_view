import QtQuick 2.0
import presenter 1.0
import QtMultimedia 5.8

Item {
	id: mediaItem
	property bool isVideo: controller.videoURL.length != 0
	property int mediaWidth: isVideo ? videoView.sourceRect.width : imageView.imageWidth
	property int mediaHeight: isVideo ? videoView.sourceRect.height : imageView.imageHeight

	property real scale: Math.min(width / mediaWidth, height / mediaHeight)
	property real scaledWidth: mediaWidth * scale
	property real scaledHeight: mediaHeight * scale
	property variant mediaRect: Qt.rect((width - scaledWidth)/2, (height-scaledHeight)/2, scaledWidth, scaledHeight)

	property bool muted: false

	Item {
		id: contentContainer
		x: mediaItem.mediaRect.x
		y: mediaItem.mediaRect.y
		width: mediaItem.mediaRect.width
		height: mediaItem.mediaRect.height

		ImageView {
			id: imageView
			anchors.fill: parent
			image: controller.image
			visible: !mediaItem.isVideo
		}

		MediaPlayer {
			id: player
			source: controller.videoURL
			loops: MediaPlayer.Infinite
			autoPlay: true
			muted: mediaItem.muted

			onError: function(error, errorString){
				console.log("playback error:", error, errorString);
				Qt.quit()
			}
		}
		VideoOutput {
			id: videoView
			anchors.fill: parent
			source: player
			visible: mediaItem.isVideo
		}
	}
}
