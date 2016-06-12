
import os
import bpy

root = '/Volumes/wangyida/train'
files = os.listdir(root)
log = '_'
for file in files:
	if os.path.isdir(root+'/'+file) and file!='.DS_Store' and file!='..DS_Store':
		objlist = os.listdir(root+'/'+file)
		for object in objlist:
			name_obj = root+'/'+file+'/'+object
			override = {'selected_bases': list(bpy.context.scene.object_bases)}
			bpy.ops.object.delete(override)
			bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=object)
			bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=object)

root = '/Volumes/wangyida/train_perturbed'
files = os.listdir(root)
log = '_'
for file in files:
	if os.path.isdir(root+'/'+file) and file!='.DS_Store' and file!='..DS_Store':
		objlist = os.listdir(root+'/'+file)
		for object in objlist:
			name_obj = root+'/'+file+'/'+object
			override = {'selected_bases': list(bpy.context.scene.object_bases)}
			bpy.ops.object.delete(override)
			bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=object)
			bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=object)

root = '/Volumes/wangyida/val'
files = os.listdir(root)
log = '_'
for file in files:
	if os.path.isdir(root+'/'+file) and file!='.DS_Store' and file!='..DS_Store':
		objlist = os.listdir(root+'/'+file)
		for object in objlist:
			name_obj = root+'/'+file+'/'+object
			override = {'selected_bases': list(bpy.context.scene.object_bases)}
			bpy.ops.object.delete(override)
			bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=object)
			bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=object)


root = '/Volumes/wangyida/val_perturbed'
files = os.listdir(root)
log = '_'
for file in files:
	if os.path.isdir(root+'/'+file) and file!='.DS_Store' and file!='..DS_Store':
		objlist = os.listdir(root+'/'+file)
		for object in objlist:
			name_obj = root+'/'+file+'/'+object
			override = {'selected_bases': list(bpy.context.scene.object_bases)}
			bpy.ops.object.delete(override)
			bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=object)
			bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=object)


root = '/Volumes/wangyida/test_allinone'
files = os.listdir(root)
log = '_'
for file in files:
	if file!='.DS_Store' and file!='..DS_Store':
		name_obj = root+'/'+file
		override = {'selected_bases': list(bpy.context.scene.object_bases)}
		bpy.ops.object.delete(override)
		bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=file)
		bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=file)


root = '/Volumes/wangyida/test_allinone_perturbed'
files = os.listdir(root)
log = '_'
for file in files:
	if file!='.DS_Store' and file!='..DS_Store':
		name_obj = root+'/'+file
		override = {'selected_bases': list(bpy.context.scene.object_bases)}
		bpy.ops.object.delete(override)
		bpy.ops.import_scene.obj(filepath=name_obj,filter_glob=file)
		bpy.ops.export_scene.obj(filepath=name_obj,filter_glob=file)
