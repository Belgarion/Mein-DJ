import os
from config_helper import *

joysticks = {}

def handleMasterVolume(ev):
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

def handleLineInVolume(ev):
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


js0 = JoyStick()
js0.axis['9'] = handleMasterVolume
js0.axis['10'] = handleLineInVolume

joysticks[0] = js0



"""
Examples:

js0 = JoyStick()

js0.buttons['0'] = "Space"
js0.buttons['1'] = "Return"
js0.buttons['2'] = "x"
js0.buttons['3'] = "Mouse_click1"

js.axis['0'] = handleAxis0Function


"""
