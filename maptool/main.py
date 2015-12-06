from pico2d import *
import Camera
import InputManager

WIDTH, HEIGHT = 800, 800
NODE_NUM = 32
mapW, mapH = 12000, 12000
isRunning = True
isRClick, isMClick = False, False
ry = 0
mx, my = 0, 0
wSize, hSize = 80, 80
wMin, wMax, hMin, hMax = 50, 80, 50, 80
objectList = []
nodeList = None
building = None
map = None


class Object:
    def __init__(self, x, y, w, h, img):
        self.image = img
        self.x, self.y, self.w, self.h = x, y, w, h

    def draw(self):
        mul = Camera.GetMul(WIDTH, HEIGHT)
        cx, cy = Camera.GetCameraPos(self.x, self.y)
        self.image.draw(cx * mul, cy * mul, self.w * mul, self.h * mul)


class Building:
    def __init__(self, x, y, w, h):
        self.image = [load_image('building.png'), load_image('building_error.png')]
        self.x, self.y, self.w, self.h = x, y, w, h
        self.isAbleToCreate = True

    def draw(self):
        mul = Camera.GetMul(WIDTH, HEIGHT)
        cx, cy = Camera.GetCameraPos(self.x, self.y)
        if self.isAbleToCreate:
            self.image[0].draw(cx * mul, cy * mul, self.w * mul, self.h * mul)
        else:
            self.image[1].draw(cx * mul, cy * mul, self.w * mul, self.h * mul)


class Node:
    W, H = 30, 30

    def __init__(self, x, y):
        self.x, self.y = x, y
        self.link = 0

    def draw(self):
        mul = Camera.GetMul(WIDTH, HEIGHT)
        cx, cy = Camera.GetCameraPos(self.x, self.y)
        w, h = self.W / 2, self.H / 2
        draw_rectangle((cx - w) * mul, (cy - h) * mul, (cx + w) * mul, (cy + h) * mul)


def init():
    global building, nodeList, map
    print('width : %d, height : %d' % (wSize, hSize))
    InputManager.mouseX, InputManager.mouseY = WIDTH / 2, HEIGHT / 2
    building = Building(900, 900, wSize, hSize)
    map = Object(mapW / 2, mapH / 2, mapW, mapH, load_image('area.png'))
    Camera.x, Camera.y = map.w / 2, map.h / 2
    Camera.w, Camera.h = WIDTH * 2, HEIGHT * 2

    w, h = mapW / (NODE_NUM - 1), mapH / (NODE_NUM - 1)
    if nodeList is None:
        nodeList = []
    for i in range(NODE_NUM):
        l = []
        for j in range(NODE_NUM):
            n = Node(j * w, i * h)
            l.append(n)
        nodeList.append(l)


def draw():
    global objectList, nodeList
    global map

    map.draw()

    for o in objectList:
        o.draw()
    for l in nodeList:
        for n in l:
            n.draw()

    mul = Camera.GetMul(WIDTH, HEIGHT)
    dir = ((0, 1), (1, -1), (1, 0), (1, 1))
    for i in range(NODE_NUM):
        for j in range(NODE_NUM):
            n = nodeList[i][j]
            nx, ny = Camera.GetCameraPos(n.x, n.y)
            for k in range(4):
                if n.link & 1 << k != 0:
                    v = dir[k]
                    t = nodeList[i + v[0]][j + v[1]]
                    tx, ty = Camera.GetCameraPos(t.x, t.y)
                    draw_line(nx * mul, ny * mul, tx * mul, ty * mul)

    building.draw()


