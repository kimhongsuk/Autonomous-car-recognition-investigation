## 소개

![KITTI](http://www.cvlibs.net/datasets/kitti/images/mpi.jpg)

 KITTI는 2012년 3월에 공개된 오픈데이터 및 benchmark로 WAYMO나 nuScenes와 달리 데이터셋을 직접 다운받아 사용합니다.

 주행 장면은 rawData 기준 시간이 졍해져 있지 않고 약 10Hz의 frame을 갖고 있습니다. 그리고 8가지 종류의 주석이 포함되어 있습니다. 그러나 공개된 데이터들을 대부분 2011년 9월에 만들어진 파일들입니다.

 다른 데이터셋에 비해 오래된 데이터이고 신호등이 주석 처리 되어있지 않아 학습에 제한이 있는 단점이 있지만, KITTI를 활용한 논문이 많기 때문에 관련 자료가 방대하다는 장점이 있습니다.

 그리고 benchmark로 활용이 가능해서 알고리즘 점수를 알아볼 수 있는 활용도가 있습니다.

 KITTI-360이라는 조금 더 최신화된 데이터셋이 있으나, 다운로드를 위해 가입이 필요하고 다른 데이터셋보다 오래되고 관련 자료가 적다는 단점 때문에 생략합니다.


## 데이터 수집 센서

-   GPS 1개
    -   제품명: OXTS RT 3003
    -   기능: 
-   LiDAR 1개
    -   제품명: Velodyne HDL-64E
    -   10Hz frame
    -   초당 10만개 points 측정가능
-   흑백 카메라 2개
    -   제품명: Point Grey Flea 2 (FL2-14S3M-C) 140만 화소
    -   최대 셔터 스피드: 2ms
    -   라이다의 트리거에 맞춰 촬영
-   컬러 카메라 2개
    -   제품명: Point Grey Flea 2 (FL2-14S3C-C) 140만 화소
    -   최대 셔터 스피드: 2ms
    -   라이다의 트리거에 맞춰 촬영

![KITTI Car Setup](http://www.cvlibs.net/datasets/kitti/images/setup_top_view.png)


## 데이터 주석

 KITTI는 주석이 8개의 class로 이루어져 있습니다. 그리고 xml 파일로 저장되어 있습니다.

 옛날 데이터라서 표지판이나 신호등이 주석 항목에 포함되어 있지 않습니다.

-   Car
-   Van
-   Truck
-   Pedestrians
-   Sitters
-   Cyclists
-   Trams
    -   독일의 데이터라서 트램이 포함되어 있습니다.
-   Misc
    -   상단 항목 외 기타 항목입니다.

## 데이터 포맷

 주석 처리하는 파일은 다음과 같이 되어 있으며 중요부분을 제외하고 생략합니다.

```
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<!DOCTYPE boost_serialization>
<boost_serialization signature="serialization::archive" version="9">
<tracklets class_id="0" tracking_level="0" version="0">
	<count>15</count>
	<item_version>1</item_version>
	<item class_id="1" tracking_level="0" version="1">
		<objectType>Car</objectType>
		<h>2.1672349</h>
		<w>1.7057118</w>
		<l>4.3293562</l>
		<first_frame>0</first_frame>
		<poses class_id="2" tracking_level="0" version="0">
			<count>14</count>
			<item_version>2</item_version>
			<item class_id="3" tracking_level="0" version="2">
				<tx>25.212516037456261</tx>
				<ty>8.6033915444744817</ty>
				<tz>-1.7921571450416216</tz>
				<rx>0</rx>
				<ry>0</ry>
				<rz>-3.1842001424562025</rz>
				<state>2</state>
				<occlusion>0</occlusion>
				<occlusion_kf>1</occlusion_kf>
				<truncation>0</truncation>
				<amt_occlusion>0</amt_occlusion>
				<amt_occlusion_kf>1</amt_occlusion_kf>
				<amt_border_l>0</amt_border_l>
				<amt_border_r>1</amt_border_r>
				<amt_border_kf>-1</amt_border_kf>
			</item>
		</poses>
		<finished>1</finished>
	</item>
```

 물체의 type과 boundingbox의 크기가 있으며 매 프레임마다 위치와 속성을 갱신하여 표시합니다.

 frame별로 자동차의 데이터는 txt 파일로 존재하는데, SQL 방식이 아닌 순차적으로 숫자가 적혀있습니다. 그리고 이 순서는 다음과 같이 정해져 있습니다. 미세하게 조정하기 위해 여러 방식이 적혀있지만, 가급적 일부를 추출해서 사용합니다.

-   lat: 차량의 위도 (deg)
-   lon: 차량의 경도 (deg)
-   alt: 차량의 고도 (deg)
-   roll: roll 값 (rad), 왼쪽이 들리면 양수
-   pitch: pitch 값 (rad), 앞면이 내려가면 양수
-   yaw: yaw 값 (rad), 동쪽이 0이며 시계방향이 양수 (-pi ~ +pi)
-   vn: 북쪽으로 향하는 속도 (m/s)
-   ve: 동쪽으로 향하는 속도 (m/s)
-   vf: 앞으로 가는 속도 (m/s)
-   vl: 왼쪽으로 가는 속도 (m/s)
-   vu: 위쪽으로 가는 속도 (m/s)
-   ax: x 방향으로 가는 가속도 (m/s^2), 자동차 앞쪽 방향
-   ay: y 방향으로 가는 가속도 (m/s^2), 자동차 왼쪽 방향
-   az: z 방향으로 가는 가속도 (m/s^2), 자동차 위쪽 방향
-   af: 앞으로 가는 가속도 (m/s^2)
-   al: 왼쪽으로 가는 가속도 (m/s^2)
-   au: 위쪽으로 가는 가속도 (m/s^2)
-   wx: x축 각속도 (rad/s)
-   wy: y축 각속도 (rad/s)
-   wz: z축 각속도 (rad/s)
-   wf: 앞쪽 방향축 각속도 (rad/s)
-   wl: 왼쪽 방향축 각속도 (rad/s)
-   wu: 위쪽 방향축 각속도 (rad/s)
-   pos\_accuracy: 위치 정확도
-   vel\_accuracy: 속도 정확도
-   navstat: 네비게이션 상태
-   numsats: GPS와 연결된 인공위성 수
-   posmode: GPS의 위치 모드
-   velmode: GPS의 속도 모드
-   orimode: GPS의 방향 모드

 센서의 calibration 값은 cam to cam, imu to velo, velo to cam (velo는 velodyne의 약자로 LiDAR를 의미)으로 이루어져 있으며 calibration한 시각과 세부값이 있습니다.

## 데이터 예시

![Image 00](https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/KITTI/Image_00.png)

![Image 01](https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/KITTI/Image_01.png)

![Image 02](https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/KITTI/Image_02.png)

![Image 03](https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/KITTI/Image_03.png)

 첫번째 사진과 두번째 사진은 흑백 카메라로 촬영한 사진이고, 세번째 사진과 네번째 사진은은 컬러 카메라로 촬영한 사진입니다. 그리고 같은 특성을 가진 사진들의 차이점은 차량 중앙에 위치한 카메라인가, 측면에 위치한 카메라인가 입니다.

 이 외 calibration이나 주석파일들은 상단에 설명한 자료들이 있으므로 생략합니다.

## 출처

 KITTI: [www.cvlibs.net/datasets/kitti/index.php](http://www.cvlibs.net/datasets/kitti/index.php)
 
 데이터 다운로드: [www.cvlibs.net/datasets/kitti/raw\_data.php](http://www.cvlibs.net/datasets/kitti/raw_data.php)
