## 소개

<img src="https://pointclouds.org/assets/images/logo.png" width="250">

 Point Cloud Library는 Point Cloud를 처리하는 알고리즘들을 구현해둔 라이브러리입니다.

 라이브러리 소개에 앞서, Point Cloud가 무엇인지 알아봅니다. LiDAR 센서, RGB-D 센서는 물체에 빛이나 신호(Wave)를 보내서 돌아오는 시간을 기록하여 거리 정보를 알아내 하나의 Point를 생성합니다. 그리고 이 포인트들이 3차원 공간에 모여있는 것을 Point Cloud라고 합니다.

Point Cloud는 깊이(z)를 가지고 있기 때문에 N x 3 Numpy 배열로 표현 됩니다. 하지만 센서에서 제공되는 추가 정보가 있을 경우 N x 4 Numpy 배열로 표현 됩니다. 예를 들어 Lidar 센서 데이터에는 반사도가 추가될 수 있으며, RGB-D 센서 데이터에는 색 정보가 추가 될 수 있습니다.

 Point Cloud Library는 Point Cloud의 파일 저장, 읽기, noise 제거, 군집화, 분류 등 여러 기능을 수행하는 알고리즘들을 제공합니다. 특히 다른 3D 오픈소스들과 차별점으로, 코드가 상속으로 구성되어 있어 알고리즘을 쉽게 수정한 후 적용할 수 있다는 점이 있습니다.
 
 Point Cloud Library는 LiDAR를 이용한 자율주행에 제일 중요한 Library입니다. 노이즈 제거 및 군집화를 통해 정제된 데이터는 Object Detection에 활용이 되며, 카메라와 LiDAR가 sensor fusion을 하는 경우 기본 골자가 됩니다. 만약 차량 제어를 하게될 때 Point Cloud의 local과의 거리를 측정하여 사고가 나지 않도록 해줍니다.


## 모듈
-   pcl\_filters : Point Cloud data에서 Error 값과 노이즈 제거 등의 필터들을 제공합니다.
-   pcl\_features : Point Cloud data로부터 3D 특징 추정 (feature estimation) 을 위한 수많은 자료 구조와 방법들을 제공합니다.
-   pcl\_keypoints : Keypoint를 검출하는 알고리즘을 제공합니다. (BRISK, Harris Corner, NARF, SIFT, SUSAN 등)
-   pcl\_registration : 여러 데이터셋을 합쳐 큰 모델로 만드는 registration 작업을 합니다. (ICP 등)
-   pcl\_kdtree : 빠른 최근거리 이웃을 탐색하는 FLANN 라이브러리를 사용한 kdtree 자료 구조를 제공합니다.
-   pcl\_octree : Point Cloud data로부터 계층 트리 구조를 구성하는 효율적인 방법을 제공합니다.
-   pcl\_segmentation : Point Cloud data로부터 개별 클러스터들로 구분하는 알고리즘들을 제공합니다.
-   pcl\_sample\_consensus : 선, 평면, 실린더 등의 모델 계수 추정을 위한 RANSAC 등의 알고리즘들을 제공합니다.
-   pcl\_surface : 3D 표면 복원 기법들을 제공합니다. (meshing, convex hulls, Moving Least Squares 등)
-   pcl\_range\_image : range image나 depth map을 나타내고 처리하는 방법들을 제공합니다.
-   pcl\_io : OpenNI에 호환되는 depth camera로부터 Point Cloud data를 읽고 쓰는 방법을 제공합니다.
-   pcl\_visualization : Point Cloud data를 처리하는 알고리즘의 결과를 시각화하는 방법을 제공합니다.
-   pcl\_common: 다른 PCL Library에서도 상당히 많이 쓰이는 자료 구조나 알고리즘들을 포함합니다.  
-   pcl\_geometry: 컴퓨터를 사용하는 geometry 자료 구조나 알고리즘들을 포함합니다.
-   pcl\_outofcore: 용량이 커서 메인 메모리에 저장할 수 없는 Point Cloud를 위해 8진수 자료 구조를 제공합니다.
-   pcl\_recognition: 개체 인식 응용 프로그램에 사용되는 알고리즘을 포함합니다.
-   pcl\_search: 인접한 값을 검색하는 자료 구조들을 제공합니다. (kdtree, octrees, brute forcd 등)