def update():
    global objectList, map, wSize, hSize, wMin, wMax, hMin, hMax

    if InputManager.GetKeyState(SDLK_a):
        Camera.x -= 10
    if InputManager.GetKeyState(SDLK_d):
        Camera.x += 10
    if InputManager.GetKeyState(SDLK_w):
        Camera.y += 10
    if InputManager.GetKeyState(SDLK_s):
        Camera.y -= 10

    isSizeChange = False
    if InputManager.GetKeyState(SDLK_UP) and hSize < hMax:
        hSize += 1
        isSizeChange = True
    if InputManager.GetKeyState(SDLK_DOWN) and hSize > hMin:
        hSize -= 1
        isSizeChange = True
    if InputManager.GetKeyState(SDLK_LEFT) and wSize > wMin:
        wSize -= 1
        isSizeChange = True
    if InputManager.GetKeyState(SDLK_RIGHT) and wSize < wMax:
        wSize += 1
        isSizeChange = True
    if isSizeChange:
        building.w, building.h = wSize, hSize
        print('width : %d, height : %d' % (wSize, hSize))

    mul = Camera.GetMul(WIDTH, HEIGHT)
    wx, wy = Camera.GetWorldPos(InputManager.mouseX / mul, InputManager.mouseY / mul)
    building.x, building.y = wx, wy

    # 다른 건물과 겹치거나 맵을 벗어날 때 금지
    if wx - building.w / 2 < 0 or wx + building.w / 2 > map.w or wy - building.h / 2 < 0 or wy + building.h / 2 > map.h:
        building.isAbleToCreate = False
    else:
        isCollision = False
        for o in objectList:
            if o.x + o.w / 2 < wx - building.w / 2 or o.x - o.w / 2 > wx + building.w / 2:
                continue
            if o.y + o.h / 2 < wy - building.h / 2 or o.y - o.h / 2 > wy + building.h / 2:
                continue
            isCollision = True
            break

        building.isAbleToCreate = not isCollision


def save(fname='map_data'):
    global objectList, mapW, mapH, nodeList
    saveData = {'data': [], 'mw': mapW, 'mh': mapH}

    for o in objectList:
        saveData['data'].append({'x': o.x, 'y': o.y, 'w': o.w, 'h': o.h})

    with open(fname + '.json', 'w') as f:
        json.dump(saveData, f, sort_keys=True, indent=4, separators=(',', ': '))

    with open(fname + '_node.txt', 'w') as f:
        f.write(str(NODE_NUM) + '\n')
        for i in range(NODE_NUM):
            i = NODE_NUM - i - 1
            line = ''
            for j in range(NODE_NUM):
                link_data = 0
                n = nodeList[i][j]
                if n.link & 1:
                    link_data |= 1
                if n.link & 2:
                    link_data |= 1 << 5
                if n.link & 4:
                    link_data |= 1 << 6
                if n.link & 8:
                    link_data |= 1 << 7
                if i > 0 and j + 1 < NODE_NUM and nodeList[i - 1][j + 1].link & 2:
                    link_data |= 1 << 1
                if i > 0 and nodeList[i - 1][j].link & 4:
                    link_data |= 1 << 2
                if i > 0 and j > 0 and nodeList[i-1][j-1].link & 8:
                    link_data |= 1 << 3
                if j > 0 and nodeList[i][j-1].link & 1:
                    link_data |= 1 << 4
                line += str(link_data) + ' '
            f.write(line+'\n')


def load(fname='map_data.json'):
    global objectList, imageBuilding, mapW, mapH, map

    with open(fname, 'r') as f:
        loadData = json.load(f)

        mapW = loadData['mw']
        mapH = loadData['mh']
        map.w = mapW
        map.h = mapH
        map.x = mapW / 2
        map.y = mapH / 2

        objectList.clear()
        for d in loadData['data']:
            objectList.append(Object(d['x'], d['y'], d['w'], d['h'], building.image[0]))


def dot(v1, v2):
    return v1[0] * v2[0] + v1[1] * v2[1]


def cross(v1, v2):
    return v1[0] * v2[1] - v1[1] * v2[0]


def point_distance(x1, y1, x2, y2):
    return math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)


def line_point_length(lx1, ly1, lx2, ly2, px, py):
    v1, v2, v3 = (px - lx1, py - ly1), (px - lx2, py - ly2), (lx2 - lx1, ly2 - ly1)
    if dot(v1, v3) * dot(v2, v3) <= 0:
        return math.fabs(cross(v1, v3)) / point_distance(lx1, ly1, lx2, ly2)
    else:
        return min(point_distance(lx1, ly1, px, py), point_distance(lx2, ly2, px, py))


