from PIL import Image
from PIL import ImageMath

watermark = Image.open("watermark_spaceship.jpg")
watermark=watermark.resize(dimensions)
watermark.save("enlarged_watermark_spaceship.jpg")