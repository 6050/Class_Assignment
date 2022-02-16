import glfw
from OpenGL.GL import *
import numpy as np

trans = []

def render():
	glClear(GL_COLOR_BUFFER_BIT)
	glLoadIdentity() 

	# draw cooridnates
	glBegin(GL_LINES)
	glColor3ub(255,0,0)
	glVertex2fv(np.array([0.,0.]))
	glVertex2fv(np.array([1.,0.]))
	glColor3ub(0,255,0)
	glVertex2fv(np.array([0.,0.]))
	glVertex2fv(np.array([0.,1.]))
	glEnd()
	
	glColor3ub(255,255,255)
	
	for i in reversed(trans):
		if i == "Q":
			glTranslate(-0.1, 0, 0)
		elif i == "E":
			glTranslate(0.1, 0, 0)
		elif i == "A":
			glRotate(10, 0, 0, 1)
		elif i == "D":
			glRotate(-10, 0, 0, 1)
		elif i == "1":
			glLoadIdentity()
	
	drawTriangle()
	
def drawTriangle():
	glBegin(GL_TRIANGLES)
	glVertex2fv(np.array([0.,.5]))
	glVertex2fv(np.array([0.,0.]))
	glVertex2fv(np.array([.5,0.]))
	glEnd()

def key_callback(window, key, scancode, action, mods):

	global trans

	if key==glfw.KEY_Q:
		if action==glfw.PRESS or action==glfw.REPEAT:

			trans.append("Q")
			

	elif key==glfw.KEY_E:
		if action==glfw.PRESS or action==glfw.REPEAT:
	
			trans.append("E")
			
	elif key==glfw.KEY_A:
		if action==glfw.PRESS or action==glfw.REPEAT:

			trans.append("A")


	elif key==glfw.KEY_D:
		if action==glfw.PRESS or action==glfw.REPEAT:
	
			trans.append("D")


	elif key==glfw.KEY_1:
		if action==glfw.PRESS or action==glfw.REPEAT:
			
			trans = []


def main():
	
	if not glfw.init():
		return
	
	window = glfw.create_window(480, 480, "2018007974", None, None)
	if not window:
		glfw.terminate()
		return

	glfw.set_key_callback(window, key_callback)

	glfw.make_context_current(window)

	while not glfw.window_should_close(window):
		glfw.poll_events()

		render()
		
		glfw.swap_buffers(window)

	glfw.terminate()

if __name__ == "__main__":
	main()
