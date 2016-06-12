rootmodel="/Users/yidawang/Downloads/collection_origin/"
rootback="/Users/yidawang/Documents/database/backgrd_flickr/"
for i in $(seq 50)
do
./ReadOBJ -objmodel=${rootmodel}aeroplane/${i}/${i}.obj -mtlmodel=${rootmodel}aeroplane/${i}/${i}.mtl -texmodel=${rootmodel}aeroplane/${i}/ -label_class=1 -label_item=$i -bakgrdir=${rootback}aeroplane_sky/ -semisphere=0 -view_region=1
./ReadOBJ -objmodel=${rootmodel}bicycle/${i}/${i}.obj -mtlmodel=${rootmodel}bicycle/${i}/${i}.mtl -label_class=2 -label_item=$i -bakgrdir=${rootback}bicycle/ -z_range=0.3 -view_region=1
./ReadOBJ -objmodel=${rootmodel}boat/${i}/${i}.obj -mtlmodel=${rootmodel}boat/${i}/${i}.mtl -texmodel=${rootmodel}boat/${i}/ -label_class=3 -label_item=$i -bakgrdir=${rootback}boat/ -z_range=0.3 -view_region=1
./ReadOBJ -objmodel=${rootmodel}bus/${i}/${i}.obj -mtlmodel=${rootmodel}bus/${i}/${i}.mtl -texmodel=${rootmodel}bus/${i}/ -label_class=5 -label_item=$i -bakgrdir=${rootback}bus/ -z_range=0.3 -view_region=1
./ReadOBJ -objmodel=${rootmodel}car/${i}/${i}.obj -mtlmodel=${rootmodel}car/${i}/${i}.mtl -label_class=6 -label_item=$i -bakgrdir=${rootback}car/ -z_range=0.3 -view_region=1
./ReadOBJ -objmodel=${rootmodel}motorbike/${i}/${i}.obj -mtlmodel=${rootmodel}motorbike/${i}/${i}.mtl -label_class=9 -label_item=$i -bakgrdir=${rootback}motorbike/ -z_range=0.3 -view_region=1
./ReadOBJ -objmodel=${rootmodel}train/${i}/${i}.obj -mtlmodel=${rootmodel}train/${i}/${i}.mtl -texmodel=${rootmodel}train/${i}/ -label_class=11 -label_item=$i -bakgrdir=${rootback}train/ -z_range=0.3 -view_region=1
done
