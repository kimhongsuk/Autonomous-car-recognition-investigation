## 소개

 <img src="https://waymo.com/static/images/header/logo.svg" width="150px">

 WAYMO Opensource Dataset은 2019년 8월부터 시작된 연구 목적의 비상업용 개방형 오픈데이터로 Motion Dataset과 Perception Dataset으로 나누어 제공되는데, 인지와 관련된 데이터는 Perception Dataset입니다.

 Perception Dataset은 WAYMO 자율주행 차량이 1950개 주행 구간에서 수집한 지역별, 시간별, 날씨별 데이터를 포함하고 있습니다. (2021년 3월 기준)

 각 구간은 10Hz, 20초의 연속주행 데이터를 담고 있으며, 웨이모가 무인 자율주행 택시 시범 서비스를 진행하고 있는 미국 애리조나주 피닉스의 운행 데이터도 포함돼 있습니다.

 데이터는 5개의 라이다(LiDAR)와 5개의 전·측면 표준 카메라로 수집됐으며, 360도 고해상도 캡처 뷰와 차량, 보행자, 자전거, 간판을 포함한 모든 물체 데이터도 함께 제공됩니다.

 WAYMO Opensource Dataset은 인지와 자율주행 기술 등을 발전시키는 용도로 사용하며, 알고리즘등을 겨루는 공간도 있습니다.


## 데이터 수집 센서

 자동차에 설치된 센서들은 다음과 같이 있습니다.

-   중거리 LiDAR 1개
    -   최대 75m를 봅니다.

-   단거리 LiDAR 4개
    -   최대 25m를 봅니다.

-   전, 측면 카메라 5개


## 데이터 주석

 대부분의 주석 처리는 2D 이미지와 3D 이미지 모두 같은 조건을 가집니다. 단, 2D 이미지로 보았을 때 물체 인식이 애매하면 2D 이미지에서 주석 처리를 생략합니다.

####  Vehicle Labeling Specifications

-   차량은 LiDAR 데이터나 카메라 이미지로 검출되면 만들어지며, 자전거나 오토바이의 운전자도 이동수단 주석 처리 대상에 포함됩니다. 단, 열차나 트램은 주석 처리 대상에 포함되지 않습니다.
-   자동차의 사이드미러는 포함되나 안테나 같은 엄청 작은 물체는 제외됩니다.
-   트레일러는 별도의 이동수단으로 표시되며 포크레인처럼 움직이는 부분도 별도로 표시됩니다.

 <img src="https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/vehicle-3D-labeling-example.png" width="500px">

![Vehicle Labeling, 2D Image](https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/vehicle-2D-labeling-example.png)

####  Pedestrian Labeling Specifications

-   차량에 탑승해 있는 경우를 제외한 모든 사람들은 Pedestrian으로 주석 처리합니다. 단, 킥보드 같은 개인 운송 장치에 탄 사람은 Pedestrian으로 포함됩니다.
-   사람이 아닌 마네킹이나 더미, 포스터 속 인물들은 주석 처리 대상이 아닙니다.
-   겹쳐있는 사람은 분리 인식하며 2m 이상의 물체를 가진 사람을 제외하고는 들고 있는 물건도 보행자로 인식합니다.
-   유모차도 보행자로 인식하나 child로 인식되지 않습니다.

 <img src="https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/pedestrian-3D-labeling-example.png" width="500px">

![Pedestrian Labeling, 2D Image](https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/pedestrian-2D-labeling-example.png)

####  Cyclist Labeling

-   2륜, 3륜 등 자전거로 분류되는 모든 것들을 타고있는 사람들을 포함합니다. 단, 자전거에 오를 때까지는 보행자로 인식합니다.
-   Cyclist가 주석 대상이기 때문에 자전거만 있는 경우는 포함하지 않습니다.

 <img src="https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/cyclist-3D-labeling-example.png" width="500px">

![Cyclist Labeling, 2D Image](https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/cyclist-2D-labeling-example.png)

