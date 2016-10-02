# parameter setting
rootmodel="./collection/"
rootback_o="/Users/yidawang/Documents/database/backgrd_black/"
targetimage_o="../data/images_sp_object/"
rootback_p="/Users/yidawang/Documents/database/backgrd_flickr/"
targetimage_p="../data/images_sp_photo/"

# making directories
strings="aeroplane bicycle boat bottle bus car chair diningtable motorbike sofa train monitor"
cat_array=($strings)
if [ ! -d ../data/images_sp_object ]; then
	mkdir ../data/images_sp_object
fi
if [ ! -d ../data/images_sp_photo ]; then
	mkdir ../data/images_sp_photo
fi
for i in $(seq 12)
do
	if [ ! -d ../data/images_sp_object/${cat_array[i-1]} ]; then
		mkdir ../data/images_sp_object/${cat_array[i-1]}
	fi
	if [ ! -d ../data/images_sp_photo/${cat_array[i-1]} ]; then
		mkdir ../data/images_sp_photo/${cat_array[i-1]}
	fi
done

# rendering for real images
for i in $(seq 50)
do
	./semantic_render -objmodel=${rootmodel}${cat_array[0]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=1 -label_item=$i -semisphere=0 -imagedir_p=${targetimage_o}${cat_array[0]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[1]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=2 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[1]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[2]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=3 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[2]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[3]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=4 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[3]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[4]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=5 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[4]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[5]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=6 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[5]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[6]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=7 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[6]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[7]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=8 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[7]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[8]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=9 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[8]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[9]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=10 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[9]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[10]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=11 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[10]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[11]}/${i}/model.obj -bakgrdir_p=${rootback_o} -label_class=12 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[11]} -frontalLight=1
done
# rendering for the semantic images
for i in $(seq 50)
do
	./pose_render -objmodel=${rootmodel}${cat_array[0]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[0]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[0]}/${i} -label_class=1 -label_item=${i} -bakgrdir_p=${rootback_p}aeroplane_sky/ -semisphere=0 -imagedir_p=${targetimage_p}${cat_array[0]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[1]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[1]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[1]}/${i} -label_class=2 -label_item=${i} -bakgrdir_p=${rootback_p}bicycle/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[1]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[2]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[2]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[2]}/${i} -label_class=3 -label_item=${i} -bakgrdir_p=${rootback_p}boat/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[2]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[3]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[3]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[3]}/${i} -label_class=4 -label_item=${i} -bakgrdir_p=${rootback_p}bottle/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[3]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[4]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[4]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[4]}/${i} -label_class=5 -label_item=${i} -bakgrdir_p=${rootback_p}bus/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[4]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[5]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[5]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[5]}/${i} -label_class=6 -label_item=${i} -bakgrdir_p=${rootback_p}car/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[5]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[6]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[6]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[6]}/${i} -label_class=7 -label_item=${i} -bakgrdir_p=${rootback_p}chair/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[6]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[7]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[7]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[7]}/${i} -label_class=8 -label_item=${i} -bakgrdir_p=${rootback_p}diningtable/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[7]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[8]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[8]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[8]}/${i} -label_class=9 -label_item=${i} -bakgrdir_p=${rootback_p}motorbike/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[8]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[9]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[9]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[9]}/${i} -label_class=10 -label_item=${i} -bakgrdir_p=${rootback_p}sofa/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[9]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[10]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[10]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[10]}/${i} -label_class=11 -label_item=${i} -bakgrdir_p=${rootback_p}train/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[10]} -frontalLight=0
	./pose_render -objmodel=${rootmodel}${cat_array[11]}/${i}/model.obj -mtlmodel=${rootmodel}${cat_array[11]}/${i}/model.mtl -texmodel=${rootmodel}${cat_array[11]}/${i} -label_class=12 -label_item=${i} -bakgrdir_p=${rootback_p}monitor/ -z_range=0.8 -imagedir_p=${targetimage_p}${cat_array[11]} -frontalLight=0
done
