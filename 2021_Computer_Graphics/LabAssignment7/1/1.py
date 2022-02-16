import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np
from OpenGL.arrays import vbo
import ctypes

camAng = 0.
camHeight = 1.

aLightColor = 'f'
sObjectColor = 'f'

def drawcube_glvertex():

    glBegin(GL_TRIANGLES)

    glNormal3f(0,0,1) # v0, v2, v1, v0, v3, v2 normal
    glVertex3f( -1 ,  1 ,  1 ) # v0 position
    glVertex3f(  1 , -1 ,  1 ) # v2 position
    glVertex3f(  1 ,  1 ,  1 ) # v1 position

    glVertex3f( -1 ,  1 ,  1 ) # v0 position
    glVertex3f( -1 , -1 ,  1 ) # v3 position
    glVertex3f(  1 , -1 ,  1 ) # v2 position

    glNormal3f(0,0,-1)
    glVertex3f( -1 ,  1 , -1 ) # v4
    glVertex3f(  1 ,  1 , -1 ) # v5
    glVertex3f(  1 , -1 , -1 ) # v6

    glVertex3f( -1 ,  1 , -1 ) # v4
    glVertex3f(  1 , -1 , -1 ) # v6
    glVertex3f( -1 , -1 , -1 ) # v7

    glNormal3f(0,1,0)
    glVertex3f( -1 ,  1 ,  1 ) # v0
    glVertex3f(  1 ,  1 ,  1 ) # v1
    glVertex3f(  1 ,  1 , -1 ) # v5

    glVertex3f( -1 ,  1 ,  1 ) # v0
    glVertex3f(  1 ,  1 , -1 ) # v5
    glVertex3f( -1 ,  1 , -1 ) # v4

    glNormal3f(0,-1,0)
    glVertex3f( -1 , -1 ,  1 ) # v3
    glVertex3f(  1 , -1 , -1 ) # v6
    glVertex3f(  1 , -1 ,  1 ) # v2

    glVertex3f( -1 , -1 ,  1 ) # v3
    glVertex3f( -1 , -1 , -1 ) # v7
    glVertex3f(  1 , -1 , -1 ) # v6

    glNormal3f(1,0,0)
    glVertex3f(  1 ,  1 ,  1 ) # v1
    glVertex3f(  1 , -1 ,  1 ) # v2
    glVertex3f(  1 , -1 , -1 ) # v6

    glVertex3f(  1 ,  1 ,  1 ) # v1
    glVertex3f(  1 , -1 , -1 ) # v6
    glVertex3f(  1 ,  1 , -1 ) # v5

    glNormal3f(-1,0,0)
    glVertex3f( -1 ,  1 ,  1 ) # v0
    glVertex3f( -1 , -1 , -1 ) # v7
    glVertex3f( -1 , -1 ,  1 ) # v3

    glVertex3f( -1 ,  1 ,  1 ) # v0
    glVertex3f( -1 ,  1 , -1 ) # v4
    glVertex3f( -1 , -1 , -1 ) # v7
    glEnd()

def render():

    global camAng, camHeight, aLightColor, sObjectColor

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, 1, 1,10)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(5*np.sin(camAng),camHeight,5*np.cos(camAng), 0,0,0, 0,1,0)

    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)

    glEnable(GL_NORMALIZE)
    
    glPushMatrix()

    lightPos = (3., 4., 5., 1.)
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos)

    glPopMatrix()

    ambientLightColor = (.1, .1, .1, 1.)
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor)

    specularObjectColor = (1., 1., 1., 1.)
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularObjectColor)
    
    glMaterialfv(GL_FRONT, GL_SHININESS, 10)


    if aLightColor == 'r':
        ambientLightColor = (1., .0, .0, 1.)
    elif aLightColor == 'g':
        ambientLightColor = (0., 1., 0., 1.)
    elif aLightColor == 'b':
        ambientLightColor = (0., 0., 1., 1.)
    elif aLightColor == 'w':
        ambientLightColor = (1., 1., 1., 1.)

    if sObjectColor == 'r':
        specularObjectColor = (1., 0., 0., 1.)
    elif sObjectColor == 'g':
        specularObjectColor = (0., 1., 0., 1.)
    elif sObjectColor == 'b':
        specularObjectColor = (0., 0., 1., 1.)
    elif sObjectColor == 'w':
        specularObjectColor = (1., 1., 1., 1.)

    if aLightColor != 'f':
        glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLightColor)
        glLightfv(GL_LIGHT0, GL_SPECULAR, ambientLightColor)
    if sObjectColor != 'f':
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, specularObjectColor)
        
    glPushMatrix()
    drawcube_glvertex()
    glPopMatrix()

    glDisable(GL_LIGHTING)

def key_callback(window, key, scancode, action, mods):
    
    global camAng, camHeight, aLightColor, sObjectColor

    if action == glfw.PRESS or action == glfw.REPEAT:
        if key == glfw.KEY_1:
            camAng += np.radians(-10)
        elif key == glfw.KEY_3:
            camAng += np.radians(10)
        elif key == glfw.KEY_2:
            camHeight += .1
        elif key == glfw.KEY_W:
            camHeight += -.1
        elif key == glfw.KEY_A:
            aLightColor = 'r'
        elif key == glfw.KEY_S:
            aLightColor = 'g'
        elif key == glfw.KEY_D:
            aLightColor = 'b'
        elif key == glfw.KEY_F:
            aLightColor = 'w'
        elif key == glfw.KEY_Z:
            sObjectColor = 'r'
        elif key == glfw.KEY_X:
            sObjectColor = 'g'
        elif key == glfw.KEY_C:
            sObjectColor = 'b'
        elif key == glfw.KEY_V:
            sObjectColor = 'w'

def main():

    if not glfw.init():
        return
    
    window = glfw.create_window(480, 480, '2018007974', None,None)
    if not window:
        glfw.terminate()
        return
    
    glfw.make_context_current(window)
    glfw.set_key_callback(window, key_callback)
    glfw.swap_interval(1)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        render()
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()