## 기본적인 사용법

-   #### PCL 선언 및 Point 입력
    

```
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

pcl::PointCloud<pcl::PointXYZ> cloud;
// pcl::PointCloud<pcl::PointXYZI> cloud;
// pcl::PointCloud<pcl::PointXYZRGB> cloud;

// 첫번째, 포인터 좌표를 개별적으로 넣는 방법
pcl::PointXYZ point_xyz;
point_xyz.x = 1;
point_xyz.y = 2;
point_xyz.z = 3

// 두번째, 한줄로 좌표를 넣는 방법
pcl::PointXYZ point_xyz = {1, 2, 3};
```

 PCL을 import하고 원하는 type에 맞춰 선언할 수 있습니다.

 Point도 하나의 객체여서 값을 입력하는 방법이 여러개 있습니다.

-   #### Point 다중 입력
    

 pcl은 std::vector로 구성되어 있기 때문에 사용법이 유사합니다.

```
pcl::PointCloud<pcl::PointXYZ> cloud;

// cloud size를 3으로 설정
cloud.resize(3);

cloud.points[0].x = 1;
cloud.points[0].y = 2;
cloud.points[0].z = 3;

cloud.points[1].x = 4;
cloud.points[1].y = 5;
cloud.points[1].z = 6;

cloud.points[2].x = 7;
cloud.points[2].y = 8;
cloud.points[2].z = 9;
```

 1. PointCloud를 선언한 후 resize()로 공간예약을 합니다.

```
pcl::PointCloud<pcl::PointXYZ> cloud;

// pcl::PointXYZ이라는 type 변수 선언
pcl::PointXYZ point_xyz;

point_xyz.x = 1;
point_xyz.y = 2;
point_xyz.z = 3;
cloud.push_back(point_xyz);

point_xyz.x = 4;
point_xyz.y = 5;
point_xyz.z = 6;
cloud.push_back(point_xyz);

point_xyz.x = 7;
point_xyz.y = 8;
point_xyz.z = 9;
cloud.push_back(point_xyz);
```

 2. 임의의 변수를 선언한 후 push\_back을 사용합니다.

```
pcl::PointCloud<pcl::PointXYZ> cloud;

// 한줄로 쓰는 방법
cloud.push_back(pcl::PointXYZ(1, 2, 3));
cloud.push_back(pcl::PointXYZ(4, 5, 6));
cloud.push_back(pcl::PointXYZ(7, 8, 9));
```

 3. PointXYZ 내부 메소드로 값을 입력합니다.

-   #### Point 출력
    

```
for (int i = 0 ; i < cloud.size(); ++i){
      cout << i << ": ";
      cout << cloud.points[i].x << ", ";
      cout << cloud.points[i].y << ", ";
      cout << cloud.points[i].z << endl;
  }
```

   0: 1, 2, 3  
   1: 4, 5, 6  
   2: 7, 8, 9

 Point 객체의 x, y, z값으로 접근할 수 있습니다. 

-   #### begin()
    

```
cout << "begin(): ";
cout << cloud.begin()->x << ", ";
cout << cloud.begin()->y << ", ";
cout << cloud.begin()->z << endl;
```

   begin(): 1, 2, 3

 Point Cloud의 첫 부분을 가리키는 iterator을 반환합니다. 따라서 주소값을 받아오기 때문에 ->를 사용합니다.

-   #### end()
    

```
cout << "end(): ";
cout << cloud.end()->x << ", ";
cout << cloud.end()->y << ", ";
cout << cloud.end()->z << endl;
```

   end(): 0, 0, 0

 Point Cloud의 마지막 부분을 가리키는 iterator를 반환합니다. 그러나 vector의 end()처럼 마지막 요소의 다음 부분, 메모리 상 값이 할당되지 않은 부분을 가르키기 때문에 0, 0, 0이란 결과 값이 나옵니다.

