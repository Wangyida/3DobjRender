#include "vtkBMPReader.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkOBJImporter.h"
#include "vtkPNGWriter.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include "vtkPointData.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageReader.h"
#include "vtkJPEGReader.h"
#include "vtkImageCanvasSource2D.h"
#include "vtkImageCast.h"
#include <vtkLight.h>
#include "vtkCamera.h"
#include "vtkTexture.h"
#include "vtkWindowToImageFilter.h"
#include "vtksys/SystemTools.hxx"
#include <opencv2/viz/vizcore.hpp>
#include "sphereview.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace cv;
using namespace std;

void listDir(const char *path, std::vector<String>& files, bool r)
{
  DIR *pDir;
  struct dirent *ent;
  char childpath[512];
  pDir = opendir(path);
  memset(childpath, 0, sizeof(childpath));
  while ((ent = readdir(pDir)) != NULL)
  {
	if (ent->d_type & DT_DIR)
	{
	  if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".DS_Store") == 0 || strcmp(ent->d_name, "._.DS_Store") == 0)
	  {
		continue;
	  }
	  if (r)
	  {
		sprintf(childpath, "%s/%s", path, ent->d_name);
		listDir(childpath,files,false);
	  }
	}
	else
	{
	  if (strcmp(ent->d_name, ".DS_Store") != 0)
	  files.push_back(ent->d_name);
	}
  }
  sort(files.begin(),files.end());
};