####  Sign Labeling Specifications

-   2D 이미지 없이 3D bound box로만 인식합니다.
-   교통 흐름에 영향을 주는 표지판만 인식합니다. 따라서 사람이 임의로 쓴 표지판이나 길의 이름을 쓴 표지판들은 포함되지 않습니다.
-   그림이 그려진 부분을 앞쪽으로 인식합니다.

 <img src="https://github.com/waymo-research/waymo-open-dataset/raw/master/docs/images/sign-3D-labeling-example.png" width="500px">


## 데이터 포맷

 데이터들은 각각 Frame 메시지에 담겨있습니다.

 Frame 내부 메시지들도 의존성을 갖기 때문에 기본 message부터 Frame까지 설명합니다. 단, 설명 위주의 코드인 이유로 삽입되는 값들은 자료형이 전혀 맞지 않음을 강조합니다.

```
message Label {
  // Upright box, zero pitch and roll.
  message Box {
    // Box coordinates in vehicle frame.
    optional double center_x = 1;
    optional double center_y = 2;
    optional double center_z = 3;

    // Dimensions of the box. length: dim x. width: dim y. height: dim z.
    optional double length = 5;
    optional double width = 4;
    optional double height = 6;

    // The heading of the bounding box (in radians).  The heading is the angle
    // required to rotate +x to the surface normal of the box front face. It is
    // normalized to [-pi, pi).
    optional double heading = 7;

    enum Type {
      TYPE_UNKNOWN = 0;
      // 7-DOF 3D (a.k.a upright 3D box).
      TYPE_3D = 1;
      // 5-DOF 2D. Mostly used for laser top down representation.
      TYPE_2D = 2;
      // Axis aligned 2D. Mostly used for image.
      TYPE_AA_2D = 3;
    }
  }
  
  optional Box box = 1;

  message Metadata {
    optional double speed_x = 1;
    optional double speed_y = 2;
    optional double accel_x = 3;
    optional double accel_y = 4;
  }
  
  optional Metadata metadata = 2;

  enum Type {
    TYPE_UNKNOWN = 0;
    TYPE_VEHICLE = 1;
    TYPE_PEDESTRIAN = 2;
    TYPE_SIGN = 3;
    TYPE_CYCLIST = 4;
  }
  
  optional Type type = 3;
  // Object ID.
  optional string id = 4;

  // The difficulty level of this label. The higher the level, the harder it is.
  enum DifficultyLevel {
    UNKNOWN = 0;
    LEVEL_1 = 1;
    LEVEL_2 = 2;
  }
  
  // Difficulty level for detection problem.
  optional DifficultyLevel detection_difficulty_level = 5;
  // Difficulty level for tracking problem.
  optional DifficultyLevel tracking_difficulty_level = 6;

  // The total number of lidar points in this box.
  optional int32 num_lidar_points_in_box = 7;

}
```

 Label은 주석 처리된 데이터들을 나타내는 메시지 타입입니다. 이 안에 박스의 위치, 박스의 타입, 물체의 타입 등을 포함하고 있습니다. 또한 lidar 포인트 개수, 인지 난이도 등이 있습니다.

```
message MatrixShape {
  // Dimensions for the Matrix messages defined below. Must not be empty.
  //
  // The order of entries in 'dims' matters, as it indicates the layout of the
  // values in the tensor in-memory representation.
  //
  // The first entry in 'dims' is the outermost dimension used to lay out the
  // values; the last entry is the innermost dimension.  This matches the
  // in-memory layout of row-major matrices.
  repeated int32 dims = 1;
}
```

 MatrixShape는 차원의 모양을 선언합니다. 왜냐하면 차원이 커질수록 필요한 변수의 개수가 늘어나기 때문입니다.

```
// Row-major matrix.
// Requires: data.size() = product(shape.dims()).
message MatrixFloat {
  repeated float data = 1 [packed = true];
  optional MatrixShape shape = 2;
}
```

 MatrixFloat는 차원의 모양을 실수로 선언합니다.

