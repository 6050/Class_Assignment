import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np
from OpenGL.arrays import vbo
import ctypes

camAng = 0.
camHeight = 1.

colorR = 0
colorG = 0
colorB = 0
first = 1

def createArrays():
    
    varr = np.array([                                                           
    ( -1 ,  1 ,  1 ), # v0                                              
    (  1 ,  1 ,  1 ), # v1                                              
    (  1 , -1 ,  1 ), # v2                                              
    ( -1 , -1 ,  1 ), # v3                                              
    ( -1 ,  1 , -1 ), # v4                                              
    (  1 ,  1 , -1 ), # v5                                              
    (  1 , -1 , -1 ), # v6                                              
    ( -1 , -1 , -1 ), # v7                                              
    ], 'float32')                                                       

    iarr = np.array([                                                           
    (0,2,1),                                                            
    (0,3,2),                                                            
    (4,5,6),                                                            
    (4,6,7),                                                            
    (0,1,5),                                                            
    (0,5,4),                                                            
    (3,6,2),                                                            
    (3,7,6),                                                            
    (1,2,6),                                                            
    (1,6,5),                                                            
    (0,7,3),                                                            
    (0,4,7),                                                            
    ])

    narr = np.array([                                                           
    ( -0.5773502691896258 , 0.5773502691896258 , 0.5773502691896258 ),      
    ( 0.8164965809277261 , 0.4082482904638631 , 0.4082482904638631 ),       
    ( 0.4082482904638631 , -0.4082482904638631 , 0.8164965809277261 ),      
    ( -0.4082482904638631 , -0.8164965809277261 , 0.4082482904638631 ),     
    ( -0.4082482904638631 , 0.4082482904638631 , -0.8164965809277261 ),     
    ( 0.4082482904638631 , 0.8164965809277261 , -0.4082482904638631 ),      
    ( 0.5773502691896258 , -0.5773502691896258 , -0.5773502691896258 ),     
    ( -0.8164965809277261 , -0.4082482904638631 , -0.4082482904638631 )     
    ])

    return varr, iarr, narr

def drawCube_glDrawElements():
    global vertexArrayIndexed, indexArray, normalArray
    varr = vertexArrayIndexed
    iarr = indexArray
    narr = normalArray
    glEnableClientState(GL_VERTEX_ARRAY)
    glEnableClientState(GL_NORMAL_ARRAY)
    glVertexPointer(3, GL_FLOAT, 3*varr.itemsize, varr)
    glNormalPointer(GL_FLOAT, 6*varr.itemsize, varr)
    glDrawElements(GL_TRIANGLES, iarr.size, GL_UNSIGNED_INT, iarr)

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
    
    specularObjectColor = (colorR, colorG, colorB, 1.)
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularObjectColor)
    
    glMaterialfv(GL_FRONT, GL_SHININESS, 10)
    
    if not first:
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, specularObjectColor)

    glPushMatrix()
    drawCube_glDrawElements()
    glPopMatrix()

    glDisable(GL_LIGHTING)

def key_callback(window, key, scancode, action, mods):
    
    global camAng, camHeight, colorR, colorG, colorB

    if action == glfw.PRESS or action == glfw.REPEAT:
        if key == glfw.KEY_1:
            camAng += np.radians(-10)
        elif key == glfw.KEY_3:
            camAng += np.radians(10)
        elif key == glfw.KEY_2:
            camHeight += .1
        elif key == glfw.KEY_W:
            camHeight += -.1
        elif key == glfw.KEY_R:
            colorR = not colorR
            first = 0
        elif key == glfw.KEY_G:
            colorG = not colorG
            first = 0
        elif key == glfw.KEY_B:
            colorB = not colorB
            first = 0

vertexArrayIndexed = None
indexArray = None
normalArray = None

def main():

    global vertexArrayIndexed, indexArray, normalArray
    vertexArrayIndexed, indexArray, normalArray = createArrays()

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

