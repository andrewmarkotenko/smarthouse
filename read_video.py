#!/usr/bin/python3

import cv2

camera = cv2.VideoCapture(0)
# camera = cv2.VideoCapture("rtsps://192.168.1.1:7441/fOA1IXk3su0Oyu12?enableSrtp", cv2.CAP_FFMPEG)
# camera = cv2.VideoCapture("rtsps://192.168.1.1:7441/fOA1IXk3su0Oyu12?enableSrtp", cv2.CAP_DSHOW)
#video  = cv2.VideoWriter('video.avi', -1, 25, (640, 360));
#video = cv2.VideoWriter("output.avi", cv2.VideoWriter_fourcc(*'MPEG'), 25, (640, 480))
video = cv2.VideoWriter("output.avi", cv2.VideoWriter_fourcc('M','J','P','G'), 20, (640, 360))
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
while True:
   f,img = camera.read()
   hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
   gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
   faces = face_cascade.detectMultiScale(gray, 1.3, 5)
   for (x, y, w, h) in faces:
      # To draw a rectangle in a face
      cv2.rectangle(img, (x, y), (x + w, y + h), (255, 255, 0), 2)
      roi_gray = gray[y:y + h, x:x + w]
      roi_color = img[y:y + h, x:x + w]

      # Detects eyes of different sizes in the input image
      eyes = eye_cascade.detectMultiScale(roi_gray)

      # To draw a rectangle in eyes
      for (ex, ey, ew, eh) in eyes:
         cv2.rectangle(roi_color, (ex, ey), (ex + ew, ey + eh), (0, 127, 255), 2)

   cv2.rectangle(img, (100, 100), (400, 400), (0, 255, 0), 1)
   video.write(img)
   cv2.imshow("webcam",img)
   # cv2.imshow('frame', hsv)
   # cv2.imshow('blackandblue', gray)
   if (cv2.waitKey(5) != -1):
       break

video.release()
cv2.destroyAllWindows()