```
// Row-major matrix.
// Requires: data.size() = product(shape.dims()).
message MatrixInt32 {
  repeated int32 data = 1 [packed = true];
  optional MatrixShape shape = 2;
}
```

 MatrixInt32는 차원의 모양을 정수로 선언합니다.

```
message CameraName {
  enum Name {
    UNKNOWN = 0;
    FRONT = 1;
    FRONT_LEFT = 2;
    FRONT_RIGHT = 3;
    SIDE_LEFT = 4;
    SIDE_RIGHT = 5;
  }
}
```

 CameraName은 차량에 장착된 카메라를 지칭합니다.

```
// 'Laser' is used interchangeably with 'Lidar' in this file.
message LaserName {
  enum Name {
    UNKNOWN = 0;
    TOP = 1;
    FRONT = 2;
    SIDE_LEFT = 3;
    SIDE_RIGHT = 4;
    REAR = 5;
  }
}
```

 LaserName은 차량에 장착된 라이다를 지칭합니다.

```
// 4x4 row major transform matrix that tranforms 3d points from one frame to
// another.
message Transform {
  repeated double transform = 1;
}
```

 Transform은 3D points를 한 프레임에서 다른 프레임으로 변환할때 쓰인 변주 행렬입니다.

```
message Velocity {
  // Velocity in m/s.
  optional float v_x = 1;
  optional float v_y = 2;
  optional float v_z = 3;

  // Angular velocity in rad/s.
  optional double w_x = 4;
  optional double w_y = 5;
  optional double w_z = 6;
}
```

 Velocity는 물체의 속도를 나타냅니다.

```
message CameraCalibration {
  optional CameraName.Name name = 1;
  // 1d Array of [f_u, f_v, c_u, c_v, k{1, 2}, p{1, 2}, k{3}].
  // Note that this intrinsic corresponds to the images after scaling.
  // Camera model: pinhole camera.
  // Lens distortion:
  //   Radial distortion coefficients: k1, k2, k3.
  //   Tangential distortion coefficients: p1, p2.
  // k_{1, 2, 3}, p_{1, 2} follows the same definition as OpenCV.
  // https://en.wikipedia.org/wiki/Distortion_(optics)
  // https://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
  repeated double intrinsic = 2;
  // Camera frame to vehicle frame.
  optional Transform extrinsic = 3;
  // Camera image size.
  optional int32 width = 4;
  optional int32 height = 5;

  enum RollingShutterReadOutDirection {
    UNKNOWN = 0;
    TOP_TO_BOTTOM = 1;
    LEFT_TO_RIGHT = 2;
    BOTTOM_TO_TOP = 3;
    RIGHT_TO_LEFT = 4;
    GLOBAL_SHUTTER = 5;
  }
  optional RollingShutterReadOutDirection rolling_shutter_direction = 6;
}
```

 CameraCalibration은 카메라를 calibration 하면서 카메라의 image size와 내부 설정 등을 보여줍니다.

```
message LaserCalibration {
  optional LaserName.Name name = 1;
  // If non-empty, the beam pitch (in radians) is non-uniform. When constructing
  // a range image, this mapping is used to map from beam pitch to range image
  // row.  If this is empty, we assume a uniform distribution.
  repeated double beam_inclinations = 2;
  // beam_inclination_{min,max} (in radians) are used to determine the mapping.
  optional double beam_inclination_min = 3;
  optional double beam_inclination_max = 4;
  // Lidar frame to vehicle frame.
  optional Transform extrinsic = 5;

}
```

 LaserCalibration은 LiDAR를 calibration한 값을 보여줍니다.

