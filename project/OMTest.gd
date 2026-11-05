extends Node3D

func _ready():
	var finder = OMTFinder.new()
	add_child(finder)
	print("Finder lancé")
	await get_tree().create_timer(3.0).timeout
	finder.refresh()
	print("Sources : ", finder.get_sources())
