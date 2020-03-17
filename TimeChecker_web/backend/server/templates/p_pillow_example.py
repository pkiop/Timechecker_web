#p_pillow_example

from PIL import Image

im = Image.new("RGB", (500, 500), (100, 200, 220))
im2 = Image.new("RGB", (200, 200))
im.save("a.jpg")
im2.save("b.jpg")

im.paste(im2, (20, 20, 220, 220))
im.save("c.jpg")