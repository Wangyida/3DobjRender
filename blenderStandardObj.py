import os
import bpy

root = '/Volumes/wangyida/collection'
log = '_'
for file in os.listdir(root):
	if os.path.isdir(root+'/'+file) and file[0]!='.':
		objlist = os.listdir(root+'/'+file)
		for object in objlist:
			if object[0]!='.':
				name_obj = root+'/'+file+'/'+object+'/model.obj'
				override = {'selected_bases': list(bpy.context.scene.object_bases)}
				bpy.ops.object.delete(override)
				bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=object)
				bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=object)
