import os

from xml.dom.minidom import *

from threads import threaded
from xkeys import XKeys
from config_helper import *

class JKeys:
	def __init__(self, config):
		self.config = config

		try:
			import pygame
		except:
			print "You need to install pygame to capture joystick controls"
		pygame.init()


		self.nbJoy = pygame.joystick.get_count()

		for i in range(self.nbJoy):
			pygame.joystick.Joystick(i).init()

		if self.nbJoy == 0:
			print "Sorry no joysticks found!!"

		elif self.nbJoy != len(self.config.joysticks):
			print "%d joysticks configured, only %d joysticks found" % (len(self.config.joysticks), self.nbJoy)

		self.debug = False

	@threaded
	def run(self):
		if self.nbJoy != 0:
			import pygame
			pygame.display.init()
			pygame.time.set_timer(1, 100)
			capture_events = [pygame.JOYBUTTONDOWN, pygame.JOYAXISMOTION, pygame.JOYBUTTONUP, pygame.JOYHATMOTION]
			x = XKeys()

			# clear out all pre existing pygame events else we get 1 of each
			pygame.event.clear()

			# only allow wanted events on the queue
			pygame.event.set_allowed(capture_events)

			while True:
				pygame.event.pump()
				ev = pygame.event.wait()

				if ev.type in capture_events:

					joy = self.config.joysticks[ev.joy]

					if ev.type == pygame.JOYHATMOTION:
						res = gethatcode(ev)
						if self.debug: print "UNHANDLED JOY HAT code: ", res

					elif ev.type == pygame.JOYBUTTONDOWN:
						button = str(ev.button)
						if joy.buttons.has_key(button):
							key_code = joy.buttons[button]
							if self.debug: print "Button down : " + key_code
							key_press(key_code, x)
					elif ev.type == pygame.JOYBUTTONUP:
						button = str(ev.button)
						if joy.buttons.has_key(button):
							key_code = joy.buttons[button]
							if self.debug: print "Button up : " + key_code
							key_release(key_code, x)


					elif ev.type == pygame.JOYAXISMOTION:
						axis_number = str(ev.axis)
						if joy.axis.has_key(axis_number):
							if self.debug: print "Handle axis:", axis_number
							joy.axis[axis_number](ev)
						else:
							if self.debug: print "Unhandled axis:", axis_number

			pygame.display.quit()
			return res

	def __del__(self):
		import pygame
		pygame.quit()