```
message Context {
  // A unique name that identifies the frame sequence.
  optional string name = 1;
  repeated CameraCalibration camera_calibrations = 2;
  repeated LaserCalibration laser_calibrations = 3;
  // Some stats for the run segment used.
  message Stats {
    message ObjectCount {
      optional Label.Type type = 1;
      // The number of unique objects with the type in the segment.
      optional int32 count = 2;
    }
    repeated ObjectCount laser_object_counts = 1;
    repeated ObjectCount camera_object_counts = 5;
    // Day, Dawn/Dusk, or Night, determined from sun elevation.
    optional string time_of_day = 2;
    // Human readable location (e.g. CHD, SF) of the run segment.
    optional string location = 3;
    // Currently either Sunny or Rain.
    optional string weather = 4;
  }
  optional Stats stats = 4;
}
```

 Context는 해당 Frame이 어떤 이름을 갖고 있는지, 당시 카메라와 LiDAR를 Calibration한 값, 주석 처리한 물체의 개수, 당시 날씨와 위치 등을 나타냅니다.

```
message RangeImage {
  // Zlib compressed [H, W, 4] serialized version of MatrixFloat.
  // To decompress:
  // string val = ZlibDecompress(range_image_compressed);
  // MatrixFloat range_image;
  // range_image.ParseFromString(val);
  // Inner dimensions are:
  //   * channel 0: range
  //   * channel 1: intensity
  //   * channel 2: elongation
  //   * channel 3: is in any no label zone.
  optional bytes range_image_compressed = 2;

  // Lidar point to camera image projections. A point can be projected to
  // multiple camera images. We pick the first two at the following order:
  // [FRONT, FRONT_LEFT, FRONT_RIGHT, SIDE_LEFT, SIDE_RIGHT].
  //
  // Zlib compressed [H, W, 6] serialized version of MatrixInt32.
  // To decompress:
  // string val = ZlibDecompress(camera_projection_compressed);
  // MatrixInt32 camera_projection;
  // camera_projection.ParseFromString(val);
  // Inner dimensions are:
  //   * channel 0: CameraName.Name of 1st projection. Set to UNKNOWN if no
  //       projection.
  //   * channel 1: x (axis along image width)
  //   * channel 2: y (axis along image height)
  //   * channel 3: CameraName.Name of 2nd projection. Set to UNKNOWN if no
  //       projection.
  //   * channel 4: x (axis along image width)
  //   * channel 5: y (axis along image height)
  // Note: pixel 0 corresponds to the left edge of the first pixel in the image.
  optional bytes camera_projection_compressed = 3;

  // Zlib compressed [H, W, 6] serialized version of MatrixFloat.
  // To decompress:
  // string val = ZlibDecompress(range_image_pose_compressed);
  // MatrixFloat range_image_pose;
  // range_image_pose.ParseFromString(val);
  // Inner dimensions are [roll, pitch, yaw, x, y, z] represents a transform
  // from vehicle frame to global frame for every range image pixel.
  // This is ONLY populated for the first return. The second return is assumed
  // to have exactly the same range_image_pose_compressed.
  //
  // The roll, pitch and yaw are specified as 3-2-1 Euler angle rotations,
  // meaning that rotating from the navigation to vehicle frame consists of a
  // yaw, then pitch and finally roll rotation about the z, y and x axes
  // respectively. All rotations use the right hand rule and are positive
  // in the counter clockwise direction.
  optional bytes range_image_pose_compressed = 4;

  // Zlib compressed [H, W, 5] serialized version of MatrixFloat.
  // To decompress:
  // string val = ZlibDecompress(range_image_flow_compressed);
  // MatrixFloat range_image_flow;
  // range_image_flow.ParseFromString(val);
  // Inner dimensions are [vx, vy, vz, pointwise class].
  //
  // If the point is not annotated with scene flow information, class is set
  // to -1. A point is not annotated if it is in a no-label zone or if its label
  // bounding box does not have a corresponding match in the previous frame,
  // making it infeasible to estimate the motion of the point.
  // Otherwise, (vx, vy, vz) are velocity along (x, y, z)-axis for this point
  // and class is set to one of the following values:
  //  -1: no-flow-label, the point has no flow information.
  //   0:  unlabeled or "background,", i.e., the point is not contained in a
  //       bounding box.
  //   1: vehicle, i.e., the point corresponds to a vehicle label box.
  //   2: pedestrian, i.e., the point corresponds to a pedestrian label box.
  //   3: sign, i.e., the point corresponds to a sign label box.
  //   4: cyclist, i.e., the point corresponds to a cyclist label box.
  optional bytes range_image_flow_compressed = 5;

  // Deprecated, do not use.
  optional MatrixFloat range_image = 1 [deprecated = true];
}
```

 RangeImage는 사진이 나타내는 특성을 가리킵니다. 각 특성에 맞춰 채널을 맞추면 원하는 데이터를 얻을 수 있습니다.

