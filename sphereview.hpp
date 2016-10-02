#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <set>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


#include "opencv2/viz/vizcore.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc.hpp"
/** @brief Icosohedron based camera view data generator.
 The class create some sphere views of camera towards a 3D object meshed from .ply files @cite hinterstoisser2008panter .
 */
using namespace cv;
using namespace std;
/************************************ Data Generation Class ************************************/
class icoSphere
{
    private:
    /** @brief X position of one base point on the initial Icosohedron sphere,
      Y is set to be 0 as default.
     */
    float X;

    /** @brief Z position of one base point on the initial Icosohedron sphere.
     */
    float Z;

    /** @brief A threshold for the dupicated points elimination.
     */
    float diff;

    /** @brief Temp camera position for duplex position elimination.
     */
    std::vector<cv::Point3d> CameraPos_temp;

    /** @brief Make all view points having the same distance from the focal point used by the camera view.
     */
    void norm(float v[]);

    /** @brief Add a new view point.
     */
    void add(float v[]);

    /** @brief Generate new view points from all triangles.
     */
    void subdivide(float v1[], float v2[], float v3[], int depth);

    public:
    /** @brief Camera position on the sphere after duplicated points elimination.
     */
    std::vector<cv::Point3d> CameraPos;

    /** @brief Generating a sphere by mean of a iteration based points selection process.
    @param radius_in Another radius used for adjusting the view distance.
    @param depth_in Number of interations for increasing the points on sphere.
     */
    icoSphere(float radius_in, int depth_in);

    /** @brief Get the center of points on surface in .ply model.
    @param cloud Point cloud used for computing the center point.
     */
    cv::Point3d getCenter(cv::Mat cloud);

    /** @brief Get the proper camera radius from the view point to the center of model.
    @param cloud Point cloud used for computing the center point.
    @param center center point of the point cloud.
     */
    float getRadius(cv::Mat cloud, cv::Point3d center);

    /** @brief Suit the position of bytes in 4 byte data structure for particular system.
     */
    static int swapEndian(int val);

    /** @brief Create header in binary files collecting the image data and label.
    @param num_item Number of items.
    @param rows Rows of a single sample image.
    @param cols Columns of a single sample image.
    @param headerPath Path where the header will be stored.
     */
    static void createHeader(int num_item, int rows, int cols, const char* headerPath);

    /** @brief Write binary files used for training in other open source project including Caffe.
    @param filenameImg Path which including a set of images.
    @param binaryPath Path which will output a binary file.
    @param headerPath Path which header belongs to.
    @param num_item Number of samples.
    @param label_class Class label of the sample.
    @param x Pose label of X.
    @param y Pose label of Y.
    @param z Pose label of Z.
    @param isrgb Option for choice of using RGB images or not.
     */
    static void writeBinaryfile(String filenameImg, const char* binaryPath, const char* headerPath, int num_item, int label_class, int x, int y, int z, int isrgb);
};

icoSphere::icoSphere(float radius_in, int depth_in)
{
    X = 0.5f;
    Z = 0.5f;
    float vdata[12][3] = { { -X, 0.0f, Z }, { X, 0.0f, Z },
      { -X, 0.0f, -Z }, { X, 0.0f, -Z }, { 0.0f, Z, X }, { 0.0f, Z, -X },
      { 0.0f, -Z, X }, { 0.0f, -Z, -X }, { Z, X, 0.0f }, { -Z, X, 0.0f },
      { Z, -X, 0.0f }, { -Z, -X, 0.0f } };
    int tindices[20][3] = { { 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 },
      { 4, 5, 8 }, { 4, 8, 1 }, { 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 },
      { 5, 2, 3 }, { 2, 7, 3 }, { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 },
      { 11, 0, 6 }, { 0, 1, 6 }, { 6, 1, 10 }, { 9, 0, 11 },
      { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 } };
    diff = 0.00000001;
    X *= (int)radius_in;
    Z *= (int)radius_in;

    // Iterate over points
    for (int i = 0; i < 20; ++i)
    {
        subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]],
          vdata[tindices[i][2]], depth_in);
    }
    CameraPos_temp.push_back(CameraPos[0]);
    for (unsigned int j = 1; j < CameraPos.size(); ++j)
    {
        for (unsigned int k = 0; k < j; ++k)
        {
            float dist_x, dist_y, dist_z;
            dist_x = (CameraPos.at(k).x-CameraPos.at(j).x) * (CameraPos.at(k).x-CameraPos.at(j).x);
            dist_y = (CameraPos.at(k).y-CameraPos.at(j).y) * (CameraPos.at(k).y-CameraPos.at(j).y);
            dist_z = (CameraPos.at(k).z-CameraPos.at(j).z) * (CameraPos.at(k).z-CameraPos.at(j).z);
            if (dist_x < diff && dist_y < diff && dist_z < diff)
                break;
            else if (k == j-1)
                CameraPos_temp.push_back(CameraPos[j]);
        }
    }
    CameraPos = CameraPos_temp;
    cout << "View points in total: " << CameraPos.size() << endl;
    /*
    cout << "The coordinate of view point: " << endl;
    for(unsigned int i = 0; i < CameraPos.size(); i++)
    {
        cout << CameraPos.at(i).x <<' '<< CameraPos.at(i).y << ' ' << CameraPos.at(i).z << endl;
    }*/
};
void icoSphere::norm(float v[])
{
    float len = 0;
    for (int i = 0; i < 3; ++i)
    {
        len += v[i] * v[i];
    }
    len = sqrt(len);
    for (int i = 0; i < 3; ++i)
    {
        v[i] /= ((float)len);
    }
};

