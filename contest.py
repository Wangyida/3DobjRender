import os
root = '/Volumes/wangyida/train'
files = os.listdir(root)
log1 = '_'
log2 = '.'
for file in files:
	if os.path.isdir(root+'/'+file) and file[0]!='.':
		objlist = os.listdir(root+'/'+file)
		for object in objlist:
			if object[0]!='.':
				name_obj = root+'/'+file+'/'+object
				os.system('./contestShapenet -objmodel=%s -label_class=%s -label_item=%s -bakgrdir=%s -cam_rot=1' % (name_obj, file, object[object.find(log1) + 1:object.find(log2)], '/Users/yidawang/Documents/database/backgrd_black/'))

