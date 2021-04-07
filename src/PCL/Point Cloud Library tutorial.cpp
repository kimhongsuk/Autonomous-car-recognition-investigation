// Tistory 블로그에 있는 코드를 모았습니다.
// PCL Package 외 필요한 코드들은 주석처리하였습니다.

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

using namespace std;


// PCL 선언
pcl::PointCloud<pcl::PointXYZ> cloud;
// pcl::PointCloud<pcl::PointXYZI> cloud;
// pcl::PointCloud<pcl::PointXYZRGB> cloud;


// Point 입력
// 첫번째, 포인터 좌표를 개별적으로 넣는 방법
pcl::PointXYZ point_xyz;
point_xyz.x = 1;
point_xyz.y = 2;
point_xyz.z = 3


// Point 입력
// 두번째, 한줄로 좌표를 넣는 방법
pcl::PointXYZ point_xyz = {1, 2, 3};


// Point 다중입력1
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


// Point 다중입력2
// pcl::PointXYZ이라는 type 변수 선언
// pcl::PointXYZ point_xyz;

// point_xyz.x = 1;
// point_xyz.y = 2;
// point_xyz.z = 3;
// cloud.push_back(point_xyz);

// point_xyz.x = 4;
// point_xyz.y = 5;
// point_xyz.z = 6;
// cloud.push_back(point_xyz);

// point_xyz.x = 7;
// point_xyz.y = 8;
// point_xyz.z = 9;
// cloud.push_back(point_xyz);


// Point 다중 입력3
// 한줄로 쓰는 방법
cloud.push_back(pcl::PointXYZ(1, 2, 3));
cloud.push_back(pcl::PointXYZ(4, 5, 6));
cloud.push_back(pcl::PointXYZ(7, 8, 9));


// Point 출력
for (int i = 0 ; i < cloud.size(); ++i){
    cout << i << ": ";
    cout << cloud.points[i].x << ", ";
    cout << cloud.points[i].y << ", ";
    cout << cloud.points[i].z << endl;
}


// begin()
cout << "begin(): ";
cout << cloud.begin()->x << ", ";
cout << cloud.begin()->y << ", ";
cout << cloud.begin()->z << endl;


// end()
cout << "end(): ";
cout << cloud.end()->x << ", ";
cout << cloud.end()->y << ", ";
cout << cloud.end()->z << endl;


// end() 보정
cout << "end() -1: ";
cout << (cloud.end()-1)->x << ", ";
cout << (cloud.end()-1)->y << ", ";
cout << (cloud.end()-1)->z << endl;


// front()
cout << "front(): ";
cout << cloud.front().x << ", ";
cout << cloud.front().y << ", ";
cout << cloud.front().z << endl;


// back()
cout << "back(): ";
cout << cloud.back().x << ", ";
cout << cloud.back().y << ", ";
cout << cloud.back().z << endl;


// at(int index)
cout << "at(1): ";
cout << cloud.at(1).x << ", ";
cout << cloud.at(1).y << ", ";
cout << cloud.at(1).z << endl;


// empty()
if (cloud.empty()){
	cout << "True";
}
else {
	cout << "False";
}
cout << " | size of pc: " << cloud.size() << endl;


// clear()
cloud.clear();

if (cloud.empty()){
	cout << "True";
}
else {
	cout << "False";
}
cout << " | size of pc: " << cloud.size() << endl;


// Point Cloud 병합
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



// PCL Pointer
pcl::PointCloud<pcl::PointXYZ>::Ptr ptr_cloud(new pcl::PointCloud<pcl::PointXYZ>);

// ptr_cloud = &cloud2; # Wrong
*ptr_cloud = cloud2; # Right

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



// Point Cloud 형 변환 (need Ros msg)
// cloud.msg to Point Cloud
//pcl::PointCloud<pcl::PointXYZ> cloudmsg2cloud(sensor_msgs::PointCloud2 cloudmsg){
//    pcl::PointCloud<pcl::PointXYZ> cloud_dst;
//    pcl::fromROSMsg(cloudmsg, cloud_dst);
//    return cloud_dst;
//}
//
//
// Point Cloud to cloud.msg
//sensor_msgs::PointCloud2 cloud2cloudmsg(pcl::PointCloud<pcl::PointXYZ> cloud_src)
//{
//    sensor_msgs::PointCloud2 cloudmsg;
//    pcl::toROSMsg(cloudsrc, cloudmsg);
//    cloudmsg.header.frame_id = "map";
//    return cloudmsg;
//}
//
//
// laser.msg to Point Cloud
//sensor_msgs::PointCloud2 laser2cloudmsg(sensor_msgs::LaserScan laser)
//{
//    static laser_geometry::LaserProjection projector;
//    sensor_msgs::PointCloud2 pc2_dst;
//    projector.projectLaser(laser, pc2_dst,-1,laser_geometry::channel_option::Intensity | laser_geometry::channel_option::Distance);
//    pc2_dst.header.frame_id = "map";
//
//    return pc2_dst;
//}
//
//
// Voxelization (Need Points)
//#include <pcl/filters/voxel_grid.h>
//
//Input: pcl::PointCloud source, cloud_src
//Output: voxelized pcl::PointCloud, pc_voxelized 
//
//pcl::PointCloud<pcl::PointXYZ> pc_voxelized;
//pcl::PointCloud<pcl::PointXYZ>::Ptr ptr_filtered(new pcl::PointCloud<pcl::PointXYZ>);
//pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;
//
//double var_voxelsize = 0.05;
//
//*ptr_filtered = cloud_src;
//voxel_filter.setInputCloud(ptr_filtered);
//voxel_filter.setLeafSize(var_voxelsize, var_voxelsize, var_voxelsize);
//voxel_filter.filter(*ptr_filtered);
//
//pc_voxelized = *ptr_filtered;
//
// //Function
//void voxelize(pcl::PointCloud<pcl::PointXYZ>::Ptr pc_src, pcl::PointCloud<pcl::PointXYZ>& pc_dst, double var_voxel_size){
//  static pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;
//  voxel_filter.setInputCloud(pc_src);
// voxel_filter.setLeafSize(var_voxel_size, var_voxel_size, var_voxel_size);
//  voxel_filter.filter(pc_dst);
//}