```
// All timestamps in this proto are represented as seconds since Unix epoch.
message CameraImage {
  optional CameraName.Name name = 1;
  // JPEG image.
  optional bytes image = 2;
  // SDC pose.
  optional Transform pose = 3;
  // SDC velocity at 'pose_timestamp' below. The velocity value is represented
  // at *global* frame.
  // With this velocity, the pose can be extrapolated.
  // r(t+dt) = r(t) + dr/dt * dt where dr/dt = v_{x,y,z}.
  // dR(t)/dt = W*R(t) where W = SkewSymmetric(w_{x,y,z})
  // This differential equation solves to: R(t) = exp(Wt)*R(0) if W is constant.
  // When dt is small: R(t+dt) = (I+W*dt)R(t)
  // r(t) = (x(t), y(t), z(t)) is vehicle location at t in the global frame.
  // R(t) = Rotation Matrix (3x3) from the body frame to the global frame at t.
  // SkewSymmetric(x,y,z) is defined as the cross-product matrix in the
  // following:
  // https://en.wikipedia.org/wiki/Cross_product#Conversion_to_matrix_multiplication
  optional Velocity velocity = 4;
  // Timestamp of the `pose` above.
  optional double pose_timestamp = 5;

  // Rolling shutter params.
  // The following explanation assumes left->right rolling shutter.
  //
  // Rolling shutter cameras expose and read the image column by column, offset
  // by the read out time for each column. The desired timestamp for each column
  // is the middle of the exposure of that column as outlined below for an image
  // with 3 columns:
  // ------time------>
  // |---- exposure col 1----| read |
  // -------|---- exposure col 2----| read |
  // --------------|---- exposure col 3----| read |
  // ^trigger time                                ^readout end time
  //             ^time for row 1 (= middle of exposure of row 1)
  //                    ^time image center (= middle of exposure of middle row)
  // Shutter duration in seconds. Exposure time per column.
  optional double shutter = 6;
  // Time when the sensor was triggered and when last readout finished.
  // The difference between trigger time and readout done time includes
  // the exposure time and the actual sensor readout time.
  optional double camera_trigger_time = 7;
  optional double camera_readout_done_time = 8;
}
```

 CameraImage에는 사진의 찍힌 시간과 환경, 찍었을 때의 카메라 조작값 등을 나타냅니다. 

```
// The camera labels associated with a given camera image. This message
// indicates the ground truth information for the camera image
// recorded by the given camera. If there are no labeled objects in the image,
// then the labels field is empty.
message CameraLabels {
  optional CameraName.Name name = 1;
  repeated Label labels = 2;
}
```

 CameraLabels는 어떤 카메라에서 어떤 주석이 있는지 나타냅니다.

```
message Laser {
  optional LaserName.Name name = 1;
  optional RangeImage ri_return1 = 2;
  optional RangeImage ri_return2 = 3;
}
```

 Laser는 어떤 Lidar에서 어떤 RangeImage가 있는지 나타냅니다.

