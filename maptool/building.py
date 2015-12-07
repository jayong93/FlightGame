import InputManager
import random
from object import *
from pico2d import *
from share import *

image = None
error_image = None
is_able_to_create = True
width, height = 80, 80  #opengl에서 x, z 크기


def init():
    global image, error_image
    image = load_image('building.png')
    error_image = load_image('building_error.png')


class Building(Object):
    def __init__(self, x, y, img):
        super().__init__(x, y, width, height, img)
        self.d = random.randint(100, 350)
        self.type = Object.BUILDING


def update(objectList):
    global is_able_to_create
    mul = Camera.GetMul(WIDTH, HEIGHT)
    wx, wy = Camera.GetWorldPos(InputManager.mouseX / mul, InputManager.mouseY / mul)

    # 다른 건물과 겹치거나 맵을 벗어날 때 금지
    if wx - width / 2 < 0 or wx + width / 2 > mapW or wy - height / 2 < 0 or wy + height / 2 > mapH:
        is_able_to_create = False
    else:
        isCollision = False
        for o in objectList:
            if o.type != Object.BUILDING: continue
            if o.x + o.w / 2 < wx - width / 2 or o.x - o.w / 2 > wx + width / 2:
                continue
            if o.y + o.h / 2 < wy - height / 2 or o.y - o.h / 2 > wy + height / 2:
                continue
            isCollision = True
            break

        is_able_to_create = not isCollision


def draw():
    mul = Camera.GetMul(WIDTH, HEIGHT)
    cx, cy = InputManager.mouseX, InputManager.mouseY
    if is_able_to_create:
        image.draw(cx, cy, width * mul, height * mul)
    else:
        error_image.draw(cx, cy, width * mul, height * mul)


def add(objectList):
    if is_able_to_create:
        mul = Camera.GetMul(WIDTH, HEIGHT)
        wx, wy = Camera.GetWorldPos(InputManager.mouseX / mul, InputManager.mouseY / mul)
        objectList.append(Building(wx, wy, image))
