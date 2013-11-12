import cv
import time

cv.StartWindowThread()
capture = cv.CaptureFromCAM(0)

imUndistort = cv.CreateImage((640, 480), cv.IPL_DEPTH_8U, 3)

cv.NamedWindow('Original')
cv.NamedWindow('Unidistorted')

objectPoints = cv.CreateMat(1, 3, cv.CV_64F)
imagePoints = cv.CreateMat(1, 2, cv.CV_64F)
rvec = cv.CreateMat(1, 3, cv.CV_64F)
tvec = cv.CreateMat(1, 3, cv.CV_64F)
cv.Zero(rvec)
cv.Zero(tvec)

key = 0
while (key != 27):
    im = cv.QueryFrame(capture)
    cv.Undistort2(im, imUndistort, cameraMatrix, distCoeffs)
    if key == 32:
        objectPoints[0,0] = input('X = ')
        objectPoints[0,1] = input('Y = ')
        objectPoints[0,2] = input('Z = ')
        cv.ProjectPoints2(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints)      
        print '%f, %f' %(imagePoints[0,0], imagePoints[0,1])    
    center = (cv.Round(imagePoints[0,0]), cv.Round(imagePoints[0, 1]))
    cv.Circle(imUndistort, center, 1, cv.Scalar(0, 0, 255), 2)
    cv.ShowImage('Original', im)
    cv.ShowImage('Unidistorted', imUndistort)
    key = cv.WaitKey(10)%256
    
cv.DestroyWindow('Original')
cv.DestroyWindow('Unidistorted')
del(capture)