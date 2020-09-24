from PIL import Image
from PIL import ImageMath

im = Image.open("IMG_2007.jpg")
pix_val = im.load()
dimensions = im.size
'''for i in range(dimensions[0]):
    for j in range(dimensions[1]):
        tup = pix_val[i, j]
        r = tup[0]
        g = tup[1]
        b = tup[2]
        avg = int((r + b + g)/3)
        pix_val[i, j] = (avg, avg, avg)'''

watermark = Image.open("watermark_spaceship.jpg")
originalWatermarkDimensions = watermark.size;
watermark=watermark.resize(dimensions)
#print(str(watermark.size[0]) + " " + str(watermark.size[1]))
splittedOriginal = im.split()
alpha = 1;
red = 0;
green = 0;
blue = 0;
if(len(splittedOriginal) == 4):
    red, green, blue, alpha = splittedOriginal;

else:
    red, green, blue = splittedOriginal

splittedWatermark = watermark.split()
walpha = 1;
wred = 0;
wgreen = 0;
wblue = 0;
if(len(splittedWatermark) == 4):
    wred, wgreen, wblue, walpha = splittedWatermark

else:
    red, green, blue = splittedWatermark

wred, wgreen, wblue, walpha = watermark.split()

red2 = ImageMath.eval("convert(a&0xFE|b&0x1,'L')", a=red, b=wred)
green2 = ImageMath.eval("convert(a&0xFE|b&0x1,'L')", a=green, b=wgreen)
blue2 = ImageMath.eval("convert(a&0xFE|b&0x1,'L')", a=blue, b=wblue)

encoded = Image.merge("RGB", (red2, green2, blue2))
#encoded = encoded.resize(dimensions);
encoded.save("encoded.png")

stegged=Image.open("encoded.png")
red, green, blue = stegged.split()
extractedRed=ImageMath.eval("(a&0x1)*255",a=red) # convert to 0 or 255
#extractedGreen=ImageMath.eval("(a&0x1)*255",a=green)
#extractedBlue=ImageMath.eval("(a&0x1)*255", a=blue)
#extractedImage = Image.merge("RGB", (extractedRed, extractedGreen, extractedBlue))
extractedImage = extractedRed;
extractedImage=extractedImage.convert("L")
extractedImage.save("extracted-watermark.png")



