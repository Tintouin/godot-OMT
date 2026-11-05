extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var f = OMTFinder.new()
	add_child(f)
	print(f.get_sources())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