def link_node():
    global nodeList, objectList

    for i in range(NODE_NUM):
        for j in range(NODE_NUM):
            n = nodeList[i][j]
            if j + 1 < NODE_NUM:
                n.link |= 1
            if i + 1 < NODE_NUM:
                n.link |= 4
                if j - 1 >= 0:
                    n.link |= 2
                if j + 1 < NODE_NUM:
                    n.link |= 8

    bw, bh = mapW / (NODE_NUM - 1), mapH / (NODE_NUM - 1)
    for obj in objectList:
        ox, oy = int(obj.x / bw), int(obj.y / bh)
        for i in range(-1, 2):
            if oy+i < 0 or oy+i >= NODE_NUM: continue
            for j in range(-1, 3):
                if ox+j < 0 or ox+j >= NODE_NUM: continue
                if ox + j + 1 < NODE_NUM:
                    n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i][ox + j + 1]
                    if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
                        n1.link &= ~1
                if oy + i + 1 < NODE_NUM:
                    n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i + 1][ox + j]
                    if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
                        n1.link &= ~4

                    if ox + j > 0:
                        n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i + 1][ox + j - 1]
                        if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
                            n1.link &= ~2
                    if ox + j + 1 < NODE_NUM:
                        n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i + 1][ox + j + 1]
                        if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
                            n1.link &= ~8

    return
    # for i in range(NODE_NUM):
    #     for j in range(NODE_NUM):
    #         if j + 1 < NODE_NUM:
    #             n1, n2 = nodeList[i][j], nodeList[i][j + 1]
    #             if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
    #                 n1.link &= ~1
    #         if i + 1 < NODE_NUM:
    #             n1, n2 = nodeList[i][j], nodeList[i + 1][j]
    #             if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
    #                 n1.link &= ~4
    #
    #             if j > 0:
    #                 n1, n2 = nodeList[i][j], nodeList[i + 1][j - 1]
    #                 if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
    #                     n1.link &= ~2
    #             if j < NODE_NUM - 1:
    #                 n1, n2 = nodeList[i][j], nodeList[i + 1][j + 1]
    #                 if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 100:
    #                     n1.link &= ~8


def add_building():
    global objectList, nodeList
    mul = Camera.GetMul(WIDTH, HEIGHT)
    wx, wy = Camera.GetWorldPos(InputManager.mouseX / mul, InputManager.mouseY / mul)
    objectList.append(Object(wx, wy, wSize, hSize, building.image[0]))


def handle_events():
    global objectList, isRunning, isRClick, isMClick, ry, mx, my, building, map, wSize, hSize
    events = get_events()
    for event in events:
        if event.type == SDL_QUIT:
            isRunning = False
        elif event.type == SDL_KEYDOWN:
            if event.key == SDLK_ESCAPE:
                isRunning = False
            elif event.key == SDLK_k:
                fname = input('Save File Name: ')
                save(fname)
            elif event.key == SDLK_l:
                fname = input('Load File Name: ')
                load(fname + '.json')
            elif event.key == SDLK_n:
                link_node()
            elif event.key == SDLK_z:
                if len(objectList) != 0:
                    objectList.pop()
            elif event.key == SDLK_c:
                objectList.clear()
            else:
                InputManager.KeyDown(event.key)
        elif event.type == SDL_KEYUP:
            InputManager.KeyUp(event.key)
        elif event.type == SDL_MOUSEBUTTONDOWN:
            if event.button == SDL_BUTTON_RIGHT:
                isRClick = True
                ry = HEIGHT - event.y - 1
            elif event.button == SDL_BUTTON_MIDDLE:
                isMClick = True
                mx, my = event.x, HEIGHT - event.y - 1
            elif building.isAbleToCreate:
                add_building()
        elif event.type == SDL_MOUSEBUTTONUP:
            if event.button == SDL_BUTTON_RIGHT:
                isRClick = False
            elif event.button == SDL_BUTTON_MIDDLE:
                isMClick = False
        elif event.type == SDL_MOUSEMOTION:
            if isRClick:
                # 확대
                if ry - (HEIGHT - event.y - 1) < 0 and (Camera.w >= WIDTH + 80 and Camera.h >= HEIGHT + 80):
                    Camera.w -= 80
                    Camera.h -= 80
                # 축소
                elif ry - (HEIGHT - event.y - 1) > 0:
                    Camera.w += 80
                    Camera.h += 80
                ry = HEIGHT - event.y - 1
            elif isMClick:
                mul = (WIDTH / Camera.w + HEIGHT / Camera.h) / 2
                Camera.x -= (event.x - mx) / mul
                Camera.y -= ((HEIGHT - event.y - 1) - my) / mul
                mx, my = event.x, HEIGHT - event.y - 1
            InputManager.mouseX, InputManager.mouseY = event.x, HEIGHT - event.y - 1


if __name__ == "__main__":
    open_canvas(WIDTH, HEIGHT)
    init()

    while isRunning:
        clear_canvas()
        draw()
        update_canvas()
        handle_events()
        update()

    close_canvas()
