keyMap = {}
lButton = False
rButton = False
mouseX, mouseY = None, None


def KeyDown(key):
    keyMap[key] = True


def KeyUp(key):
    keyMap[key] = False


def GetKeyState(key):
    if key in keyMap.keys() and keyMap[key] == True:
        return True
    return False


def GetLMouseState():
    pass


def GetRMouseState():
    pass


def GetMousePos():
    pass
