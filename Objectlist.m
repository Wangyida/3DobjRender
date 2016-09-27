%loading information and files
clear;
direct = dir('/home/yida/Documents/buildboat/ReadOBJ/data/images_shapenets');
for i = 4:length(direct)
    filename{i-3} = direct(i).name;
end
label_class = zeros(length(filename),1);
label_item = label_class;
label_camdst = label_class;
label_coor = zeros(length(filename),3);
for i = 1:length(filename)
    label_class(i,1) = str2double(filename{i}(1:2));
    label_item(i,1) = str2double(filename{i}(4:5));
    label_coor(i,1) = str2double(filename{i}(7:10));
    label_coor(i,2) = str2double(filename{i}(12:15));
    label_coor(i,3) = str2double(filename{i}(17:20));
    label_camdst(i,1) = str2double(filename{i}(22:23));
end
for i = 1:12
    distribution(i) = length(find(label_class == i));
end
%%
%loading information from Imagenet
direct = dir('/home/yida/Documents/buildboat/readOBJ/data/images_sp_photo');
label_class = zeros(length(direct) - 3,1);
for i = 4:length(direct)
    filename{i-3} = direct(i).name;
    [~, ind] = find(filename{i-3} == '_');
    label_class(i-3) = str2num(filename{i-3}(1:ind-1));
end
for i = 1:12
    distribution(i) = length(find(label_class == i));
end
%%
%Regular Training Arrangement on Class
tic;
max_set = 100000;
reference = randperm(max_set);
reference = mod(reference, length(filename)) + 1;
list_out = cell(5 * max_set,1);
dist = zeros(length(filename),1);
for i = 1:max_set
    diff = label_coor - repmat(label_coor(reference(i),:),length(filename),1);
    for j = 1:length(filename)
        dist(j) = norm(diff(j,:));
    end
    idx_pos = find(label_class == label_class(reference(i)));
    idx_neg1 = find(label_class ~= label_class(reference(i)));
    idx_neg2 = find(label_class ~= label_class(reference(i)));
    idx_neg3 = find(label_class ~= label_class(reference(i)));
    list_out{5*(i-1)+1} = filename(reference(i));
    list_out{5*(i-1)+2} = filename(idx_pos(randperm(length(idx_pos),1)));
    list_out{5*(i-1)+3} = filename(idx_neg1(randperm(length(idx_neg1),1)));
    list_out{5*(i-1)+4} = filename(idx_neg2(randperm(length(idx_neg2),1)));
    list_out{5*(i-1)+5} = filename(idx_neg3(randperm(length(idx_neg3),1)));
end
toc;
%writing to txt files for Caffe
fileid = fopen('testlist_regular.txt', 'w');
for i = 1:size(list_out)
    fprintf(fileid, '/home/whdeng-k40/Desktop/images_all_rendered/%s %i\n', list_out{i}{1}, str2num(list_out{i}{1}(1:2)));
end
%%
%Special Arrangement on both Class and Pose
tic;
max_set = 500000;
reference = randperm(max_set);
reference = mod(reference, length(filename)) + 1;
list_out = cell(5 * max_set,1);
dist = zeros(length(filename),1);
for i = 1:max_set
    diff = label_coor - repmat(label_coor(reference(i),:),length(filename),1);
    for j = 1:length(filename)
        dist(j) = norm(diff(j,:));
    end
    if label_class(reference(i)) == 4
        idx_pos = find(label_class == label_class(reference(i)));
        idx_pos = intersect(idx_pos, find(label_item == label_item(reference(i))));
        %idx_pos = intersect(idx_pos, find(label_camdst == label_camdst(reference(i))));
        %idx_pos = intersect(idx_pos, find(dist == 0));
        idx_pos = intersect(idx_pos, find(dist ~= 0));
        idx_neg1 = find(label_class ~= label_class(reference(i)));
        idx_neg2 = find(label_class ~= label_class(reference(i)));
        idx_neg3 = find(label_class ~= label_class(reference(i)));
    else
        idx_pos = find(label_class == label_class(reference(i)));
        idx_pos = intersect(idx_pos, find(label_item == label_item(reference(i))));
        idx_pos = intersect(idx_pos, find(label_camdst == label_camdst(reference(i))));
        idx_pos = intersect(idx_pos, find(dist < 40));
        %idx_pos = intersect(idx_pos, find(dist == 0));
        idx_pos = intersect(idx_pos, find(dist ~= 0));
        idx_neg1 = find(label_class ~= label_class(reference(i)));
        idx_neg2 = find(label_class ~= label_class(reference(i)));
        idx_neg3 = find(label_class == label_class(reference(i)));
        idx_neg3 = intersect(idx_neg3, find(label_item == label_item(reference(i))));
        idx_neg3 = intersect(idx_neg3, find(label_camdst == label_camdst(reference(i))));
        idx_neg3 = intersect(idx_neg3, find(dist > 50));
    end
    list_out{5*(i-1)+1} = filename(reference(i));
    list_out{5*(i-1)+2} = filename(idx_pos(randperm(length(idx_pos),1)));
    list_out{5*(i-1)+3} = filename(idx_neg1(randperm(length(idx_neg1),1)));
    list_out{5*(i-1)+4} = filename(idx_neg2(randperm(length(idx_neg2),1)));
    list_out{5*(i-1)+5} = filename(idx_neg3(randperm(length(idx_neg3),1)));
