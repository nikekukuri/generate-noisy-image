import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread("output.png", cv2.IMREAD_UNCHANGED)

for i in range(0, 10):
    print(img[i, i])


hist = cv2.calcHist([img], channels=[0], mask=None, histSize=[1023], ranges=[0, 1023])

# plt.hist(img.ravel(), bins=1023, rwidth=1.0, range=(0, 1023))
plt.plot(hist)
plt.show()

cv2.imshow('', img)

cv2.waitKey(0)
cv2.destroyAllWindows()
