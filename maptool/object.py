import Camera


class Object:
    BUILDING, RING, MAP_OBJ = 0, 1, 2

    # x: opengl의 x, y: opengl의 z
    def __init__(self, x, y, w, h, img):
        self.image = img
        self.x, self.y, self.w, self.h = x, y, w, h
        self.type = None

    def draw(self):
        mul = Camera.GetMul(800, 800)
        cx, cy = Camera.GetCameraPos(self.x, self.y)
        self.image.draw(cx * mul, cy * mul, self.w * mul, self.h * mul)