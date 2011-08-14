from mouse import mouseKeys

mouse_helper = {}

def key_press(key, x):
	if key.lower().startswith("mouse_"):
		key = key.split("_")[1]
		if not mouse_helper.has_key(key):
			mouse_helper[key] = mouseKeys()
		else:
			 x.SendKeyRelease( key )
		mouse[key].StartMouseMove(key)
	else:
		x.SendKeyPress( key )

def key_release(key, x):
	if key.lower().startswith("mouse_"):
		key = key.split("_")[1]
		if mouse_helper.has_key(key):
			mouse_helper[key].StopMouseMove()
	else:
		x.SendKeyRelease( key )

class JoyStick():
	def __init__(self):
		self.axis = { }
		self.buttons = { }

	def print_info(self):
		print "Joystick mappings found : "
		for axis in self.axis.keys():
			print " - axis %s movement mapped to function %s" % (axis, self.axis[axis].__name__)

		for key in self.buttons:
			print " - button %s mapped to key %s" % (key, self.buttons[key])

