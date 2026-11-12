class_name OMTTestBase
extends Control

var finder: OMTFinder
var player: VideoStreamPlayer

func setup_player() -> void:
	finder = OMTFinder.new()
	add_child(finder)
	player = VideoStreamPlayer.new()
	add_child(player)
	player.size = get_viewport_rect().size
	player.expand = true

func find_source(part: String) -> String:
	for s in finder.get_sources():
		if part in s:
			return s
	return ""

func play_source(address: String) -> void:
	var stream := VideoStreamOMT.new()
	stream.source = address
	player.stream = stream
	player.play()
