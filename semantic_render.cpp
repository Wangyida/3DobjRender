#include "vtkBMPReader.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkOBJReader.h>
#include "vtkOBJImporter.h"
#include "vtkJPEGWriter.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include "vtkPointData.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageReader.h"
#include "vtkJPEGReader.h"
#include "vtkImageCanvasSource2D.h"
#include "vtkImageCast.h"
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkLight.h>
#include <vtkLightActor.h>
#include <vtkSphereSource.h>
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
	  if (strcmp(ent->d_name, ".DS_Store") != 0 && strcmp(ent->d_name, "._.DS_Store") != 0)
	  files.push_back(ent->d_name);
	}
  }
  sort(files.begin(),files.end());
};

int main( int argc, char * argv [] )
{
  const String keys = "{help | | demo :$ ./sphereview_test -ite_depth=2 -plymodel=../data/3Dmodel/ape.ply -imagedir=../data/images_all/ -labeldir=../data/label_all.txt -num_class=6 -label_class=0, then press 'q' to run the demo for images generation when you see the gray background and a coordinate.}"
  "{ite_depth | 2 | Iteration of sphere generation, suggested: 2.}"
  "{objmodel | | Path of the '.obj' file for image rendering. }"
  "{mtlmodel | | Path of the '.mtl' file for image rendering. }"
  "{texmodel | | Path of the texture file for image rendering. }"
  "{imagedir_p | ../data/images_sp_photo/ | Path of the generated images for one particular .ply model. }"
  "{labeldir | ../data/label_all.txt | Path of the generated images for one particular .ply model. }"
  "{bakgrdir_p | /home/yida/Documents/database/backgrd_black/ | Path of the backgroud images sets. }"
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
  string labeldir = parser.get<String>("labeldir");
  String bakgrdir_p = parser.get<string>("bakgrdir_p");
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
  
  vtkSmartPointer<vtkOBJReader> reader =
    vtkSmartPointer<vtkOBJReader>::New();
  reader->SetFileName(objmodel.c_str());
  reader->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
 
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renWin->AddRenderer(ren);
  ren->AddActor(actor);
  ren->TexturedBackgroundOn();
  ren->GetActiveCamera()->SetViewUp(0,1,0);
  iren->SetRenderWindow(renWin);
  renWin->SetSize(227,227);
  renWin->Render();
  vtkSmartPointer<vtkJPEGReader> imReader = vtkSmartPointer<vtkJPEGReader>::New();
  vtkSmartPointer<vtkTexture> atext = vtkSmartPointer<vtkTexture>::New();
  atext->SetInputConnection(imReader->GetOutputPort());
  atext->InterpolateOn();
  ren->SetBackground(0,0,0);
  ren->SetBackgroundTexture(atext); //add background
  vtkSmartPointer<vtkJPEGWriter> writer = vtkSmartPointer<vtkJPEGWriter>::New();
  vtkSmartPointer<vtkWindowToImageFilter> wintoimg = vtkSmartPointer<vtkWindowToImageFilter>::New();
  wintoimg->SetInput(renWin);
  writer->SetInputConnection(wintoimg->GetOutputPort());
  //myLightB->SetIntensity(10);
  //myLightF->SetPosition(10, 10, 0);
  vtkSmartPointer<vtkLightActor> lightActor = vtkSmartPointer<vtkLightActor>::New();
  vtkSmartPointer<vtkPolyDataMapper> lightFocalPointMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  lightFocalPointMapper->SetInputConnection(reader->GetOutputPort());
 
  vtkSmartPointer<vtkActor> lightFocalPointActor = vtkSmartPointer<vtkActor>::New();
  lightFocalPointActor->SetMapper(lightFocalPointMapper);
  double combi[12][3] = {{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1},{1,0.38,0},{0.75,0.75,0.75},{0.73,0.56,0.56},{0.85,0.44,0.84},{0.5,0,1},{0.5,1,0.83}};
  switch (label_class) {
    case 1:
      lightFocalPointActor->GetProperty()->SetColor(combi[0]);
      break; 
    case 2:
      lightFocalPointActor->GetProperty()->SetColor(combi[1]);
      break; 
    case 3:
      lightFocalPointActor->GetProperty()->SetColor(combi[2]);
      break; 
    case 4:
      lightFocalPointActor->GetProperty()->SetColor(combi[3]);
      break; 
    case 5:
      lightFocalPointActor->GetProperty()->SetColor(combi[4]);
      break; 
    case 6:
      lightFocalPointActor->GetProperty()->SetColor(combi[5]);
      break; 
    case 7:
      lightFocalPointActor->GetProperty()->SetColor(combi[6]);
      break; 
    case 8:
      lightFocalPointActor->GetProperty()->SetColor(combi[7]);
      break; 
    case 9:
      lightFocalPointActor->GetProperty()->SetColor(combi[8]);
      break; 
    case 10:
      lightFocalPointActor->GetProperty()->SetColor(combi[9]);
      break; 
    case 11:
      lightFocalPointActor->GetProperty()->SetColor(combi[10]);
      break; 
    case 12:
      lightFocalPointActor->GetProperty()->SetColor(combi[11]);
      break; 
  }

  ren->AddViewProp(lightFocalPointActor);
  
  char temp[32];
  char* bgname = new char;
  std::vector<String> name_bkg_p;
  if (bakgrdir_p.size() != 0)
  {
  	/* List the file names under a given path */
  	listDir(bakgrdir_p.c_str(), name_bkg_p, false);
  	for (unsigned int i = 0; i < name_bkg_p.size(); i++)
  	{
  	  name_bkg_p.at(i) = bakgrdir_p + name_bkg_p.at(i);
  	}
  }
  /* Images will be saved as .png files. */
  int cnt_img;
  /* Real random related to time */
  // srand((int)time(0));
  srand(1);
  double dist_shift_factor=0.04, shift_x, shift_y, shift_z, dist_cam_factor;
  do
  {
	cnt_img = 0;
	for(int pose = 0; pose < static_cast<int>(campos.size()); pose++){
      iren->Initialize();
	  int label_x, label_y, label_z;
	  label_x = static_cast<int>(campos.at(pose).x*100);
	  label_y = static_cast<int>(campos.at(pose).y*100);
	  label_z = static_cast<int>(campos.at(pose).z*100);
	  shift_x = (rand()%7-3)*dist_shift_factor;
	  shift_y = (rand()%7-3)*dist_shift_factor;
	  shift_z = (rand()%7-3)*dist_shift_factor;
      dist_cam_factor = (rand()%5+11)/5;
	  ren->GetActiveCamera()->SetFocalPoint(shift_x,shift_y,shift_z);
	  ren->GetActiveCamera()->SetPosition(campos.at(pose).x*dist_cam_factor,campos.at(pose).z*dist_cam_factor,campos.at(pose).y*dist_cam_factor);

	  sprintf (temp,"%02i_%02i_%03i", label_class, label_item, cnt_img);
      String filename = temp;

	  filename += ".jpg";
	  filename = imagedir_p + '/' + filename;
	  imReader->SetFileName(name_bkg_p.at(rand()%(name_bkg_p.size() - 1)).c_str());
	  wintoimg->Modified();
	  wintoimg->Update();
	  writer->SetFileName(filename.c_str());
	  writer->Write();
      iren->Initialize();
  
	  //iren->NewInstance();
	  cnt_img++;
	}
  } while (cnt_img != campos.size());
  return 1;
}