```
message Frame {
  // The following field numbers are reserved for third-party extensions. Users
  // may declare new fields in that range in their own .proto files without
  // having to edit the original file.
  extensions 1000 to max;

  // This context is the same for all frames belong to the same driving run
  // segment. Use context.name to identify frames belong to the same driving
  // segment. We do not store all frames from one driving segment in one proto
  // to avoid huge protos.
  optional Context context = 1;

  // Frame start time, which is the timestamp of the first top lidar spin
  // within this frame.
  optional int64 timestamp_micros = 2;
  // The vehicle pose.
  optional Transform pose = 3;
  repeated CameraImage images = 4;
  repeated Laser lasers = 5;
  repeated Label laser_labels = 6;
  // Lidar labels (laser_labels) projected to camera images. A projected
  // label is the smallest image axis aligned rectangle that can cover all
  // projected points from the 3d lidar label. The projected label is ignored if
  // the projection is fully outside a camera image. The projected label is
  // clamped to the camera image if it is partially outside.
  repeated CameraLabels projected_lidar_labels = 9;
  // NOTE: if a camera identified by CameraLabels.name has an entry in this
  // field, then it has been labeled, even though it is possible that there are
  // no labeled objects in the corresponding image, which is identified by a
  // zero sized CameraLabels.labels.
  repeated CameraLabels camera_labels = 8;
  // No label zones in the *global* frame.
  repeated Polygon2dProto no_label_zones = 7;
}
```

 Frame은 위에서 나온 모든 값들을 조합하여 하나의 Frame으로 나타냅니다. 각각의 값은 key값으로 나타납니다.


## 데이터 예시

####  waymo\_open\_dataset package 설치

```
!rm -rf waymo-od > /dev/null
!git clone https://github.com/waymo-research/waymo-open-dataset.git waymo-od
!cd waymo-od && git branch -a
!cd waymo-od && git checkout remotes/origin/master
!pip3 install --upgrade pip
!pip3 install waymo-open-dataset-tf-2-1-0==1.2.0
```

 Linux 환경에서 위와 같이 입력하면 waymo\_open\_dataset package가 설치됩니다.

####  작업 환경 구축

```
import os
import tensorflow.compat.v1 as tf
import math
import numpy as np
import itertools

tf.enable_eager_execution()

from waymo_open_dataset.utils import range_image_utils
from waymo_open_dataset.utils import transform_utils
from waymo_open_dataset.utils import  frame_utils
from waymo_open_dataset import dataset_pb2 as open_dataset
```

 Tutorial에서는 tensorflow를 이용함으로 tensorflow를 import 했으며, 데이터셋을 이용하는 도구도 import 합니다.

####  프레임 1개 읽어오기

```
FILENAME = '/content/waymo-od/tutorial/frames'
dataset = tf.data.TFRecordDataset(FILENAME, compression_type='')

for data in dataset:
    frame = open_dataset.Frame()
    frame.ParseFromString(bytearray(data.numpy()))
    break

(range_images, camera_projections, range_image_top_pose) = frame_utils.parse_range_image_and_camera_projection(frame)

print(frame.context)
```

 Dataset에서 원하는 프레임 하나를 가져온 후, 사용할 수 있도록 parsing하는 작업을 합니다.