end
toc;
%writing to txt files for Caffe
fileid = fopen('testlist_pose.txt', 'w');
for i = 1:size(list_out)
    fprintf(fileid, '/home/whdeng-k40/Desktop/images_all_rendered/%s %i\n', list_out{i}{1}, str2num(list_out{i}{1}(1:2)));
end
%%
%Arrange Files as a One Against All Data Set for Training
tic;
max_set = 10000;
class_tag = unique(label_class);
for group = 1:length(class_tag)
    group_member = find(label_class == class_tag(group));
    file_group{group} = filename(group_member);
    listlength(group) = length(file_group{group});
end
squence1 = randperm(max_set);
squence2 = randperm(max_set);
count = 1;
for i = 1:max_set
    for j = 1:length(class_tag)
        reference = mod(squence1(i), length(file_group{j})) + 1;
        list_out{count} = file_group{j}(reference);
        tag(count) = i;
        count = count+1;
        reference = mod(squence2(i), length(file_group{j})) + 1;
        list_out{count} = file_group{j}(reference);
        tag(count) = i;
        count = count+1;
        others = setdiff(1:length(class_tag),j);
        for k = others
            reference = mod(squence1(i), length(file_group{k})) + 1;
            list_out{count} = file_group{k}(reference);
            tag(count) = k;
            count = count+1;
        end
    end
end
toc;
%writing to txt files for Caffe
fileid = fopen('testlist_oneagall.txt', 'w');
for i = 1:length(list_out)
    fprintf(fileid, '/Users/yidawang/Documents/buildboat/imagegen/data/images_all/%s %i\n', list_out{i}{1}, tag(i));
end
%%
%Using Real World Images for Training
direct = dir('/Users/yidawang/Documents/database/Imagenet');
label_class = zeros(length(direct) - 3,1);
for i = 4:length(direct)
    filename{i-3} = direct(i).name;
    [~, ind] = find(filename{i-3} == '_');
    label_class(i-3) = str2num(filename{i-3}(1:ind-1));
end
max_set = 100000;
reference = randperm(max_set);
reference = mod(reference, length(filename)) + 1;
list_out = cell(5 * max_set,1);
dist = zeros(length(filename),1);
for i = 1:max_set
    idx_pos = find(label_class == label_class(reference(i)));
    idx_neg1 = find(label_class ~= label_class(reference(i)));
    idx_neg2 = find(label_class ~= label_class(reference(i)));
    idx_neg3 = find(label_class ~= label_class(reference(i)));
    list_out{5*(i-1)+1} = filename(reference(i));
    list_out{5*(i-1)+2} = filename(idx_pos(randperm(length(idx_pos),1)));
    list_out{5*(i-1)+3} = filename(idx_neg1(randperm(length(idx_neg1),1)));
    list_out{5*(i-1)+4} = filename(idx_neg2(randperm(length(idx_neg2),1)));
    list_out{5*(i-1)+5} = filename(idx_neg3(randperm(length(idx_neg3),1)));
