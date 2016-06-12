#include "vtkBMPReader.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkOBJImporter.h"
#include "vtkOBJReader.h"
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
#include <opencv2/cnn_3dobj.hpp>
#include <opencv2/viz/vizcore.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace cv;
using namespace std;
using namespace cv::cnn_3dobj;

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
	  if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".DS_Store") == 0)
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
  "{ite_depth | 0 | Iteration of sphere generation, suggested: 3.}"
  "{objmodel | | Path of the '.obj' file for image rendering. }"
  "{imagedir | ../data/images_all/ | Path of the generated images for one particular .ply model. }"
  "{labeldir | ../data/label_all.txt | Path of the generated images for one particular .ply model. }"
  "{bakgrdir | | Path of the backgroud images sets. }"
  "{center_gen | 0 | Find center from all points. }"
  "{image_size | 227 | Size of captured images. }"
  "{label_class |  | Class label of current .obj model. }"
  "{label_item |  | Item label of current .obj model. }"
  "{rgb_use | 0 | Use RGB image or grayscale. }"
  "{view_region | 0 | Take a special view of front or back angle}"
  "{cam_rot | 0 | Rotating the up direction of camera}";
  /* Get parameters from comand line. */
  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("Generating training data for CNN with triplet loss");
  if (parser.has("help"))
  {
	parser.printMessage();
	return 0;
  }
  int ite_depth = parser.get<int>("ite_depth");
  string objmodel = parser.get<String>("objmodel");
  string imagedir = parser.get<String>("imagedir");
  string labeldir = parser.get<String>("labeldir");
  string bakgrdir = parser.get<string>("bakgrdir");
  int label_class = parser.get<int>("label_class");
  int label_item = parser.get<int>("label_item");
  int center_gen = parser.get<int>("center_gen");
  int image_size = parser.get<int>("image_size");
  int view_region = parser.get<int>("view_region");
  int cam_rot = parser.get<int>("cam_rot");
  double bg_dist;
  cv::Mat cam_up;
  if (cam_rot == 1) {
  	cam_up = (Mat_<int>(3,3) << 0, 1, 0, 0, -1, 0, 0, 0, -1);
  } else {
  	cam_up = (Mat_<int>(1,3) << 0, 1, 0);
  }
  cv::cnn_3dobj::icoSphere ViewSphere(10,ite_depth);
  std::vector<cv::Point3d> campos = ViewSphere.CameraPos;
  // Reader of OBJ file
  std::string filenameOBJ(objmodel);
  vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
  reader->SetFileName(filenameOBJ.data());
  reader->Update();
  // Create a mapper.
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  mapper->ScalarVisibilityOn();
  
  // Create the actor.
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renWin->AddRenderer(ren);
  ren->TexturedBackgroundOn();
  ren->GetActiveCamera()->SetFocalPoint(0,0,0);
  ren->AddActor(actor);
  iren->SetRenderWindow(renWin);
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
  
  char temp[128];
  char* bgname = new char;
  std::vector<String> name_bkg;
  std::fstream imglabel;
  char* p=(char*)labeldir.data();
  imglabel.open(p, fstream::app|fstream::out);
  if (bakgrdir.size() != 0)
  {
	/* List the file names under a given path */
	listDir(bakgrdir.c_str(), name_bkg, false);
	for (unsigned int i = 0; i < name_bkg.size(); i++)
	{
	  name_bkg.at(i) = bakgrdir + name_bkg.at(i);
	}
  }
  /* Images will be saved as .png files. */
  int cnt_img;
  /* Real random related to time */
  //srand((int)time(0));
  double dist_cam_factor=1.8;
  do
  {
	cnt_img = 0;
	for(int pose = 0; pose < static_cast<int>(campos.size()); pose++){
	  int label_x, label_y, label_z;
	  label_x = static_cast<int>(campos.at(pose).x*100);
	  label_y = static_cast<int>(campos.at(pose).y*100);
	  label_z = static_cast<int>(campos.at(pose).z*100);
	  for(int camup = 0; camup < cam_up.rows; camup++){
	  	sprintf (temp,"%08i_%06i_%02i", label_class, label_item, cnt_img);
	  	String filename = temp;
	  	filename += ".png";
	  	imglabel << filename << ' ' << label_class << endl;
	  	filename = imagedir + filename;
	  	imReader->SetFileName(name_bkg.at(rand()%name_bkg.size()).c_str());
	  	ren->GetActiveCamera()->SetPosition(campos.at(pose).x*dist_cam_factor,campos.at(pose).z*dist_cam_factor,campos.at(pose).y*dist_cam_factor);
    	  	ren->GetActiveCamera()->SetViewUp(cam_up.at<int>(camup,0),cam_up.at<int>(camup,1),cam_up.at<int>(camup,2));
	  	wintoimg->Modified();
	  	wintoimg->Update();
	  	writer->SetFileName(filename.c_str());
	  	writer->Write();
	  	cv::Mat grayimg = cv::imread(filename,0);
	  	cv::imwrite(filename,grayimg);
  
	  	iren->Initialize();
	  	cnt_img++;
	  }
	}
  } while (cnt_img != campos.size()*cam_up.rows);
  imglabel.close();
  return 1;
}