```
name: "11004685739714500220_2300_000_2320_000"
camera_calibrations {
  name: FRONT
  intrinsic: 2055.556149361639
  intrinsic: 2055.556149361639
  intrinsic: 939.6574698861468
  intrinsic: 641.0721821943271
  intrinsic: 0.03231600849798887
  intrinsic: -0.3214124825527059
  intrinsic: 0.0007932583953709973
  intrinsic: -0.0006257493541333847
  intrinsic: 0.0
  extrinsic {
    transform: 0.9998926849887427
    transform: -0.005993208400016058
    transform: 0.0133678704017097
    transform: 1.5389142447125008
    transform: 0.006042236521329663
    transform: 0.9999751560547995
    transform: -0.003630241176497072
    transform: -0.02363394083934774
    transform: -0.013345781499156929
    transform: 0.003710623431877962
    transform: 0.999904056092345
    transform: 2.115270572975561
    transform: 0.0
    transform: 0.0
    transform: 0.0
    transform: 1.0
  }
  width: 1920
  height: 1280
  rolling_shutter_direction: LEFT_TO_RIGHT
}
laser_calibrations {
  name: FRONT
  beam_inclination_min: -1.5707963267948966
  beam_inclination_max: 0.5235987755982988
  extrinsic {
    transform: 0.9998652264631824
    transform: -0.012374982714412487
    transform: 0.01078836004899645
    transform: 4.07
    transform: 0.012370123575608356
    transform: 0.9999233534288761
    transform: 0.0005170205750618116
    transform: 0.0
    transform: -0.010793931278870031
    transform: -0.0003834975473860608
    transform: 0.9999416702874113
    transform: 0.689
    transform: 0.0
    transform: 0.0
    transform: 0.0
    transform: 1.0
  }
}
stats {
  laser_object_counts {
    type: TYPE_VEHICLE
    count: 13
  }
  laser_object_counts {
    type: TYPE_PEDESTRIAN
    count: 21
  }
  laser_object_counts {
    type: TYPE_SIGN
    count: 10
  }
  time_of_day: "Day"
  location: "location_sf"
  weather: "sunny"
  camera_object_counts {
    type: TYPE_VEHICLE
    count: 9
  }
  camera_object_counts {
    type: TYPE_PEDESTRIAN
    count: 10
  }
}
```

 frame.context는 데이터 포멧에 맞춰 name, camera\_calibrations, laser\_calibrations, stats가 위와 같은 결과로 나옵니다. (카메라와 LiDAR의 일부 결과값은 형태가 중복이고 너무 길어서 생략했습니다.)


####  카메라 이미지와 주석 확인

```
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def show_camera_image(camera_image, camera_labels, layout, cmap=None):
  """Show a camera image and the given camera labels."""

  ax = plt.subplot(*layout)

  # Draw the camera labels.
  for camera_labels in frame.camera_labels:
    # Ignore camera labels that do not correspond to this camera.
    if camera_labels.name != camera_image.name:
      continue

    # Iterate over the individual labels.
    for label in camera_labels.labels:
      # Draw the object bounding box.
      ax.add_patch(patches.Rectangle(
        xy=(label.box.center_x - 0.5 * label.box.length,
            label.box.center_y - 0.5 * label.box.width),
        width=label.box.length,
        height=label.box.width,
        linewidth=1,
        edgecolor='red',
        facecolor='none'))

  # Show the camera image.
  plt.imshow(tf.image.decode_jpeg(camera_image.image), cmap=cmap)
  plt.title(open_dataset.CameraName.Name.Name(camera_image.name))
  plt.grid(False)
  plt.axis('off')

plt.figure(figsize=(25, 20))

for index, image in enumerate(frame.images):
  show_camera_image(image, frame.camera_labels, [3, 3, index+1])
```

 matplotlib을 활용하여 카메라 이미지를 띄우고, 라벨 덮어씌웁니다.

<img src="https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/WAYMO/Camera_Images_and_Labels.png" width="650">


####  라이다 이미지(Range Images) 확인

