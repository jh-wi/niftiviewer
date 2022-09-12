import SimpleITK as sitk
import matplotlib.pyplot as plt
import numpy as np
from simple_3dviz import Mesh
from simple_3dviz.window import show
from simple_3dviz.utils import render
from simple_3dviz.behaviours.movements import CameraTrajectory
from simple_3dviz.behaviours.trajectory import Circle
#from ctypes import cdll

def showNii(img):
	for i in range(img.shape[0]):
		print(i)
		plt.imshow(img[i, :, :], cmap='gray')
		plt.show()

itk_img = sitk.ReadImage('../data/4D.nii')
img = sitk.GetArrayFromImage(itk_img)
print(img.shape)
print(img[0].shape)
f = open("../data/densities.txt", "w")
xlength = len(img[0])
ylength = len(img[0][0])
zlength = len(img[0][0][0])
f.write(str(xlength) + "\n")
f.write(str(ylength) + "\n")
f.write(str(zlength) + "\n")
m = 0
for x in range(0, 128, 2):
	for y in range(0, 128, 2):
		for z in range(0, 128, 2):
			if img[0][x][y][z] > m:
				m = img[0][x][y][z]
			f.write(str(img[0][x][y][z]) + "\n")
f.close()
print("max density: " + str(m))

isolevel = 100
scan = img[0][::2,::2,::2]
print(scan.shape)


#plot voxels
x, y, z = np.indices((5, 5, 5))
voxels = scan > isolevel
colors = np.empty(voxels.shape + (3,), dtype=np.float32)
colors[voxels] = (1, 0, 0)

#ax = plt.figure().add_subplot(projection='3d')
#ax.voxels(voxels, facecolors=colors, edgecolor='k')
#plt.show()

#m = Mesh.from_voxel_grid(voxels=voxels, sizes=(0.49,0.49,0.49), colors=colors)

show(
	Mesh.from_voxel_grid(voxels=voxels, colors=colors),
	light=(-1, -1, 1),
	behaviours=[
		CameraTrajectory(
			Circle(center=(0,0,0), point=(2,-1,0), normal=(0,0,-1)), speed=0.003
		)
	]
)

#showNii(img)
#sitk.Show(img, 'ahh')
#print(img)