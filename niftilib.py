import ctypes
#from cytpes import cdll
import sys
import os
import SimpleITK as sitk
import numpy as np
import math

handle = ctypes.cdll.LoadLibrary(os.getcwd() + "/nifti viewer/x64/Debug/nifti viewer.dll")
handle._SetChunkParams.argtypes = [ctypes.c_int, ctypes.c_int]
handle._ReceiveChunk.argtypes = [np.ctypeslib.ndpointer(dtype=np.int32), ctypes.c_int, ctypes.c_int, ctypes.c_int]

def log2(x):
	return math.log10(x) / math.log10(2)

def PowerOf2(x):
	return math.ceil(math.log2(x)) == math.floor(math.log2(x))

def NextPowerOf2(n):
    if n == 0:
        return 1
    if n & (n - 1) == 0:
        return n
    while n & (n - 1) > 0:
        n &= (n - 1)
    return n << 1

def ShowNii(path):
	itk_img = sitk.ReadImage(path)
	img = sitk.GetArrayFromImage(itk_img)
	
	xsize = len(img[0])
	ysize = len(img[0][0])
	zsize = len(img[0][0][0])
	
	print("Volume size: ", xsize, ", ", ysize, ", ", zsize);
	
	maxsize = max(xsize, ysize, zsize)
	if not (PowerOf2(maxsize)):
		maxsize = NextPowerOf2(maxsize)
	
	d = maxsize
	chunkSize = maxsize
	imgcut = np.zeros((d,d,d))
	
	print("Reading .nii file...")
	for x in range(d):
		for y in range(d):
			for z in range(d):
				if (x >= xsize or y >= ysize or z >= zsize):
					imgcut[x][y][z] = 0
				else:
					imgcut[x][y][z] = img[0][x][y][z]
	
	handle._SetChunkParams(chunkSize, d);
	
	for x in range(int(d/chunkSize)):
		for y in range(int(d/chunkSize)):
			for z in range(int(d/chunkSize)):
				data = np.zeros(chunkSize*chunkSize*chunkSize).astype(np.int32)
				for a in range(chunkSize):
					for b in range(chunkSize):
						for c in range(chunkSize):
							data[a*chunkSize*chunkSize+b*chunkSize+c] = imgcut[a+x*chunkSize][b+y*chunkSize][c+z*chunkSize]
				handle._ReceiveChunk(data, chunkSize*chunkSize*chunkSize, x, y, z)
	handle._window()
	