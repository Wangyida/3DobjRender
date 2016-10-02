# parameter setting
rootmodel="./collection/"
rootback_o="/Users/yidawang/Documents/database/backgrd_black/"
targetimage_o="../data/images_sp_object/"
rootback_p="/Users/yidawang/Documents/database/backgrd_flickr/"
targetimage_p="../data/images_sp_photo/"

# making directories
strings="aeroplane bicycle boat bottle bus car chair diningtable motorbike sofa train monitor"
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
	./semantic_render -objmodel=${rootmodel}${cat_array[0]}/${i}/model.obj  -label_class=1 -label_item=$i -semisphere=0 -imagedir_p=${targetimage_o}${cat_array[0]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[1]}/${i}/model.obj  -label_class=2 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[1]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[2]}/${i}/model.obj  -label_class=3 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[2]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[3]}/${i}/model.obj  -label_class=4 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[3]}  -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[4]}/${i}/model.obj  -label_class=5 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[4]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[5]}/${i}/model.obj  -label_class=6 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[5]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[6]}/${i}/model.obj  -label_class=7 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[6]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[7]}/${i}/model.obj  -label_class=8 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[7]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[8]}/${i}/model.obj  -label_class=9 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[8]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[9]}/${i}/model.obj  -label_class=10 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[9]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[10]}/${i}/model.obj  -label_class=11 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[10]} -frontalLight=1
	./semantic_render -objmodel=${rootmodel}${cat_array[11]}/${i}/model.obj  -label_class=12 -label_item=$i -z_range=0.8 -imagedir_p=${targetimage_o}${cat_array[11]} -frontalLight=1
done