```
cout << "end() -1: ";
cout << (cloud.end()-1)->x << ", ";
cout << (cloud.end()-1)->y << ", ";
cout << (cloud.end()-1)->z << endl;
```

   end() - 1: 7, 8, 9

 그래서 vector와 마찬가지로 cloud.end()에 -1을 하면 마지막 요소를 가리키는 iterator를 반환합니다.

-   #### front()
    

```
cout << "front(): ";
cout << cloud.front().x << ", ";
cout << cloud.front().y << ", ";
cout << cloud.front().z << endl;
```

   front(): 1, 2, 3

 front()는 첫번째 요소를 반환합니다. begin()과 다른점은 Point값를 반환하는가, 주소값을 반환하는가 입니다.

-   #### back()
    

```
cout << "back(): ";
cout << cloud.back().x << ", ";
cout << cloud.back().y << ", ";
cout << cloud.back().z << endl;
```

   back(): 7, 8, 9

 front()는 마지막 요소를 반환합니다. end()과 다른점은 Point값를 반환하는가, 주소값을 반환하는가 입니다.

-   #### at(int index)
    

```
cout << "at(1): ";
cout << cloud.at(1).x << ", ";
cout << cloud.at(1).y << ", ";
cout << cloud.at(1).z << endl;
```

   at(1): 4, 5, 6

 Point Cloud 상의 index에 해당하는 요소를 반환합니다. cloud.points\[1\]과 같은 값을 냅니다.

-   #### empty()
    

```
if (cloud.empty()){
	cout << "True";
}
else {
	cout << "False";
}
cout << " | size of pc: " << cloud.size() << endl;
```

   False | size of pc: 3

 Point Cloud가 초기화 되어 있으면 True를 반환합니다.

-   #### clear()
    

```
cloud.clear();

if (cloud.empty()){
	cout << "True";
}
else {
	cout << "False";
}
cout << " | size of pc: " << cloud.size() << endl;
```

   True | size of pc: 0

 Point Cloud를 초기화합니다. 초기화 후 Point는 없습니다.

-   #### Point Cloud 병합
    

```
pcl::PointCloud<pcl::PointXYZ> cloud2;
pcl::PointCloud<pcl::PointXYZ> cloud3;

cloud2.push_back(pcl::PointXYZ(1, 2, 3));
cloud2.push_back(pcl::PointXYZ(4, 5, 6));

cloud3.push_back(pcl::PointXYZ(7, 8, 9));
cloud3.push_back(pcl::PointXYZ(10, 11, 12));

cloud2 += cloud3;

cout <<"size: " << cloud2.size() << endl;
for (int i = 0 ; i < cloud2.size(); ++i){
   cout << i << ": ";
   cout << cloud2.points[i].x << ", ";
   cout << cloud2.points[i].y << ", ";
   cout << cloud2.points[i].z << endl;
}
```

   size: 4  
   0: 1, 2, 3  
   1: 4, 5, 6  
   2: 7, 8, 9  
   3: 10, 11, 12

 + 연산으로 Point Cloud를 병합할 수 있습니다. 직관적인 연산은 Point Cloud를 사용할때 아주 유용합니다.

-   #### PCL Pointer
    

 포인터를 사용하는 이유는 2가지 주요한 요인이 있습니다.

 첫번째는 일반적인 상황에서 Point Cloud를 처리할 때 size가 너무 크기 때문입니다. 포인터를 사용하면 자료형과 주소값 하나만 전달해도 모든 요소들을 사용할 수 있습니다.

 두번째는 동적 할당을 통해 메모리를 효율적으로 사용할 수 있기 때문입니다. Point Cloud는 항상 갯수가 바뀌기 때문에 동적 할당이 적합합니다.

