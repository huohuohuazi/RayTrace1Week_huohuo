from PIL import Image
 
def ppm2jpg(str):
    str_ppm=str+".ppm"
    img = Image.open(str)
    img.save(str+".jpg")
    img.show()