void icoSphere::add(float v[])
{
    Point3f temp_Campos;
    std::vector<float>* temp = new std::vector<float>;
    for (int k = 0; k < 3; ++k)
    {
        temp->push_back(v[k]);
    }
    temp_Campos.x = temp->at(0);temp_Campos.y = temp->at(1);temp_Campos.z = temp->at(2);
    CameraPos.push_back(temp_Campos);
};

void icoSphere::subdivide(float v1[], float v2[], float v3[], int depth)
{
    norm(v1);
    norm(v2);
    norm(v3);
    if (depth == 0)
    {
        add(v1);
        add(v2);
        add(v3);
        return;
    }
    float* v12 = new float[3];
    float* v23 = new float[3];
    float* v31 = new float[3];
    for (int i = 0; i < 3; ++i)
    {
        v12[i] = (v1[i] + v2[i]) / 2;
        v23[i] = (v2[i] + v3[i]) / 2;
        v31[i] = (v3[i] + v1[i]) / 2;
    }
    norm(v12);
    norm(v23);
    norm(v31);
    subdivide(v1, v12, v31, depth - 1);
    subdivide(v2, v23, v12, depth - 1);
    subdivide(v3, v31, v23, depth - 1);
    subdivide(v12, v23, v31, depth - 1);
};

int icoSphere::swapEndian(int val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
};

cv::Point3d icoSphere::getCenter(cv::Mat cloud)
{
    Point3f* data = cloud.ptr<cv::Point3f>();
    Point3d dataout;
    for(int i = 0; i < cloud.cols; ++i)
    {
        dataout.x += data[i].x;
        dataout.y += data[i].y;
        dataout.z += data[i].z;
    }
    dataout.x = dataout.x/cloud.cols;
    dataout.y = dataout.y/cloud.cols;
    dataout.z = dataout.z/cloud.cols;
    return dataout;
};

float icoSphere::getRadius(cv::Mat cloud, cv::Point3d center)
{
    float radiusCam = 0;
    Point3f* data = cloud.ptr<cv::Point3f>();
    Point3d datatemp;
    for(int i = 0; i < cloud.cols; ++i)
    {
        datatemp.x = data[i].x - (float)center.x;
        datatemp.y = data[i].y - (float)center.y;
        datatemp.z = data[i].z - (float)center.z;
        float Radius = sqrt(pow(datatemp.x,2)+pow(datatemp.y,2)+pow(datatemp.z,2));
        if(Radius > radiusCam)
        {
            radiusCam = Radius;
        }
    }
    return radiusCam;
};

