import cv

horiCorners = input('Enter number of horizontal chessboard corners: ')
vertCorners = input('Enter number of vertical chessboard corners: ')
dims = (horiCorners, vertCorners)
length = input('Enter square length (m): ')

numCalIm = input('Enter number of calibration images: ')
iNumCalIm = 0

objPoints = cv.CreateMat(horiCorners*vertCorners*numCalIm, 3, cv.CV_32FC1)
imPoints = cv.CreateMat(horiCorners*vertCorners*numCalIm, 2, cv.CV_32FC1)
numPoints = cv.CreateMat(numCalIm, 1, cv.CV_32SC1)

cv.StartWindowThread()


cv.NamedWindow('Calibration')
im = None;
imGray = cv.CreateImage((640, 480), cv.IPL_DEPTH_8U, 1)
found = 0;
points = None;
key = 0
imageNumber = 0
while (key != 27):
    im = cv.LoadImage('./calibrationImages/%03d.png' %(imageNumber))
    cv.CvtColor(im, imGray, cv.CV_RGB2GRAY)
    found, points = cv.FindChessboardCorners(imGray, dims, cv.CV_CALIB_CB_ADAPTIVE_THRESH)
    if found != 0:
        points = cv.FindCornerSubPix(imGray, points, (11, 11), (-1, -1), (cv.CV_TERMCRIT_EPS | cv.CV_TERMCRIT_ITER, 30, 0.1))        
        cv.DrawChessboardCorners(im,dims,points,found)        
        cv.ShowImage('Calibration', im)
        key = cv.WaitKey(0)%256
        if key == 10:
            #Save as a calibrated image        
            print '%d calibration image %03d.png saved.' %(iNumCalIm+1, imageNumber)
            numPoints[iNumCalIm, 0] = horiCorners*vertCorners
            for i in range(shape(points)[0]):
                index = i + (iNumCalIm*horiCorners*vertCorners)
                imPoints[index,0] = points[i][0]
                imPoints[index,1] = points[i][1]
                objPoints[index, 0] = length*(i%horiCorners)
                objPoints[index, 1] = length*(i/horiCorners)
                objPoints[index, 2] = 0
            iNumCalIm = iNumCalIm + 1
            if iNumCalIm == numCalIm:
                #Create first Intrinsic Camera Matrix and Distortion Matrix
                cameraMatrix = cv.CreateMat(3, 3, cv.CV_64FC1)
                distCoeffs = cv.CreateMat(4, 1, cv.CV_64FC1)
                
                cv.SetZero(cameraMatrix)
                cv.SetZero(distCoeffs)
                cameraMatrix[0,0] = 1.0
                cameraMatrix[1,1] = 1.0
                
                R=cv.CreateMat(1*numCalIm, 3, cv.CV_64F)
                T=cv.CreateMat(1*numCalIm, 3, cv.CV_64F)
                size=cv.GetSize(im)
                
                cv.CalibrateCamera2(objPoints, imPoints, numPoints, size, cameraMatrix, distCoeffs, R, T)
                
                print '\ncameraMatrix:'
                for i in range(3):
                    print '%f, %f, %f' %(cameraMatrix[i,0], cameraMatrix[i,1], cameraMatrix[i,2])
                
                print '\ndistCoeffs:'
                for i in range(4):
                    print '%f' %(distCoeffs[i,0])     
                cv.Save("intrinsics.xml", cameraMatrix)
                cv.Save("distortion.xml", distCoeffs)
                break
        else:
            print '%03d.png Not used for calibration.' %(imageNumber)
    else:
        print 'No Checkerboard found on image %03d.png' %(imageNumber)
        cv.ShowImage('Calibration', im)    
        key = cv.WaitKey(0)%256
    imageNumber = imageNumber + 1

cv.DestroyWindow('Calibration')