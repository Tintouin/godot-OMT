extends Node3D

func _ready():
	var finder = OMTFinder.new()
	add_child(finder)
	finder.sources_changed.connect(_on_sources_changed.bind(finder))

func _on_sources_changed(finder):
	print("sources_changed: ", finder.get_sources())
