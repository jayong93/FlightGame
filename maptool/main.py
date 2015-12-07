from pico2d import *
import InputManager
import building
import ring
from object import *
from share import *

NODE_NUM = 32
isRunning = True
isRClick, isMClick = False, False
ry = 0
mx, my = 0, 0
objectList = []
nodeList = None
map_data = None
state = None


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
    global nodeList, map_data, state
    InputManager.mouseX, InputManager.mouseY = WIDTH / 2, HEIGHT / 2
    map_data = Object(mapW / 2, mapH / 2, mapW, mapH, load_image('area.png'))
    map_data.type = Object.MAP_OBJ
    Camera.x, Camera.y = map_data.w / 2, map_data.h / 2
    Camera.w, Camera.h = WIDTH * 2, HEIGHT * 2

    building.init()
    ring.init()
    state = building
    print('Building Mode')

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
    global map_data

    map_data.draw()

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

    state.draw()


def update():
    global state

    if InputManager.GetKeyState(SDLK_a):
        Camera.x -= 10
    if InputManager.GetKeyState(SDLK_d):
        Camera.x += 10
    if InputManager.GetKeyState(SDLK_w):
        Camera.y += 10
    if InputManager.GetKeyState(SDLK_s):
        Camera.y -= 10

    state.update(objectList)


def save(fname='map_data'):
    global objectList, mapW, mapH, nodeList
    saveData = {'data': [], 'mw': mapW, 'mh': mapH}

    for o in objectList:
        if o.type == Object.BUILDING:
            saveData['data'].append({'type': 'building', 'x': o.x, 'y': o.y, 'w': o.w, 'd': o.d, 'h': o.h})
        elif o.type == Object.RING:
            saveData['data'].append({'type': 'ring', 'x': o.x, 'y': o.y, 'z': o.z,'w': o.w, 'd': o.d, 'h': o.h})

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
                if i > 0 and j > 0 and nodeList[i - 1][j - 1].link & 8:
                    link_data |= 1 << 3
                if j > 0 and nodeList[i][j - 1].link & 1:
                    link_data |= 1 << 4
                line += str(link_data) + ' '
            f.write(line + '\n')


def load(fname='map_data.json'):
    global objectList, imageBuilding, mapW, mapH, map_data

    with open(fname, 'r') as f:
        loadData = json.load(f)

        mapW = loadData['mw']
        mapH = loadData['mh']
        map_data.w = mapW
        map_data.h = mapH
        map_data.x = mapW / 2
        map_data.y = mapH / 2

        objectList.clear()
        for d in loadData['data']:
            if d['type'] == 'building':
                objectList.append(building.Building(d['x'], d['y'], building.image))
            elif d['type'] == 'ring':
                objectList.append(ring.Ring(d['x'], d['y'], d['z'], ring.image))


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
            if oy + i < 0 or oy + i >= NODE_NUM: continue
            for j in range(-1, 3):
                if ox + j < 0 or ox + j >= NODE_NUM: continue
                if ox + j + 1 < NODE_NUM:
                    n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i][ox + j + 1]
                    if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 70:
                        n1.link &= ~1
                if oy + i + 1 < NODE_NUM:
                    n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i + 1][ox + j]
                    if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 70:
                        n1.link &= ~4

                    if ox + j > 0:
                        n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i + 1][ox + j - 1]
                        if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 70:
                            n1.link &= ~2
                    if ox + j + 1 < NODE_NUM:
                        n1, n2 = nodeList[oy + i][ox + j], nodeList[oy + i + 1][ox + j + 1]
                        if line_point_length(n1.x, n1.y, n2.x, n2.y, obj.x, obj.y) <= 70:
                            n1.link &= ~8

    return


def handle_events():
    global objectList, isRunning, isRClick, isMClick, ry, mx, my, map_data, wSize, hSize, state
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
            elif event.key == SDLK_1:
                state = building
                print('Building Mode')
            elif event.key == SDLK_2:
                state = ring
                print('Ring Mode')
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
            else:
                state.add(objectList)
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