void icoSphere::createHeader(int num_item, int rows, int cols, const char* headerPath)
{
    char* a0 = (char*)malloc(1024);
    strcpy(a0, headerPath);
    char a1[] = "image";
    char a2[] = "label";
    char* headerPathimg = (char*)malloc(1024);
    strcpy(headerPathimg, a0);
    strcat(headerPathimg, a1);
    char* headerPathlab = (char*)malloc(1024);
    strcpy(headerPathlab, a0);
    strcat(headerPathlab, a2);
    std::ofstream headerImg(headerPathimg, ios::out|ios::binary);
    std::ofstream headerLabel(headerPathlab, ios::out|ios::binary);
    int headerimg[4] = {2051,num_item,rows,cols};
    for (int i=0; i<4; i++)
        headerimg[i] = swapEndian(headerimg[i]);
    int headerlabel[2] = {2050,num_item};
    for (int i=0; i<2; i++)
        headerlabel[i] = swapEndian(headerlabel[i]);
    headerImg.write(reinterpret_cast<const char*>(headerimg), sizeof(int)*4);
    headerImg.close();
    headerLabel.write(reinterpret_cast<const char*>(headerlabel), sizeof(int)*2);
    headerLabel.close();
};

void icoSphere::writeBinaryfile(String filenameImg, const char* binaryPath, const char* headerPath, int num_item, int label_class, int x, int y, int z, int isrgb)
{
    cv::Mat ImgforBin = cv::imread(filenameImg, isrgb);
    char* A0 = (char*)malloc(1024);
    strcpy(A0, binaryPath);
    char A1[] = "image";
    char A2[] = "label";
    char* binPathimg = (char*)malloc(1024);
    strcpy(binPathimg, A0);
    strcat(binPathimg, A1);
    char* binPathlab = (char*)malloc(1024);
    strcpy(binPathlab, A0);
    strcat(binPathlab, A2);
    fstream img_file, lab_file;
    img_file.open(binPathimg,ios::in);
    lab_file.open(binPathlab,ios::in);
    if(!img_file)
    {
        cout << "Creating the training data at: " << binaryPath << ". " << endl;
        char* a0 = (char*)malloc(1024);
        strcpy(a0, headerPath);
        char a1[] = "image";
        char a2[] = "label";
        char* headerPathimg = (char*)malloc(1024);
        strcpy(headerPathimg, a0);
        strcat(headerPathimg,a1);
        char* headerPathlab = (char*)malloc(1024);
        strcpy(headerPathlab, a0);
        strcat(headerPathlab,a2);
        createHeader(num_item, 64, 64, binaryPath);
        img_file.open(binPathimg,ios::out|ios::binary|ios::app);
        lab_file.open(binPathlab,ios::out|ios::binary|ios::app);
        if (isrgb == 0)
        {
            for (int r = 0; r < ImgforBin.rows; r++)
            {
                img_file.write(reinterpret_cast<const char*>(ImgforBin.ptr(r)), ImgforBin.cols*ImgforBin.elemSize());
            }
        }
        else
        {
            std::vector<cv::Mat> Img3forBin;
            cv::split(ImgforBin,Img3forBin);
            for (unsigned int i = 0; i < Img3forBin.size(); i++)
            {
                for (int r = 0; r < Img3forBin[i].rows; r++)
                {
                    img_file.write(reinterpret_cast<const char*>(Img3forBin[i].ptr(r)), Img3forBin[i].cols*Img3forBin[i].elemSize());
                }
            }
        }
        signed char templab = (signed char)label_class;
        lab_file << templab << (signed char)x << (signed char)y << (signed char)z;
    }
    else
    {
        img_file.close();
        lab_file.close();
        img_file.open(binPathimg,ios::out|ios::binary|ios::app);
        lab_file.open(binPathlab,ios::out|ios::binary|ios::app);
        cout <<"Concatenating the training data at: " << binaryPath << ". " << endl;
        if (isrgb == 0)
        {
            for (int r = 0; r < ImgforBin.rows; r++)
            {
                img_file.write(reinterpret_cast<const char*>(ImgforBin.ptr(r)), ImgforBin.cols*ImgforBin.elemSize());
            }
        }
        else
        {
            std::vector<cv::Mat> Img3forBin;
            cv::split(ImgforBin,Img3forBin);
            for (unsigned int i = 0; i < Img3forBin.size(); i++)
            {
                for (int r = 0; r < Img3forBin[i].rows; r++)
                {
                    img_file.write(reinterpret_cast<const char*>(Img3forBin[i].ptr(r)), Img3forBin[i].cols*Img3forBin[i].elemSize());
                }
            }
        }
        signed char templab = (signed char)label_class;
        lab_file << templab << (signed char)x << (signed char)y << (signed char)z;
    }
    img_file.close();
    lab_file.close();
};
