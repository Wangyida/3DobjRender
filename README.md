# Model rendering and annotation tools

This is a code pool of Yida Wang, master student of Dr. Weihong Deng. Those codes are used for model rendering from 3D models to 2D synthetic images together with some specific annotation and contour generation. It's basically written in c++, but there are also some basic scripts for API.

Compiling is based on cmake and make, it's designed for Linux, MacOS and Unix.

# Author information

## Yida Wang

**PhD candidate** with Dr. [Federico Tombari](http://campar.in.tum.de/Main/FedericoTombari) in [Technische Universität München](https://www.tum.de/)

**M.Eng** with Dr. [Weihong Deng](http://www.whdeng.cn/) in [Beijing University of Posts and Telecommunications](http://english.bupt.edu.cn/)

Email Address: yidawang.cn@gmail.com, wangyida1@bupt.edu.cn

[ResearchGate](https://www.researchgate.net/profile/Yida_Wang), [Github](https://github.com/wangyida), [GSoC 2016](https://summerofcode.withgoogle.com/archive/2016/projects/4623962327744512/), [GSoC 2015](https://www.google-melange.com/archive/gsoc/2015/orgs/opencv/projects/wangyida.html)

## Publications

1. [ZigzagNet: Efficient Deep Learning for Real Object Recognition Based on 3D Models](https://www.researchgate.net/profile/Yida_Wang/publications?sorting=recentlyAdded) (ACCV 2016)

+ [Self-restraint Object Recognition by Model Based CNN Learning](http://ieeexplore.ieee.org/document/7532438/) (ICIP 2016)

+ [Face Recognition Using Local PCA Filters](http://link.springer.com/chapter/10.1007%2F978-3-319-25417-3_5) (CCBR 2015)

+ [CNTK on Mac: 2D Object Restoration and Recognition Based on 3D Model](https://www.microsoft.com/en-us/research/academic-program/microsoft-open-source-challenge/) (Microsoft Open Source Challenge 2016)

+ [Large-Scale 3D Shape Retrieval from ShapeNet Core55](https://shapenet.cs.stanford.edu/shrec16/shrec16shapenet.pdf) (EG 2016 workshop: 3D OR)

# Code samples

## Details for camera position
Regular objects on the ground using a semisphere view system
```cpp
if (semisphere == 1)
{
    for (int pose = 0; pose < static_cast<int>(campos_temp.size()); pose++)
    {
      if (campos_temp.at(pose).z >= -0.3 && campos_temp.at(pose).z < z_range)
        campos.push_back(campos_temp.at(pose));
    }
}
```
Special object such as plane using a full space of view sphere
```cpp
else
{
  for (int pose = 0; pose < static_cast<int>(campos_temp.size()); pose++)
  {
  if (campos_temp.at(pose).z < 0.3 && campos_temp.at(pose).z > -0.8)
  campos.push_back(campos_temp.at(pose));
  }
}
```
## Model file searching
List the file names under a given path
```cpp
listDir(bakgrdir_p.c_str(), name_bkg_p, false);
for (unsigned int i = 0; i < name_bkg_p.size(); i++)
{
  name_bkg_p.at(i) = bakgrdir_p + name_bkg_p.at(i);
}
```
## Start rendering
Step 1: Reader is the tool to load the poly files
```cpp
vtkSmartPointer<vtkOBJReader> reader =
  vtkSmartPointer<vtkOBJReader>::New();
reader->SetFileName(objmodel.c_str());
reader->Update();
```

Step 2: Build visualization enviroment:
Mapper loads what reader recorded
```cpp
vtkSmartPointer<vtkPolyDataMapper> ObjectMapper =
  vtkSmartPointer<vtkPolyDataMapper>::New();
ObjectMapper->SetInputConnection(reader->GetOutputPort());
```

Step 3: Actor set target on Mapper,
this is what we use for further operation on mesh
```cpp
vtkSmartPointer<vtkActor> actor =
  vtkSmartPointer<vtkActor>::New();
actor->SetMapper(ObjectMapper);
```

Step 4: Start the main task for rendering
Step 4(1): Render operates on -> Actor
```cpp
vtkSmartPointer<vtkRenderer> ren =
  vtkSmartPointer<vtkRenderer>::New();
ren->AddActor(actor);
ren->TexturedBackgroundOn();
ren->GetActiveCamera()->SetViewUp(0,1,0);

```

Step 4(2): RenderWindow operates on -> Render
```cpp
vtkSmartPointer<vtkRenderWindow> renWin =
  vtkSmartPointer<vtkRenderWindow>::New();
renWin->AddRenderer(ren);
renWin->SetSize(227,227);
```

Step 4(3): Interactor operates  on -> RenderWindow
```cpp
vtkSmartPointer<vtkRenderWindowInteractor> iren =
  vtkSmartPointer<vtkRenderWindowInteractor>::New();
iren->SetRenderWindow(renWin);
renWin->Render();
```

Step 5: Define details for rendering
```cpp
vtkSmartPointer<vtkJPEGReader> imReader =
  vtkSmartPointer<vtkJPEGReader>::New();
vtkSmartPointer<vtkTexture> atext =
  vtkSmartPointer<vtkTexture>::New();
atext->SetInputConnection(imReader->GetOutputPort());
atext->InterpolateOn();
ren->SetBackground(0,0,0);
ren->SetBackgroundTexture(atext);
```
