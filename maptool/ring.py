import InputManager
from pico2d import *
from object import *
from share import *

image = None
error_image = None
is_able_to_create = True
width, height, depth = 80, 80, 5    # opengl에서의 x, y, z 크기
wz = 150    # opengl에서 링의 y값


def init():
    global image, error_image
    image = load_image('ring.png')
    error_image = load_image('ring_error.png')
    print("ring Y: %d" % wz)


class Ring(Object):
    # x: opengl의 x, y: opengl의 z, z: opengl의 y
    def __init__(self, x, y, z, img):
        super().__init__(x, y, width, depth, img)
        self.z = z
        self.d = height
        self.type = Object.RING


def update(objectList):
    global is_able_to_create, wz

    if InputManager.GetKeyState(SDLK_UP):
        wz += 1
        print("ring Y: %d" % wz)
    if InputManager.GetKeyState(SDLK_DOWN):
        wz -= 1
        print("ring Y: %d" % wz)

    mul = Camera.GetMul(WIDTH, HEIGHT)
    wx, wy = Camera.GetWorldPos(InputManager.mouseX / mul, InputManager.mouseY / mul)

    # 다른 건물과 겹치거나 맵을 벗어날 때 금지
    if wx - width / 2 < 0 or wx + width / 2 > mapW or wy - height / 2 < 0 or wy + height / 2 > mapH:
        is_able_to_create = False
    else:
        isCollision = False
        for o in objectList:
            if o.x + o.w / 2 < wx - width / 2 or o.x - o.w / 2 > wx + width / 2:
                continue
            if o.y + o.h / 2 < wy - depth / 2 or o.y - o.h / 2 > wy + depth / 2:
                continue
            if o.type == Object.BUILDING and o.d < wz - height / 2:
                continue
            elif o.type == Object.RING:
                if o.z + o.d / 2 < wz - height / 2 or o.z - o.d / 2 > wz + height / 2:
                    continue
            isCollision = True
            break

        is_able_to_create = not isCollision


def draw():
    mul = Camera.GetMul(WIDTH, HEIGHT)
    cx, cy = InputManager.mouseX, InputManager.mouseY
    if is_able_to_create:
        image.draw(cx, cy, width * mul, depth * mul)
    else:
        error_image.draw(cx, cy, width * mul, depth * mul)


def add(objectList):
    if is_able_to_create:
        mul = Camera.GetMul(WIDTH, HEIGHT)
        wx, wy = Camera.GetWorldPos(InputManager.mouseX / mul, InputManager.mouseY / mul)
        objectList.append(Ring(wx, wy, wz, image))
