#!/usr/bin/python3

import cv2

#camera = cv2.VideoCapture(0)
camera = cv2.VideoCapture("rtsps://192.168.1.1:7441/fOA1IXk3su0Oyu12?enableSrtp", cv2.CAP_FFMPEG)
#camera = cv2.VideoCapture("rtsps://192.168.1.1:7441/fOA1IXk3su0Oyu12?enableSrtp", cv2.CAP_DSHOW)
#video  = cv2.VideoWriter('video.avi', -1, 25, (640, 360));
#video = cv2.VideoWriter("output.avi", cv2.VideoWriter_fourcc(*'MPEG'), 25, (640, 480))
video = cv2.VideoWriter("output.avi", cv2.VideoWriter_fourcc('M','J','P','G'), 20, (640, 360))
while True:
   f,img = camera.read()
   cv2.rectangle(img, (100, 100), (400, 400), (0, 255, 0), 1)
   video.write(img)
   cv2.imshow("webcam",img)
   if (cv2.waitKey(5) != -1):
       break
video.release()
