import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

def createArrays():
    
    varr = np.array([
        ( 0, 0, 0 ), # v0
        ( 0, 0, 1.5 ), # v1
        ( 0, 1.5, 1.5 ), # v2
        ( 0, 1.5, 0 ), # v3
        ( 1.5, 1.5, 1.5 ), # v4
        ( 1.5, 1.5, 0 ), # v5
        ( 1.5, 0, 1.5 ), # v6
        ( 1.5, 0, 0 ), # v7
        ], 'float32')
    
    iarr = np.array([
        ( 2, 3, 5, 4 ),
        ( 0, 1, 2, 3 ),
        ( 0, 3, 5, 7 ),
        ( 1, 6, 4, 2 ),
        ( 4, 6, 7, 5 ),
        ( 0, 7, 6, 1  )
        ])
    
    return varr, iarr

def render():

    global camAng, camHeight

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)

    glLoadIdentity()
    gluPerspective(45, 1, 1, 10)
    gluLookAt(5*np.sin(camAng),camHeight,5*np.cos(camAng), 0,0,0, 0,1,0)

    drawFrame()
    glColor3ub(255, 255, 255)
    drawCube_glDrawElements()

def drawFrame():
    
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([1.,0.,0.]))
    glColor3ub(0, 255, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([0.,1.,0.]))
    glColor3ub(0, 0, 255)
    glVertex3fv(np.array([0.,0.,0]))
    glVertex3fv(np.array([0.,0.,1.]))
    glEnd()

def key_callback(window, key, scancode, action, mods):
    
    global camAng, camHeight
    
    if action==glfw.PRESS or action==glfw.REPEAT:
        if key==glfw.KEY_1:
            camAng += np.radians(-10)
        elif key==glfw.KEY_3:
            camAng += np.radians(10)
        elif key==glfw.KEY_2:
            camHeight += .1
        elif key==glfw.KEY_W:
            camHeight += -.1

def drawCube_glDrawElements():

    global vertexArray, indexArray
    varr = vertexArray
    iarr = indexArray

    glEnableClientState(GL_VERTEX_ARRAY)
    glVertexPointer(3, GL_FLOAT, 3*varr.itemsize, varr)
    glDrawElements(GL_QUADS, iarr.size, GL_UNSIGNED_INT, iarr)

camAng = 0
camHeight = 1
vertexArray = None
indexArray = None

def main(): 
   
    global vertexArray, indexArray
    
    if not glfw.init():
        return

    window = glfw.create_window(480, 480, "2018007974", None, None)    
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_key_callback(window, key_callback)

    vertexArray, indexArray = createArrays()

    while not glfw.window_should_close(window):
        glfw.poll_events()

        render()

        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()
