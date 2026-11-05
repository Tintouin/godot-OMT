extends Node3D

var finder = OMTFinder.new()

func _ready():
	add_child(finder)
	print("Finder started")
	await get_tree().create_timer(3.0).timeout
	finder.refresh()
	print("Sources : ", finder.get_sources())

func _process(delta: float) -> void:
	print("Sources : ", finder.get_sources())