int main( int argc, char * argv [] )
{
  const String keys = "{help | | demo :$ ./sphereview_test -ite_depth=2 -plymodel=../data/3Dmodel/ape.ply -imagedir=../data/images_all/ -labeldir=../data/label_all.txt -num_class=6 -label_class=0, then press 'q' to run the demo for images generation when you see the gray background and a coordinate.}"
  "{ite_depth | 3 | Iteration of sphere generation, suggested: 3.}"
  "{objmodel | | Path of the '.obj' file for image rendering. }"
  "{mtlmodel | | Path of the '.mtl' file for image rendering. }"
  "{texmodel | | Path of the texture file for image rendering. }"
  "{imagedir_p | ../data/images_sp_photo/ | Path of the generated images for one particular .ply model. }"
  "{imagedir_o | ../data/images_sp_object/ | Path of the generated images for one particular .ply model. }"
  "{labeldir | ../data/label_all.txt | Path of the generated images for one particular .ply model. }"
  "{bakgrdir_p | /home/yida/Documents/database/backgrd_flickr/ | Path of the backgroud images sets. }"
  "{bakgrdir_o | /home/yida/Documents/database/backgrd_black/ | Path of the backgroud images sets. }"
  "{semisphere | 1 | Camera only has positions on half of the whole sphere. }"
  "{z_range | 0.6 | Maximum camera position on z axis. }"
  "{center_gen | 0 | Find center from all points. }"
  "{image_size | 227 | Size of captured images. }"
  "{label_class |  | Class label of current .ply model. }"
  "{label_item |  | Item label of current .ply model. }"
  "{rgb_use | 0 | Use RGB image or grayscale. }"
  "{num_class | 12 | Total number of classes of models. }"
  "{view_region | 0 | Take a special view of front or back angle}"
  "{frontalLight | 0 | Frontal light to from camera}";
  /* Get parameters from comand line. */
  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("Generating training data for CNN with triplet loss");
  if (parser.has("help"))
  {
	parser.printMessage();
	return 0;
  }
  int ite_depth = parser.get<int>("ite_depth");
  String objmodel = parser.get<String>("objmodel");
  String mtlmodel = parser.get<String>("mtlmodel");
  String texmodel = parser.get<String>("texmodel");
  String imagedir_p = parser.get<String>("imagedir_p");
  String imagedir_o = parser.get<String>("imagedir_o");
  string labeldir = parser.get<String>("labeldir");
  String bakgrdir_p = parser.get<string>("bakgrdir_p");
  String bakgrdir_o = parser.get<string>("bakgrdir_o");
  int label_class = parser.get<int>("label_class");
  int label_item = parser.get<int>("label_item");
  int semisphere = parser.get<int>("semisphere");
  float z_range = parser.get<float>("z_range");
  int center_gen = parser.get<int>("center_gen");
  int image_size = parser.get<int>("image_size");
  int rgb_use = parser.get<int>("rgb_use");
  int num_class = parser.get<int>("num_class");
  int view_region = parser.get<int>("view_region");
  int frontalLight = parser.get<int>("frontalLight");
  double bg_dist, y_range;
  y_range = 0.25;
  cv::cnn_3dobj::icoSphere ViewSphere(10,ite_depth);
  std::vector<cv::Point3d> campos;
  std::vector<cv::Point3d> campos_temp = ViewSphere.CameraPos;
  /* Regular objects on the ground using a semisphere view system */
  if (semisphere == 1)
  {
	if (view_region == 1)
	{
	  for (int pose = 0; pose < static_cast<int>(campos_temp.size()); pose++)
	  {
		if (campos_temp.at(pose).z >= 0 && campos_temp.at(pose).z < z_range && std::abs(campos_temp.at(pose).y) < y_range)
		campos.push_back(campos_temp.at(pose));
	  }
	}
	else
	{
			for (int pose = 0; pose < static_cast<int>(campos_temp.size()); pose++)
			{
			  if (campos_temp.at(pose).z >= -0.3 && campos_temp.at(pose).z < z_range)
					campos.push_back(campos_temp.at(pose));
			}
	}
  }
  /* Special object such as plane using a full space of view sphere */
  else
  {
	if (view_region == 1)
	{
	  for (int pose = 0; pose < static_cast<int>(campos_temp.size()); pose++)
	  {
		if (campos_temp.at(pose).z < 0.2 && campos_temp.at(pose).z > -0.1 && abs(campos_temp.at(pose).y) < y_range)
		campos.push_back(campos_temp.at(pose));
	  }
	}
	else
	{
	  for (int pose = 0; pose < static_cast<int>(campos_temp.size()); pose++)
	  {
		if (campos_temp.at(pose).z < 0.3 && campos_temp.at(pose).z > -0.8)
		campos.push_back(campos_temp.at(pose));
	  }
	}
  }
  
  std::string filenameOBJ(objmodel.c_str());
  std::string filenameMTL(mtlmodel.c_str());
  vtkSmartPointer<vtkOBJImporter> importer = vtkSmartPointer<vtkOBJImporter>::New();
  importer->SetFileName(filenameOBJ.data());
  importer->SetFileNameMTL(filenameMTL.data());
  importer->SetTexturePath(texmodel.c_str());
  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renWin->AddRenderer(ren);
  ren->TexturedBackgroundOn();
  ren->GetActiveCamera()->SetViewUp(0,1,0);
  iren->SetRenderWindow(renWin);
  importer->SetRenderWindow(renWin);
  importer->Update();
  renWin->SetSize(227,227);
  renWin->Render();
  vtkSmartPointer<vtkJPEGReader> imReader = vtkSmartPointer<vtkJPEGReader>::New();
  vtkSmartPointer<vtkTexture> atext = vtkSmartPointer<vtkTexture>::New();
  atext->SetInputConnection(imReader->GetOutputPort());
  atext->InterpolateOn();
  ren->SetBackground(0,0,0);
  ren->SetBackgroundTexture(atext); //add background
  vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
  vtkSmartPointer<vtkWindowToImageFilter> wintoimg = vtkSmartPointer<vtkWindowToImageFilter>::New();
  wintoimg->SetInput(renWin);
  writer->SetInputConnection(wintoimg->GetOutputPort());
  vtkSmartPointer<vtkLight> myLight= vtkSmartPointer<vtkLight>::New();
  myLight->SetColor(1,1,1);
  
  if (frontalLight == 1) ren->AddLight(myLight);
  
  char temp[128];
  char* bgname = new char;
  std::vector<String> name_bkg_p, name_bkg_o;
  std::fstream imglabel;
  char* p=(char*)labeldir.data();
  imglabel.open(p, fstream::app|fstream::out);
  if (bakgrdir_p.size() != 0)
  {
	/* List the file names under a given path */
	listDir(bakgrdir_p.c_str(), name_bkg_p, false);
	for (unsigned int i = 0; i < name_bkg_p.size(); i++)
	{
	  name_bkg_p.at(i) = bakgrdir_p + name_bkg_p.at(i);
	}
  }
  if (bakgrdir_o.size() != 0)
  {
	/* List the file names under a given path */
	listDir(bakgrdir_o.c_str(), name_bkg_o, false);
	for (unsigned int i = 0; i < name_bkg_o.size(); i++)
	{
	  name_bkg_o.at(i) = bakgrdir_o + name_bkg_o.at(i);
	}
  }
  /* Images will be saved as .png files. */
  int cnt_img;
  /* Real random related to time */
  srand((int)time(0));
  double dist_shift_factor=0.06, shift_x, shift_y, shift_z, dist_cam_factor;
  do
  {
	cnt_img = 0;
	for(int pose = 0; pose < static_cast<int>(campos.size()); pose++){
	  int label_x, label_y, label_z;
	  label_x = static_cast<int>(campos.at(pose).x*100);
	  label_y = static_cast<int>(campos.at(pose).y*100);
	  label_z = static_cast<int>(campos.at(pose).z*100);
	  shift_x = rand()%10*dist_shift_factor;
	  shift_y = rand()%10*dist_shift_factor;
	  shift_z = rand()%10*dist_shift_factor;
    dist_cam_factor = 2 * (rand()%3+1);
	  ren->GetActiveCamera()->SetFocalPoint(shift_x,shift_y,shift_z);
	  ren->GetActiveCamera()->SetPosition(campos.at(pose).x*dist_cam_factor,campos.at(pose).z*dist_cam_factor,campos.at(pose).y*dist_cam_factor);
	  if (frontalLight == 1) {
  		myLight->SetPosition(campos.at(pose).x*dist_cam_factor*10,campos.at(pose).z*dist_cam_factor*10,campos.at(pose).y*dist_cam_factor*10);
		myLight->SetFocalPoint(ren->GetActiveCamera()->GetFocalPoint());
  		myLight->SetIntensity(50);
	  }
	  sprintf (temp,"%02i_%02i_%03i", label_class, label_item, cnt_img);
	  /* Photo */
	  String filename = temp;
	  filename += ".png";
	  imglabel << filename << ' ' << label_class << endl;
	  filename = imagedir_p + filename;
	  imReader->SetFileName(name_bkg_p.at(rand()%name_bkg_p.size()).c_str());
	  /*if (view_region == 0) {
		ren->ResetCamera();// set to a camera mode suit for object
	  }*/
	  wintoimg->Modified();
	  wintoimg->Update();
	  writer->SetFileName(filename.c_str());
	  writer->Write();
  
	  iren->Initialize();
	  /* Object */
	  filename = temp;
	  filename += ".png";
	  imglabel << filename << ' ' << label_class << endl;
	  filename = imagedir_o + filename;
	  imReader->SetFileName(name_bkg_o.at(rand()%name_bkg_o.size()).c_str());
	  /*if (view_region == 0) {
		ren->ResetCamera();// set to a camera mode suit for object
	  }*/
	  wintoimg->Modified();
	  wintoimg->Update();
	  writer->SetFileName(filename.c_str());
	  writer->Write();
  
	  iren->Initialize();
	  //iren->NewInstance();
	  cnt_img++;
	}
  } while (cnt_img != campos.size());
  imglabel.close();
  return 1;
}