end
%writing to txt files for Caffe
fileid = fopen('trainlist_imagenet.txt', 'w');
for i = 1:length(list_out)
    [~, ind] = find(list_out{i}{1} == '_');
    label = str2num(list_out{i}{1}(1:ind-1));
    fprintf(fileid, '/Users/yidawang/Documents/database/imagenet/%s %i\n', list_out{i}{1}, label);
end
%%
%List for Softmax, no arrangement for triplet loss

%idx_down = [];
%class_label = unique(label_class);
%for group = 1:length(class_label)
%    group_id = class_label(group);
%    group_member = find(label_class == group_id);
%    idx_down = [idx_down; group_member(randperm(length(group_member),1700))];
%end
%filename = filename(idx_down);
%label_class = label_class(idx_down);
clear;
direct = dir('/home/yida/Documents/buildboat/readOBJ/data/images_sp_photo');
for i = 3:length(direct)
    filename{i-2} = direct(i).name;
end
tic;
max_set = length(filename);
squence1 = randperm(max_set);
count = 1;
for i = 1:max_set
    list_out{count} = filename{squence1(i)};
    count = count+1;
end
toc;
%writing to txt files for Caffe
fileid = fopen('traininglist_softmax_shapenet.txt', 'w');
for i = 1:length(list_out)
    fprintf(fileid, '/home/whdeng-k40/experiment_yida/data/images_sp_photo/%s %i\n', list_out{i}, str2num(list_out{i}(1:2)));
end
%%
tic;
clear;
file_nouse = 3;
%loading information from superpixel arrangement
direct = dir('/Users/yidawang/Documents/database/Imagenet');
label_class = zeros(length(direct) - file_nouse,1);
for i = file_nouse+1:length(direct)
    filename{i-file_nouse} = direct(i).name;
    [~, ind] = find(filename{i-file_nouse} == '_');
    label_class(i-file_nouse) = str2num(filename{i-file_nouse}(1:ind-1));
end
%arrange
max_set = length(filename);
squence1 = randperm(max_set);
count = 1;
for i = 1:max_set
    list_out{count} = filename{squence1(i)};
    count = count+1;
end
%writing to txt files for Caffe
fileid1 = fopen('testlist_sp_imagenet.txt', 'w');
fileid2 = fopen('testlist_sp_dl_imagenet.txt', 'w');
fileid3 = fopen('testlist_sp_dc_imagenet.txt', 'w');
for i = 1:length(list_out)
    sp_list = dir(strcat('/Users/yidawang/Documents/buildboat/SLIC-superpixel/data/images_sp_imagenet/',list_out{i}(1:end-4)));
    for j = 3:length(sp_list)
        fprintf(fileid3, '/home/whdeng-k40/experiment_yida/data/images_sp_imagenet/%s/%s %i\n', list_out{i}(1:end-4), sp_list(j).name, 1);
        fprintf(fileid1, '/home/whdeng-k40/experiment_yida/data/Imagenet/%s %i\n', list_out{i}, str2num(list_out{i}(1:2)));
        fprintf(fileid2, '/home/whdeng-k40/experiment_yida/data/Imagenet/%s %i\n', list_out{i}, str2num(list_out{i}(1:2)));
    end
end
toc;
%% Random arrangement for training list
clear all;
tic;
fidin1 = fopen('testlist_sp_photo.txt');
fidin2 = fopen('testlist_sp_dl.txt');
fidin3 = fopen('testlist_sp_dc.txt');
fidout1 = fopen('testlist_sp_photo_shuffle.txt', 'w');
fidout2 = fopen('testlist_sp_dl_shuffle.txt', 'w');
fidout3 = fopen('testlist_sp_dc_shuffle.txt','w');
index = 0;
while ~feof(fidin3)                               
   tline1=fgetl(fidin1);                               
   tline2=fgetl(fidin2);                               
   tline3=fgetl(fidin3);
   index = index+1;
   str1{index} = tline1;
   str2{index} = tline2;
   str3{index} = tline3;
end

rand_index = randperm(index);

for i=1:index
    fprintf(fidout1, '%s\n',str1{rand_index(i)});
    fprintf(fidout2, '%s\n',str2{rand_index(i)});
    fprintf(fidout3, '%s\n',str3{rand_index(i)});
end
toc;