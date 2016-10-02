# parameter setting
rootmodel="/home/yida/Downloads/shapenet/ShapeNetCore.v1/collection/"
rootback_o="/home/yida/Documents/database/backgrd_black/"
targetimage_o="../data/images_sp_object/"
rootback_p="/home/yida/Documents/database/backgrd_flickr/"
targetimage_p="../data/images_sp_photo/"

# making directories
strings="aeroplane bottle chair sofa bicycle bus diningtable train boat car motorbike tvmonitor"
cat_array=($strings)
for i in $(seq 12)
do
	if [ ! -d ../data/images_sp_object/${cat_array[i-1]} ]; then
		mkdir ../data/images_sp_object/${cat_array[i-1]}
	fi
	if [ ! -d ../data/images_sp_photo/${cat_array[i-1]} ]; then
		mkdir ../data/images_sp_photo/${cat_array[i-1]}
	fi
done

# rendering
for i in $(seq 50)
do
	./semantic_render -objmodel=${rootmodel}aeroplane/${i}/model.obj -mtlmodel=${rootmodel}aeroplane/${i}/model.mtl -label_class=1 -label_item=$i -semisphere=0 -imagedir_p=${targetimage_o}aeroplane  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}bicycle/${i}/model.obj -mtlmodel=${rootmodel}bicycle/${i}/model.mtl -label_class=2 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}bicycle  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}boat/${i}/model.obj -mtlmodel=${rootmodel}boat/${i}/model.mtl -label_class=3 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}boat  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}bottle/${i}/model.obj -mtlmodel=${rootmodel}bottle/${i}/model.mtl -label_class=4 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}bottle  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}bus/${i}/model.obj -mtlmodel=${rootmodel}bus/${i}/model.mtl -label_class=5 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}bus -frontalLight=1
	./semantic_render -objmodel=${rootmodel}car/${i}/model.obj -mtlmodel=${rootmodel}car/${i}/model.mtl -label_class=6 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}car -frontalLight=1
	./semantic_render -objmodel=${rootmodel}chair/${i}/model.obj -mtlmodel=${rootmodel}chair/${i}/model.mtl -label_class=7 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}chair -frontalLight=1
	./semantic_render -objmodel=${rootmodel}diningtable/${i}/model.obj -mtlmodel=${rootmodel}diningtable/${i}/model.mtl -label_class=8 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}diningtable -frontalLight=1
	./semantic_render -objmodel=${rootmodel}motorbike/${i}/model.obj -mtlmodel=${rootmodel}motorbike/${i}/model.mtl -label_class=9 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}motorbike -frontalLight=1
	./semantic_render -objmodel=${rootmodel}sofa/${i}/model.obj -mtlmodel=${rootmodel}sofa/${i}/model.mtl -label_class=10 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}sofa -frontalLight=1
	./semantic_render -objmodel=${rootmodel}train/${i}/model.obj -mtlmodel=${rootmodel}train/${i}/model.mtl -label_class=11 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}train -frontalLight=1
	./semantic_render -objmodel=${rootmodel}monitor/${i}/model.obj -mtlmodel=${rootmodel}monitor/${i}/model.mtl -label_class=12 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}tvmonitor -frontalLight=1
done
