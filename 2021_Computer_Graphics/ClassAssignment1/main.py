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
        ( 0, 7, 6, 1 )
        ])
    
    return varr, iarr

def render():

    global perspective, azim, elev, dist, rightClick, deltaX, deltaZ

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)

    glLoadIdentity()

    if(perspective):
        gluPerspective(45, 1, 1, 50)
    else:
        glOrtho(-0.5*dist,0.5*dist, -0.5*dist,0.5*dist, -25,25)

    gluLookAt(dist*np.sin(np.radians(90-elev))*np.cos(np.radians(azim)),
              dist*np.cos(np.radians(90-elev)),
              dist*np.sin(np.radians(90-elev))*np.sin(np.radians(azim)),
              0,0,0, 0,1,0)
    
    glTranslate(deltaX, 0, deltaZ)

    drawGrid()
    drawFrame()
    glColor3ub(255, 255, 50)
    drawCube_glDrawElements()

def drawFrame():
    
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([30.,0.,0.]))
    glColor3ub(0, 255, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([0.,30.,0.]))
    glColor3ub(0, 0, 255)
    glVertex3fv(np.array([0.,0.,0]))
    glVertex3fv(np.array([0.,0.,30.]))
    glEnd()

def drawGrid():

    glBegin(GL_LINES)
    glColor3ub(150, 150, 150)

    A = np.arange(0, 30, 0.5)

    for i in A:
        glVertex3fv(np.array([i, 0., 30.]))
        glVertex3fv(np.array([i, 0., -30.]))

        glVertex3fv(np.array([-i, 0., 30.]))
        glVertex3fv(np.array([-i, 0., -30.]))
        
        glVertex3fv(np.array([30., 0., i]))
        glVertex3fv(np.array([-30., 0., i]))

        glVertex3fv(np.array([30., 0., -i]))
        glVertex3fv(np.array([-30., 0., -i]))
    
    glEnd()

def key_callback(window, key, scancode, action, mods):
    
    global azim, elev, perspective
    
    if action==glfw.PRESS or action==glfw.REPEAT:
        if key == glfw.KEY_V:
            perspective = not perspective
        elif key==glfw.KEY_A:
            azim += -1
        elif key==glfw.KEY_D:
            azim += 1
        elif key==glfw.KEY_W:
            elev += 1
        elif key==glfw.KEY_S:
            elev += -1

def cursor_callback(window, xpos, ypos):

    global leftClick, rightClick, firstClick, oldX, oldY, azim, elev, deltaX, deltaZ
    
    # orbit
    if leftClick and firstClick:

        oldX = xpos
        oldY = ypos
        firstClick = 0

    elif leftClick and not firstClick:
    
        xDiff = xpos-oldX
        yDiff = ypos-oldY

        oldX = xpos
        oldY = ypos

        azim +=  0.3 * xDiff
        elev += 0.3 * yDiff
        
        if elev <= -89:
            elev = -89
        elif elev >= 89:
            elev = 89

    # panning
    if rightClick and firstClick:

        oldX = xpos
        oldY = ypos

        firstClick = 0

    elif rightClick and not firstClick:

        xTemp = (xpos-oldX)
        zTemp = (ypos-oldY)

        deg = azim - 90
        M = np.array([[np.cos(np.radians(deg)), -np.sin(np.radians(deg))],
                      [np.sin(np.radians(deg)),  np.cos(np.radians(deg))]])
        xTemp, zTemp = np.dot(M, np.array([xTemp, zTemp]))
        deltaX += 0.01 * xTemp 
        deltaZ += 0.01 * zTemp
        
        oldX = xpos
        oldY = ypos 

def button_callback(window, button, action, mod):

    global leftClick, rightClick, firstClick

    if button == glfw.MOUSE_BUTTON_LEFT:
        if action == glfw.PRESS:
            leftClick = firstClick = 1
        elif action == glfw.RELEASE: 
            leftClick = 0

    if button == glfw.MOUSE_BUTTON_RIGHT:
        if action == glfw.PRESS:
            rightClick = firstClick = 1
        elif action == glfw.RELEASE: 
            rightClick = 0

def scroll_callback(window, xoffset, yoffset):

    global dist

    dist = dist - yoffset

    if dist < 2: dist = 2
    if dist > 20: dist = 20
    
def drawCube_glDrawElements():

    global vertexArray, indexArray
    varr = vertexArray
    iarr = indexArray

    glEnableClientState(GL_VERTEX_ARRAY)
    glVertexPointer(3, GL_FLOAT, 3*varr.itemsize, varr)
    glDrawElements(GL_QUADS, iarr.size, GL_UNSIGNED_INT, iarr)

perspective = 1

azim = 45
elev = 45
dist = 10

deltaX = 0
deltaZ = 0

leftClick = 0
rightClick = 0

vertexArray = None
indexArray = None

def main(): 
   
    global vertexArray, indexArray
    
    if not glfw.init():
        return

    window = glfw.create_window(700, 700, "Basic OpenGL viewer", None, None)    
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_key_callback(window, key_callback)
    glfw.set_cursor_pos_callback(window, cursor_callback)
    glfw.set_mouse_button_callback(window, button_callback)
    glfw.set_scroll_callback(window, scroll_callback)

    vertexArray, indexArray = createArrays()

    while not glfw.window_should_close(window):
        glfw.poll_events()

        render()

        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()
