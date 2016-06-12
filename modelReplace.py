import os
import shutil
rootpath = '/home/yida/Downloads/shapenet/ShapeNetCore.v1/collection/'
listpath = '../modelReplaceLists/'
for i in range(1, 13):
	fb = open('%s%s_0.txt' % (listpath, i))
	fw = open('%s%s_1.txt' % (listpath, i))
	modelname = fb.readline()
	modelname = modelname.strip()
	for linew in fw.readlines():
		linew = rootpath + modelname + '/' + linew.strip()
		lineb = fb.readline()
		lineb = rootpath + modelname + '/' + lineb.strip()
		print lineb, linew
		if os.path.exists(lineb):
			shutil.rmtree(lineb)
		shutil.copytree(linew, lineb)
	fb.close()
	fw.close()
