extends OMTTestBase

func _ready():
	setup_player()
	await get_tree().create_timer(3.0).timeout
	play_source(find_source("OBS Output"))
	while true:
		await get_tree().create_timer(1.0).timeout
		print("FPS: ", Engine.get_frames_per_second())
