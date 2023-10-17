import cv2 as cv
import numpy as np

def read_image():
    img = cv.imread('image.jpg')
    cv.imshow('Genshin Impact', img)
    cv.waitKey(0)
    cv.destroyAllWindows()

def colour_space():
    img = cv.imread('image.jpg')
    # cvtColor()函数用于转换图像的颜色空间
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)
    cv.imshow('Genshin Impact', gray)
    cv.imshow('Genshin Impact', hsv)
    cv.waitKey(0)
    cv.destroyAllWindows()

def img_Create():
    image = cv.imread('image.jpg')
    #np.zeros_like 用于创建与给定数组形状和类型相同，但其内容全为0的数组。
    blank = np.zeros_like(image)
    # 也可以直接赋值,但是这种拷贝，其实就像指针指向，改变blank的值，image也会改变
    # blank = image
    # 这种拷贝，就是真正的拷贝，改变blank的值，image不会改变
    # blank = np.copy(image)
    #把image的10:100,10:100的区域复制到blank，先是行，再是列，最后是通道
    blank[10:100,10:100,:]=image[10:100,10:100,:]
    cv.imshow('Blank', blank)
    cv.waitKey(0)
    cv.destroyAllWindows()

def pixel_demo():
    image = cv.imread('image.jpg')
    # hwc分别是高（行），宽（列），通道数
    h,w,c = image.shape
    for row in range(h):
        for col in range(w):
            b,g,r = image[row,col]
            #在数字图像处理中，当我们谈到8位的图像时，像素值的范围是从0到255。其中，0通常代表黑色，255代表白色（对于灰度图像）。
            #对于彩色图像中的每个通道（例如RGB），这个范围也适用，其中0代表通道的最低强度，255代表最高强度。
            #当我们说“取反”，我们意味着我们要转换像素值，使亮的像素变暗，暗的像素变亮。一种简单的方法是从255中减去当前像素值。
            image[row,col] = (255-b,255-g,255-r)
    cv.imshow('Pixel Demo', image)
    cv.imwrite('Pixel Demo.jpg', image)
    cv.waitKey(0)
    cv.destroyAllWindows()

# 下面的代码只有在直接运行这个文件时才会被执行，如果是被其他文件import的话，这段代码不会被执行
# __name__ 是一个内置的全局变量，它表示当前模块的名字
if __name__ == '__main__':
    colour_space()