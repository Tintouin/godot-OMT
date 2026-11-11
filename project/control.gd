extends Control

const WANTED := "Test Godot"

func _ready():
	var finder = OMTFinder.new()
	add_child(finder)
	await get_tree().create_timer(3.0).timeout

	var chosen := ""
	for s in finder.get_sources():
		if WANTED in s:
			chosen = s
	print("Chosen: ", chosen)
	if chosen == "":
		return

	var player = VideoStreamPlayer.new()
	add_child(player)
	player.size = get_viewport_rect().size
	player.expand = true

	var stream = VideoStreamOMT.new()
	stream.source = chosen
	player.stream = stream
	player.play()