```
pcl::PointCloud<pcl::PointXYZ>::Ptr ptr_cloud(new pcl::PointCloud<pcl::PointXYZ>);

ptr_cloud = &cloud2; # Wrong
*ptr_cloud = cloud2; # Right
```

 Point Cloud의 포인터 내부 Ptr 멤버를 실행함으로써 선언할 수 있습니다. 그리고 유의할 점으로 Point Cloud는 boost::shared\_ptr로 구성되어 있기 때문에, 위와 같이 사용해야 합니다.

```
pcl::PointCloud<pcl::PointXYZ>::Ptr ptr_cloud(new pcl::PointCloud<pcl::PointXYZ>);
*ptr_cloud = cloud2;

cout<<"Original: "<<endl;
for (int i = 0 ; i < cloud3.size(); ++i){
	cout << i << ": ";
	cout << cloud3.points[i].x << ", ";
	cout << cloud3.points[i].y << ", ";
	cout << cloud3.points[i].z << endl;
}

cloud3 = *ptr_cloud;

cout << "After: " << endl;
for (int i = 0 ; i < cloud3.size(); ++i){
	cout << i << ": ";
	cout << cloud3.points[i].x << ", ";
	cout << cloud3.points[i].y << ", ";
	cout << cloud3.points[i].z << endl;
}
```

   Original:  
   0: 7, 8, 9  
   1: 10, 11, 12  
   After:  
   0: 1, 2, 3  
   1: 4, 5, 6  
   2: 7, 8, 9  
   3: 10, 11, 12

 위와 같이 포인터를 이용하면 Point Cloud를 다른 Point Cloud로 변경할 수 있습니다. 포인터를 사용하지 않으면 복사 붙여넣기 방식이지만, 포인터는 주소값이 변경이 되기 때문에 cloud2를 변경하면 cloud3도 변경됩니다.

-   #### Point Cloud 형 변환
    

 Point Cloud를 많이 활용하는 환경인 Ros에서는 sensor\_msgs::PointCloud2 메시지를 사용합니다. 그래서 data를 활용할 때 Point Cloud Library를 사용하려면 pcl::PointCloud로 변환시켜야 합니다.

 변환 없이 바로 사용할 수 없는 이유는 sensor\_msgs::PointCloud2는 range data가 unit8 1개로 encoding되어 있고 pcl::PointCloud는 float64 3개로 구성되어 있기 때문입니다. 따라서 활용을 위해서 형 변환을 시켜줘야 합니다.

```
pcl::PointCloud<pcl::PointXYZ> cloudmsg2cloud(sensor_msgs::PointCloud2 cloudmsg)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud_dst;
    pcl::fromROSMsg(cloudmsg, cloud_dst);
    return cloud_dst;
  }
```

 상단의 코드는 sensor\_msgs::PointCloud2를 pcl::PointCloud로 변환하는 방법입니다. Ros메시지로 부터 가져온다는 fromROSMsg를 사용하면 PointCloud2 형에서 PointXYZ 형으로 값이 형 변환 되어 입력됩니다.

```
sensor_msgs::PointCloud2 cloud2cloudmsg(pcl::PointCloud<pcl::PointXYZ> cloud_src)
  {
    sensor_msgs::PointCloud2 cloudmsg;
    pcl::toROSMsg(cloudsrc, cloudmsg);
    cloudmsg.header.frame_id = "map";
    return cloudmsg;
  }
```

 상단의 코드는 pcl::PointCloud를 sensor\_msgs::PointCloud2로 변환하는 방법입니다. Ros메시지로 보낸다는 toROSMsg를 사용하면 PointXYZ 형에서 PointCloud2 형으로 값이 형 변환 되어 입력됩니다.

