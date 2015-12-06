x, y = None, None
w, h = None, None

def GetCameraPos(wx, wy):
    global x, y, w, h
    return int(wx - x + w / 2), int(wy - y + h / 2)


def SetCameraPos(wx, wy):
    global x, y, w, h
    x, y = wx, wy


def SetCameraSize(cw, ch):
    global w, h
    w, h = cw, ch


def GetWorldPos(cx, cy):
    global x, y, w, h
    return int(cx + x - w/2), int(cy + y - h/2)


def GetMul(wWidth, wHeight):
    return (wWidth/w + wHeight/h)/2
