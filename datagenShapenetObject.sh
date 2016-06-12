rootmodel="/home/yida/Downloads/shapenet/ShapeNetCore.v1/collection/"
rootback_o="/home/yida/Documents/database/backgrd_black/"
targetimage_o="../data/images_sp_object/"
rootback_p="/home/yida/Documents/database/backgrd_flickr/"
targetimage_p="../data/images_sp_photo/"
for i in $(seq 50)
do
	./ReadOBJ -objmodel=${rootmodel}aeroplane/${i}/model.obj -mtlmodel=${rootmodel}aeroplane/${i}/model.mtl -texmodel=${rootmodel}aeroplane/${i} -label_class=1 -label_item=$i -bakgrdir_p=${rootback_p}aeroplane_sky/ -semisphere=0 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}bicycle/${i}/model.obj -mtlmodel=${rootmodel}bicycle/${i}/model.mtl -texmodel=${rootmodel}bicycle/${i} -label_class=2 -label_item=$i -bakgrdir_p=${rootback_p}bicycle/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}boat/${i}/model.obj -mtlmodel=${rootmodel}boat/${i}/model.mtl -texmodel=${rootmodel}boat/${i} -label_class=3 -label_item=$i -bakgrdir_p=${rootback_p}boat/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}bottle/${i}/model.obj -mtlmodel=${rootmodel}bottle/${i}/model.mtl -texmodel=${rootmodel}bottle/${i} -label_class=4 -label_item=$i -bakgrdir_p=${rootback_p}bottle/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}bus/${i}/model.obj -mtlmodel=${rootmodel}bus/${i}/model.mtl -texmodel=${rootmodel}bus/${i} -label_class=5 -label_item=$i -bakgrdir_p=${rootback_p}bus/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}car/${i}/model.obj -mtlmodel=${rootmodel}car/${i}/model.mtl -texmodel=${rootmodel}car/${i} -label_class=6 -label_item=$i -bakgrdir_p=${rootback_p}car/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}chair/${i}/model.obj -mtlmodel=${rootmodel}chair/${i}/model.mtl -texmodel=${rootmodel}chair/${i} -label_class=7 -label_item=$i -bakgrdir_p=${rootback_p}chair/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}diningtable/${i}/model.obj -mtlmodel=${rootmodel}diningtable/${i}/model.mtl -texmodel=${rootmodel}diningtable/${i} -label_class=8 -label_item=$i -bakgrdir_p=${rootback_p}diningtable/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}motorbike/${i}/model.obj -mtlmodel=${rootmodel}motorbike/${i}/model.mtl -texmodel=${rootmodel}motorbike/${i} -label_class=9 -label_item=$i -bakgrdir_p=${rootback_p}motorbike/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}sofa/${i}/model.obj -mtlmodel=${rootmodel}sofa/${i}/model.mtl -texmodel=${rootmodel}sofa/${i} -label_class=10 -label_item=$i -bakgrdir_p=${rootback_p}sofa/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}train/${i}/model.obj -mtlmodel=${rootmodel}train/${i}/model.mtl -texmodel=${rootmodel}train/${i} -label_class=11 -label_item=$i -bakgrdir_p=${rootback_p}train/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
	./ReadOBJ -objmodel=${rootmodel}monitor/${i}/model.obj -mtlmodel=${rootmodel}monitor/${i}/model.mtl -texmodel=${rootmodel}monitor/${i} -label_class=12 -label_item=$i -bakgrdir_p=${rootback_p}monitor/ -z_range=0.8 -imagedir_o=${targetimage_o} -frontalLight=0
done
