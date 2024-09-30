extends Node

@export var main_scene: PackedScene
@export var main_buttons: Node
@export var settings_menu: Node

func _on_play_button_down():
	get_tree().change_scene_to_packed(main_scene)
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED

func _on_quit_button_down():
	get_tree().quit()

func _on_settings_button_down():
	settings_menu.visible = true
	main_buttons.visible = false

func _on_back_button_down():
	settings_menu.visible = false
	main_buttons.visible = true

func _on_window_mode_item_selected(index):
	var window: Window = get_window()
	
	match index:
		0:
			window.mode = Window.MODE_WINDOWED
			window.borderless = false
		1:
			window.mode = Window.MODE_EXCLUSIVE_FULLSCREEN
		2:
			window.borderless = true
			window.mode = Window.MODE_MAXIMIZED
		3:
			window.borderless = true
			window.mode = Window.MODE_WINDOWED
