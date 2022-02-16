import numpy as np
import glfw
from OpenGL.GL import *

# Store which keybord button was pressed
button = 3

def render():
	
	glClear(GL_COLOR_BUFFER_BIT)
	glLoadIdentity()

	# Draw clock
	glBegin(GL_LINE_LOOP)
	v = np.linspace(0, 2*np.pi, 13)
	for i in range(12):
		glVertex2f(np.cos(v[i]), np.sin(v[i]))
	glEnd()

	# Draw hour hand
	glBegin(GL_LINES)
	glVertex2f(0,0)
	glVertex2f(np.cos(v[button]), np.sin(v[button]))
	glEnd()


def key_callback(window, key, scancode, action, mods):
	global button

	if key==glfw.KEY_1:
		button = 2
	elif key==glfw.KEY_2:
		button = 1
	elif key==glfw.KEY_3:
		button = 12
	elif key==glfw.KEY_4:
		button = 11
	elif key==glfw.KEY_5:
		button = 10
	elif key==glfw.KEY_6:
		button = 9
	elif key==glfw.KEY_7:
		button = 8
	elif key==glfw.KEY_8:
		button = 7
	elif key==glfw.KEY_9:
		button = 6
	elif key==glfw.KEY_0:
		button = 5
	elif key==glfw.KEY_Q:
		button = 4
	elif key==glfw.KEY_W:
		button = 3


def main():

	# Initialize the library
	if not glfw.init():
		return

	# Create a windowed mode window and its OpenGL context
	window = glfw.create_window(480, 480, "2018007974", None, None)
	if not window:
		glfw.terminate()
		return

	glfw.set_key_callback(window, key_callback)

	# Make the window's context current
	glfw.make_context_current(window)
	
	# Loop until the user closes the window
	while not glfw.window_should_close(window):
		# Poll for and process events
		glfw.poll_events()
		# Render here
		render()
		# Swap front and back buffers
		glfw.swap_buffers(window)

	glfw.terminate()

if __name__ == "__main__":
	main()