```
#include <laser_geometry/laser_geometry.h>

sensor_msgs::PointCloud2 laser2cloudmsg(sensor_msgs::LaserScan laser)
    {
      static laser_geometry::LaserProjection projector;
      sensor_msgs::PointCloud2 pc2_dst;
      projector.projectLaser(laser, pc2_dst,-1,laser_geometry::channel_option::Intensity | laser_geometry::channel_option::Distance);
      pc2_dst.header.frame_id = "map";

      return pc2_dst;
    }

```

 2D LiDAR는 Ros에서 sensor\_msgs::PointCloud2가 아닌 sensor\_msgs::LaserScan으로 통신하기 때문에 다른 방식으로 형 변환을 해줍니다. 다른 여러가지 방법이 많지만, 편의상 sensor\_msgs::PointCloud2로 변환하고 난 뒤 pcl::PointCloud로 변환해도 됩니다.


## 모듈 사용 예시

 Point Cloud Library의 모듈은 상당히 많고 그 안에 제공되는 알고리즘들과 자료 구조들은 상당히 많습니다. 그래서 그 중에 Voxelization하는 방법으로 모듈 사용법을 간략히 소개하고자 합니다.

 코드를 분석하기에 앞서 Voxelization에 대해 알아봅시다.

<img src="https://limhyungtae.github.io/img/pcl_centroid.PNG" width="350">

 Voxelization은 Point Cloud를 말 그대로 voxel화 해주는 기능입니다. 위의 그림처럼 여러 개의 Point들이 있을 때 공간을 일정한 크기로 나누어 그 크기 내의 Point들의 평균을 냅니다. 따라서 한 공간에 4개의 Point가 있다면 1개의 Point로 나타내주기 때문에 효율적으로 Point Cloud를 관리할 수 있습니다. Point의 수가 적어지기 때문에 memory를 적게 잡고 다른 후처리를 할 때도 연산 대상이 줄었기 때문에 연산 속도가 상당히 올라갑니다.

```
#include <pcl/filters/voxel_grid.h>

//Input: pcl::PointCloud source, cloud_src
//Output: voxelized pcl::PointCloud, pc_voxelized 

pcl::PointCloud<pcl::PointXYZ> pc_voxelized;
pcl::PointCloud<pcl::PointXYZ>::Ptr ptr_filtered(new pcl::PointCloud<pcl::PointXYZ>);
pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;

double var_voxelsize = 0.05;

*ptr_filtered = cloud_src;
voxel_filter.setInputCloud(ptr_filtered);
voxel_filter.setLeafSize(var_voxelsize, var_voxelsize, var_voxelsize);
voxel_filter.filter(*ptr_filtered);

pc_voxelized = *ptr_filtered;

// Function
void voxelize(pcl::PointCloud<pcl::PointXYZ>::Ptr pc_src, pcl::PointCloud<pcl::PointXYZ>& pc_dst, double var_voxel_size){
  static pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;
  voxel_filter.setInputCloud(pc_src);
  voxel_filter.setLeafSize(var_voxel_size, var_voxel_size, var_voxel_size);
  voxel_filter.filter(pc_dst);
}
```

pcl/filters/voxel\_grid.h를 import합니다. 그리고 Voxelization에 필요한 요소들 (원본 Point Cloud, voxel leaf 크기)를 지정하고, filtering하면 됩니다.

 결과적으로, 모듈의 method들을 활용하여 간편하게 사용할 수 있습니다.


## 출처

 Point Cloud Library 모듈: [pointclouds.org/documentation/modules.html](https://pointclouds.org/documentation/modules.html)

 Point Cloud 소개: [pcl.gitbook.io/tutorial/part-0/part00-chapter01](https://pcl.gitbook.io/tutorial/part-0/part00-chapter01)

 코드 사용법: [limhyungtae.github.io/2019-11-29-ROS-Point-Cloud-Library-(PCL)-1.-Tutorial-%EB%B0%8F-%EA%B8%B0%EB%B3%B8-%EC%82%AC%EC%9A%A9%EB%B2%95/](https://limhyungtae.github.io/2019-11-29-ROS-Point-Cloud-Library-(PCL)-1.-Tutorial-%EB%B0%8F-%EA%B8%B0%EB%B3%B8-%EC%82%AC%EC%9A%A9%EB%B2%95/)
