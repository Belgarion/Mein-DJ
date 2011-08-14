import os

from xml.dom.minidom import *

from threads import threaded
from xkeys import XKeys
from mouse import mouseKeys

class JoyStickAxis():
    def __init__(self, lowKey, highKey):
        self.low = lowKey
        self.high = highKey
	self.low_active = False	
	self.high_active = False	



class JoyStick():
    def __init__(self):
        self.axis = { }
        self.buttons = { }
        
    def print_info(self):
        print "Joystick mappings found : "
        for axis in self.axis.keys():
            print " - axis %s direction low mapped to key %s" % (axis, self.axis[axis].low)
            print " - axis %s direction high mapped to key %s" % (axis, self.axis[axis].high)
            
        for key in self.buttons:
            print " - button %s mapped to key %s" % (key, self.buttons[key])
    
class JKeys:
    def __init__(self, document):
    
        self.joysticks = {}

        for joy_node in document.getElementsByTagName("joystick"):
            joy = JoyStick()

            for axis_node in joy_node.getElementsByTagName("axis"):
                axis = JoyStickAxis( axis_node.getAttribute("low"), axis_node.getAttribute("high"))
                joy.axis[ axis_node.getAttribute("number") ] = axis
            
            for button_node in joy_node.getElementsByTagName("button"):
                joy.buttons[button_node.getAttribute("number")] = button_node.getAttribute("key")
            
            self.joysticks[int(joy_node.getAttribute("id"))] = joy
            
       

            joy.print_info()
 
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
            
        elif self.nbJoy != len(self.joysticks):
            print "%d joysticks configured, only %d joysticks found" % (len(self.joysticks), self.nbJoy)
        
        self.debug = False

    @threaded
    def run(self):
    
        if self.nbJoy != 0:
            import pygame
            pygame.display.init()
            pygame.time.set_timer(1, 100)
            capture_events = [pygame.JOYBUTTONDOWN, pygame.JOYAXISMOTION, pygame.JOYBUTTONUP, pygame.JOYHATMOTION]
            x = XKeys()
            mouse = {}
            
            # clear out all pre existing pygame events else we get 1 of each
            pygame.event.clear()
            
            def key_press( key ):
                if key.lower().startswith("mouse_"):
                    key = key.split("_")[1]
                    if not mouse.has_key(key):
                        mouse[key] = mouseKeys()
                    else:
                         x.SendKeyRelease( key )
                    mouse[key].StartMouseMove(key)
                else:
                    x.SendKeyPress( key )
                 
            def key_release( key ):
                if key.lower().startswith("mouse_"):
                    key = key.split("_")[1]
                    if mouse.has_key(key):
                        mouse[key].StopMouseMove()
                else:
                    x.SendKeyRelease( key )
            
            while True:
                pygame.event.pump()
                ev = pygame.event.wait()
                
                if ev.type in capture_events:

                    joy = self.joysticks[ ev.joy ]
                    
                    if ev.type == pygame.JOYHATMOTION:
                        res = gethatcode(ev)
                        if self.debug: print "UNHANDLED JOY HAT code: ", res
                        
                    elif ev.type == pygame.JOYBUTTONDOWN:
                        button = str(ev.button)
                        if joy.buttons.has_key(button):
                            key_code = joy.buttons[button]
                            if self.debug: print "Button down : " + key_code
                            key_press( key_code )
                    elif ev.type == pygame.JOYBUTTONUP:
                        button = str(ev.button)
                        if joy.buttons.has_key(button):
                            key_code = joy.buttons[button]
                            if self.debug: print "Button up : " + key_code
                            key_release( key_code )
                        
                      
                    elif ev.type == pygame.JOYAXISMOTION:
                        axis_number = str(ev.axis)
			os.system("echo HEKLLO WORLDS " + axis_number)
			if axis_number == "9": # Master volume
				if ev.value+1 < 0.05:
					os.system("pactl -s 127.0.0.1 set-sink-volume alsa_output.usb-Hercules_Hercules_DJ_Console-04-WE.analog-surround-41 0x00000")
				else:
					VOLMAX = 63192
					volbase = 30000
					vol = ((ev.value+1) * 50*0.01)*(VOLMAX-volbase)
					voltot = str(hex(int(volbase+vol)))
					volstr = "0x0"+voltot[2:]
					print volstr
					os.system("pactl -s 127.0.0.1 set-sink-volume alsa_output.usb-Hercules_Hercules_DJ_Console-04-WE.analog-surround-41 "+volstr)
			elif axis_number == '10':
				if ev.value+1 < 0.05:
					os.system("pactl -s 127.0.0.1 set-source-volume alsa_input.usb-Hercules_Hercules_DJ_Console-04-WE.analog-stereo 0x00000")
				else:
					VOLMAXLINEIN = 102400
					volbase = 30000
					vol = ((ev.value+1) * 50*0.01)*(VOLMAXLINEIN-volbase)
					voltot = str(hex(int(volbase+vol)))
					volstr = "0x0"+voltot[2:]
					print volstr
					os.system("pactl -s 127.0.0.1 set-source-volume alsa_input.usb-Hercules_Hercules_DJ_Console-04-WE.analog-stereo "+volstr)
			print "END YUGGE"
			continue
                        if joy.axis.has_key(axis_number):
                        
                            axis = joy.axis[axis_number]
    
                            if ev.value < 0:
                                if self.debug: print "Button down : " + axis.low
                                key_press( axis.low )
                                axis.low_active = True
                            
                            elif ev.value > 0:
                                if self.debug: print "Button down : " + axis.high
                                key_press( axis.high )
                                axis.high_active = True
                            
                            else:
                                if axis.low_active:
                                    if self.debug: print "Button up : " + axis.low
                                    key_release( axis.low )
                                    axis.low_active = False
                                if axis.high_active:
                                    if self.debug: print "Button up : " + axis.high
                                    key_release( axis.high )
                                    axis.high_active = False
                                

            pygame.display.quit()
            return res

    def __del__(self):
        import pygame
        pygame.quit()
 
