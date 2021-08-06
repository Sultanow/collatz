import matplotlib.pyplot as plt
import numpy as np
import math
from scipy.integrate import simpson


def get_row(number: int) -> int:
    return int(math.log(number, 2))


def get_col(row: int, number: int) -> int:
    return (number - (2**row + 1)) // 2


def get_row_count(row):
    return (2**(row+1)-2**row)//2


def rotate(vec, degrees):
    radians = math.radians(degrees)
    ca = math.cos(radians)
    sa = math.sin(radians)
    x = (vec[0] * ca) - (vec[1] * sa)
    y = (vec[0] * sa) + (vec[1] * ca)
    return (x, y)


def get_location(number):
    return rotate((0.0, 1.0), get_number_angle(number))

def get_number_angle(number):
    if number==1:
        return 0
    elif number==3:
        return 180
    elif number==5:
        return 270
    elif number==7:
        return 90
    else:
        row = get_row(number)
        col = get_col(row, number)
        return (360.0*float(col))/float(get_row_count(row))
      

def get_angle(vec):
    angle = math.degrees(math.atan2(vec[1], -vec[0]))-90
    while angle < 0.0:
        angle+=360.0
    return angle

def calculate_ratio(number, height):
    top=0
    bottom=0

    i=3
    while i<=number**height:
        angle=get_number_angle(i*number)
        # if angle>135.0 and angle<=180.0 :
        #     bottom+=1
        # elif angle>180.0 and angle<225.0:
        #     top+=1

        if angle<90.0:
            bottom+=1
        else:
            top+=1
        i*=number
    
    return (top, bottom)

fig, ax = plt.subplots(2, 2)

tops=[]
bottoms=[]
ratios=[]

for i in range(200):
    top, bottom=calculate_ratio(17, i)
    tops.append(top)
    bottoms.append(bottom)
    if bottom>0:
        ratios.append(top/bottom)

t = np.squeeze(tops)
b = np.squeeze(bottoms)
r = np.squeeze(ratios)

ax[0][0].plot(t, label = "Top")
ax[0][0].plot(b, label = "Bottom")
# ax[0][0].plot(t, label = "A")
# ax[0][0].plot(b, label = "B")
ax[0][0].legend()

ax[0][1].plot(r, label = "Ratio")
ax[0][1].legend()

################

positions_x=[]
positions_y=[]
positions_y_2=[]
height=20
for i in range(1, 2**height, 2):
    if get_row(i)<8:
        continue
    x1, y1=get_location(i)
    x2, y2=get_location(i*3)

    angle=get_number_angle(i)
    positions_x.append(angle)
    positions_y.append(x2)
    positions_y_2.append(y2)

ax[1][0].plot(positions_x, positions_y, label = "X positions")
ax[1][0].legend()
ax[1][1].plot(positions_x, positions_y_2, label = "Y positions")
ax[1][1].legend()

plt.show()