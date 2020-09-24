from PIL import Image
from PIL import ImageMath
import binascii;
import time;
import random;
import sys;
import math
import json;

def vector_norm(a, b):
    res = 0
    res += (a[0]-b[0])**2;
    res += (a[1]-b[1])**2;
    res += (a[2]-b[2])**2;
    return math.sqrt(res)
im = Image.open("white.png")

pix_val = im.load()

dimensions = im.size
print (pix_val[0, 0])
for i in range(dimensions[0]):
    for j in range(dimensions[1]):
        o = pix_val[i, j]
        norm = vector_norm(o, (254, 170, 170, -1))
        if o == (255, 255, 255, 255):
            pix_val[i, j] = (o[0], o[1], o[2], 0)
        elif norm < 79:
            pix_val[i, j] = (254, 100, 101, 255)

im.save("transparent8.png")
        