```
plt.figure(figsize=(64, 20))
def plot_range_image_helper(data, name, layout, vmin = 0, vmax=1, cmap='gray'):
  """Plots range image.

  Args:
    data: range image data
    name: the image title
    layout: plt layout
    vmin: minimum value of the passed data
    vmax: maximum value of the passed data
    cmap: color map
  """
  plt.subplot(*layout)
  plt.imshow(data, cmap=cmap, vmin=vmin, vmax=vmax)
  plt.title(name)
  plt.grid(False)
  plt.axis('off')

def get_range_image(laser_name, return_index):
  """Returns range image given a laser name and its return index."""
  return range_images[laser_name][return_index]

def show_range_image(range_image, layout_index_start = 1):
  """Shows range image.

  Args:
    range_image: the range image data from a given lidar of type MatrixFloat.
    layout_index_start: layout offset
  """
  range_image_tensor = tf.convert_to_tensor(range_image.data)
  range_image_tensor = tf.reshape(range_image_tensor, range_image.shape.dims)
  lidar_image_mask = tf.greater_equal(range_image_tensor, 0)
  range_image_tensor = tf.where(lidar_image_mask, range_image_tensor,
                                tf.ones_like(range_image_tensor) * 1e10)
  range_image_range = range_image_tensor[...,0] 
  range_image_intensity = range_image_tensor[...,1]
  range_image_elongation = range_image_tensor[...,2]
  plot_range_image_helper(range_image_range.numpy(), 'range', [8, 1, layout_index_start], vmax=75, cmap='gray')
  plot_range_image_helper(range_image_intensity.numpy(), 'intensity', [8, 1, layout_index_start + 1], vmax=1.5, cmap='gray')
  plot_range_image_helper(range_image_elongation.numpy(), 'elongation', [8, 1, layout_index_start + 2], vmax=1.5, cmap='gray')
                   
frame.lasers.sort(key=lambda laser: laser.name)
show_range_image(get_range_image(open_dataset.LaserName.TOP, 0), 1)
show_range_image(get_range_image(open_dataset.LaserName.TOP, 1), 4)
```

 라이다 이미지도 이미지인 관계로, matplotlib를 활용합니다. 다만 라이다 이미지는 3D 이미지라서 거리의 값을 가지고 있기 때문에, 거리를 명암으로 표현합니다.

<img src="https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/WAYMO/Range_Images.png" width="650">


####  측정 이미지 시각화

```
def rgba(r):
  """Generates a color based on range.

  Args:
    r: the range value of a given point.
  Returns:
    The color for a given range
  """
  c = plt.get_cmap('jet')((r % 20.0) / 20.0)
  c = list(c)
  c[-1] = 0.5  # alpha
  return c

def plot_image(camera_image):
  """Plot a cmaera image."""
  plt.figure(figsize=(20, 12))
  plt.imshow(tf.image.decode_jpeg(camera_image.image))
  plt.grid("off")

def plot_points_on_image(projected_points, camera_image, rgba_func,point_size=5.0):
  """Plots points on a camera image.

  Args:
    projected_points: [N, 3] numpy array. The inner dims are
      [camera_x, camera_y, range].
    camera_image: jpeg encoded camera image.
    rgba_func: a function that generates a color from a range value.
    point_size: the point size.

  """
  plot_image(camera_image)

  xs = []
  ys = []
  colors = []

  for point in projected_points:
    xs.append(point[0])  # width, col
    ys.append(point[1])  # height, row
    colors.append(rgba_func(point[2]))

  plt.scatter(xs, ys, c=colors, s=point_size, edgecolors="none")


images = sorted(frame.images, key=lambda i:i.name)
cp_points_all_concat = np.concatenate([cp_points_all, points_all], axis=-1)
cp_points_all_concat_tensor = tf.constant(cp_points_all_concat)

# The distance between lidar points and vehicle frame origin.
points_all_tensor = tf.norm(points_all, axis=-1, keepdims=True)
cp_points_all_tensor = tf.constant(cp_points_all, dtype=tf.int32)

mask = tf.equal(cp_points_all_tensor[..., 0], images[0].name)

cp_points_all_tensor = tf.cast(tf.gather_nd(cp_points_all_tensor, tf.where(mask)), dtype=tf.float32)
points_all_tensor = tf.gather_nd(points_all_tensor, tf.where(mask))

projected_points_all_from_raw_data = tf.concat([cp_points_all_tensor[..., 1:3], points_all_tensor], axis=-1).numpy()

plot_points_on_image(projected_points_all_from_raw_data, images[0], rgba, point_size=5.0)
```

 거리 값을 가진 이미지를 거리에 따라 색을 다르게 만들어 보여줍니다.

<img src="https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/WAYMO/Camera_Projection.png" width="650">


## 출처

 소개: [waymo.com/open/data/perception/](https://waymo.com/open/data/perception/)

 주석 설명: [github.com/waymo-research/waymo-open-dataset/blob/master/docs/labeling\_specifications.md](https://github.com/waymo-research/waymo-open-dataset/blob/master/docs/labeling_specifications.md)
