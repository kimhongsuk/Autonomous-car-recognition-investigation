## 소개

<img src="https://www.nuscenes.org/public/images/shape.svg" width="150">

 nuScenes dataset은 2019년 3월에 공개된 연구 목적의 비상업용 개방형 오픈데이터로 교통 혼잡지역으로 알려진 보스턴과 싱가포르의 주행 장면 1000개를 제공합니다.

 주행 장면은 개당 2HZ, 20초의 영상으로, 물체 단위로 3차원 박스가 23개의 클래스와 8개의 속성으로 주석이 달려있으며 다양하고 흥미로운 운전 상황, 교통 상황, 흔치 않은 행동들을 포함합니다.

 데이터셋(40만 주요 프레임)에는 140만개의 카메라 이미지, 39만개의 라이다 정보, 140만개의 레이더 정보, 140만 개의 물체 bounding 박스가 포함되어 있습니다.

 추가로 2020년 7월에 nuScenes-lidarseg를 공개하였는데, 4만개의 프인트 클라우드와 1000개의 장면에 14억개의 포인트가 포함되어 있습니다.

 비상업적 용도로는 무료이나, 상업적 용도로 사용하려면 이메일을 통해 상업용 라이센스를 취득해야 합니다.

 데이터를 이용해 물체의 감지, 추적, 예측을 위한 알고리즘을 만들거나 딥러닝을 수행할 수 있습니다. 또한 홈페이지에서 점수를 통해 다른 사람들과 성능을 겨룰 수 있습니다.

## 데이터 수집 센서

  자동차에 설치된 센서들은 다음과 같이 있습니다.

<img src="https://www.nuscenes.org/public/images/data.png" width="500">

-   회전형 LiDAR 1개
    -   20Hz 인식 속도
    -   32채널
    -   360º 수평 FOV, +10º ~ -30º 수직 FOV
    -   최대 80~100m 측정 거리, 70m 측정 범위
    -   측정 오차 범위 2cm 
    -   1초마다 139만개의 Point 측정 가능

-   장거리 RADAR 5개
    -   77GHz Wave
    -   13Hz 인식 속도
    -   Frequency Modulated Continuous Wave(주파수 변조 연속파동)을 이용하여 1회 주기로 거리와 속도를 독립적으로 측정
    -   250m 측정 범위
    -   속도 측정 오차 범위 0.1km/h

-   카메라 6개
    -   12Hz 인식 속도
    -   1/1.8인치 CMOS 센서
    -   1600x1200 화질
    -   Bayer8 포맷
    -   처리 속도와 전송 속도를 줄이기 위해 1600x900으로 crop
    -   최대 20ms의 카메라 노출
    -   이미지들은 JPEG로 압축되고 BGR로 unpacked

 고품질의 데이터를 얻기 위해 다음과 같이 센서를 calibration합니다.

-   LiDAR: 레이저를 이용하여 lidar의 정확한 위치를 측정하여 조절합니다.
-   카메라: 카메라와 LiDAR 앞에 큐브 모양의 calibation 타겟을 두고 LiDAR의 측정 위치에 맞춰 카메라를 조절합니다. 또한 알고있는 pattern을 이용하여 카메라 왜곡을 보정합니다.
-   RADAR: 동적 물체의 RADAR 측정치를 필터링한 후, 정적 물체의 보정치를 최소화하기 위해 yaw각을 조절합니다.

 센서동기화를 위해  20Hz의 LiDAR와 12Hz의 카메라를 동기화 할 때,  리소스를 줄이기 위해 12Hz로 통일합니다.


## 데이터 주석

 nuScenes는 데이터를 모은 뒤, 2Hz로 잘 동기화된 sample을 만들고 Scale사를 통해 주석처리 하였습니다. 전문가들이 정확도를 높이기 위해 절차를 만들고 수행하여 data를 만듭니다.

 그리고 주석을 만들 때에 특정한 규칙이 있습니다.

1.  물체는 위치와 모양을 알 수 있도록 적어도 라이다나 레이더 포인트 1개가 포함되어야 합니다.
2.  물체를 나타내는 직육면체는 엄청 tight해야 합니다.
3.  물체의 끝과 끝은 모두 포함되어야 합니다. 예를 들어 사람이 팔을 벌리고 있다면 전체를 포함해야 합니다.
4.  보행자가 운반하는 물건도 포함합니다. 다만 2명의 사람이 하나의 물건을 같이 드는 경우 한명에게만 포함됩니다.
5.  간혹 정적 물체가 움직이는 경우, 에러를 방지하기 위해 별도의 직육면체를 만들어야 합니다.
6.  LiDAR나 RADAR에 잘 찍히지 않는 물체는 카메라 이미지를 통해 크기를 판별합니다.
7.  모든 카메라가 볼 수 있는 물체는 특별한 속성을 부여합니다.

 주석은 총 23개의 object class로 제공되며, 모든 nuScene의 데이터셋은 아래와 같은 category로 제공됩니다.

-    vehicle.car
    -   10인승 이하의 자가용 차량들(세단, 해치백, 웨건, 미니밴, SUV 등)을 의미합니다.

<img src="https://camo.githubusercontent.com/6554d6697f9440b01aef68ebebd40a39e3d9709faeb83250ad2417623f8319a4/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f706572736f6e616c5f76656869636c655f322e6a7067" width="500">

-   vehicle.truck
    -   픽업트럭이나 탱크로리 같은 화물트럭을 의미합니다. 단, 트레일링 트럭은 트랙터 부분만 가리킵니다.

<img src="https://camo.githubusercontent.com/c8a7875ad8c849a48a224ed781a9233131243e25d606efd1abb841ac3c997a10/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f66726f6e745f6f665f73656d695f747275636b5f332e706e67" width="500">

-   vehicle.bus.bendy
    -    10인승 이상의 버스 중에서 2개 이상의 section을 갖져 굴절이 되는 버스를 의미합니다.

<img src="https://camo.githubusercontent.com/a3785809f8988768d0e90bef18c7b9f05f91262d4b1449036785b3f142fb968d/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f62656e64795f6275735f312e6a7067" width="500">

-   vehicle.bus.rigid
    -   일반적인 10인승 이상의 버스를 의미합니다.

<img src="https://camo.githubusercontent.com/ab38678d0ac8889f3cd331dfab478c0c2a43fa3f781bf10aab77089ef5e14f00/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f72696769645f6275735f322e6a7067" width="500">

-   vehicle.construction
    -   공사현장에 쓰는 중장비들을 의미합니다. 단, 덤프트럭같은 트럭류는 vehicle.truck으로 표시됩니다. 또한 교통흐름에 방해되는 중장비만 표시합니다.

<img src="https://camo.githubusercontent.com/61d21b12abccaea29e5ee3623e291f0a22ae9929f81c26439212e6d4540a6cf7/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f636f6e737472756374696f6e5f76656869636c655f332e6a7067" width="500">

-   vehicle.motorcycle
    -   오토바이를 의미합니다. 또한 아시아의 3륜 오토바이 또한 같은 카테고리에 포함됩니다. 단, 운전자와 뒷자리 승객도 포함하지만 오토바이 옆에 서 있는 사람은 포함하지 않습니다.

<img src="https://camo.githubusercontent.com/825e8b75582f5509cbb97f5933d4ed27a92a971e1a4570663ee10da879872d70/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6d6f746f726379636c655f342e6a7067" width="500">

-   vehicle.bicycle
    -   도로 위나 인도나 자전거 전용도로에 있는 자전거를 의미합니다. 단, 오토바이처럼 옆에 서 있는 사람은 포함하지 않습니다.

<img src="https://camo.githubusercontent.com/b861e52610f45f47368385cc0345016d72c6b4945f94c241cf2c0499ffe695e3/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f62696379636c655f342e6a7067" width="500">

-   static\_object.bicycle\_rack
    -   자전거 거치대를 의미합니다. 보관된 자전거도 포함되나 별도로 주차된 자전거는 vehicle.bicycle로 인식됩니다.

<img src="https://camo.githubusercontent.com/621e14a3624f9e07e16c6f32834e1c6fa6016b786f9a56a30fc6d7840a6cd62e/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f62696b655f7261636b5f332e706e67" width="500">

-   vehicle.trailer
    -   트럭이나 자동차, 자전거로 견인되는 트레일러를 의미합니다. 단, 자동차를 견인하는 경우는 vehicle로 인식됩니다.

<img src="https://camo.githubusercontent.com/44b226159e1debcda9579c572d73260381a6cb62dd72e1c2fad4de30914af53d/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f747261696c65725f322e6a7067" width="500">

-   vehicle.emergency.police
    -   경찰이 타고다니는 모든 운송수단을 의미합니다.

<img src="https://camo.githubusercontent.com/be4cd012e392d22686d7c498de59bf8e8d80d1346116ebf7d273d52cb444506d/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f706f6c6963655f76656869636c655f342e706e67" width="500">

-   vehicle.emergency.ambulance
    -   앰뷸런스 차량이면 모두 포함합니다.

<img src="https://camo.githubusercontent.com/0fcd1ed340cdb0867ba802f1c2497f51f0c429731adcd727d489c970a8edb6a9/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f616d62756c616e63655f322e6a7067" width="500">

-   human.pedestrian.adult
    -   도시에 보이는 모든 어른 보행자를 의미합니다. 심지어 마네킹도 포함됩니다.

<img src="https://camo.githubusercontent.com/76676b59c801d994deef1622bcc9f1b0d2e1f9f7d97ab74eee5aa0ef96c0d5a8/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6164756c745f7065646573747269616e5f332e6a7067" width="500">

-   human.pedestrian.child
    -   보행하는 모든 아이들을 의미합니다.

<img src="https://camo.githubusercontent.com/127e8f3511068cbcf756c71291831014ba20f25e70d85ba04439508308fa8021/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6368696c645f7065646573747269616e5f342e6a7067" width="500">

-   human.pedestrian.construction\_worker
    -   공사란 목적을 가진 인부들을 의미합니다.

<img src="https://camo.githubusercontent.com/d711af31229c5af4cd5d050269abb789358f51c1a320ee95774ffa08d1798025/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f636f6e737472756374696f6e5f776f726b65725f342e6a7067" width="500">

-   human.pedestrian.stroller
    -   모든 유모차를 포함합니다. 유모차에 타고있는 아기들도 포함되며, 유모차를 끄는 부모님들은 별도로 주석이 됩니다.

<img src="https://camo.githubusercontent.com/3def72d3cf00160a638fd47abc8e1af61e209ab05e80bd9f598190709eb6cae3/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f7374726f6c6c65725f362e6a7067" width="500">

-   human.pedestrain.wheelchair
    -   모든 휠체어를 포함합니다. 유모차와 같은 방식으로 동작합니다.

<img src="https://camo.githubusercontent.com/415d9b06d56a520898669355c6939d780a168d076f0375cac4f0c10501a65489/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f776865656c63686169725f312e6a7067" width="500">

-   human.pedestrian.person\_mobility
    -   전동 킥보드같이 직립주행하는 운송수단을 의미합니다. 운전자도 포함됩니다.

<img src="https://camo.githubusercontent.com/d124fc77f8cda2b6796488eea4701eeeac88f5e64e4eb8c36d16d147deaf15ae/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f706572736f6e616c5f6d6f62696c6974795f312e706e67" width="500">

-   human.pedestrian.police\_officer
    -   모든 경찰관들을 의미합니다. 도로 교통 지도를 하는 경찰관도 포함됩니다.

<img src="https://camo.githubusercontent.com/17aec1764afae4063502280f04192a743f13589e61fb5ac1f1fa7e98290943ee/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6f746865725f706f6c6963655f312e706e67" width="500">

-   animal
    -   모든 종류의 동물들을 의미합니다.

<img src="https://camo.githubusercontent.com/3c6e182d16b12cd80f55df8ba125482c013e460bbec98f8e3de4bd8e5da236bf/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f736d616c6c5f616e696d616c5f312e706e67" width="500">

-   movable\_object.trafficcone
    -   모든 종류의 라바콘을 의미합니다.

<img src="https://camo.githubusercontent.com/eff8da5a18fd5d3767a1614d1cde4bdfbe2a69dbb42294dcfa2d8fe713d5735c/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f747261666669635f636f6e655f322e6a7067" width="500">

-   movable\_object.barrier
    -   임시로 설치된 방벽을 의미합니다. 단, 영구적으로 설치된 방벽은 주석처리 하지 않으며, 여러개의 방벽이 합쳐지는 구조라면 분리된 상태로 인식합니다.

<img src="https://camo.githubusercontent.com/a7ab7d3f062f7ee72d7ac4e5731d41fd27ad389ef58bf43c87c0230a760c358b/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f74656d706f726172795f747261666669635f626172726965725f312e6a7067" width="500">

-   movable\_object.pushable\_pullable
    -   보행자가 옮길 수 있는 물체들을 의미합니다. 다만 일반적으로 사람을 태울 수 있는 구조가 아니어야 합니다.

<img src="https://camo.githubusercontent.com/cbbba8248c0d118148517dc2527651ed723b389012337d2d51b97430f050377b/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f7075736861626c655f70756c6c61626c655f332e706e67" width="500">

-   movable\_object.debris
    -   도로에 떨어져 안전 운전을 방해하는 모든 쓰레기를 의미합니다. 나무같은 물체도 포함합니다.

<img src="https://camo.githubusercontent.com/9825c476d407b96531584e0bbb48bebe0ea7ed1dcf7cbf0b372e3658ebab084c/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6d6f7661626c655f6f62737461636c655f312e706e67" width="500">

 nuScenes-lidarseg의 경우, 모든 점에 주석을 달려있습니다. 그리고 23개의 물체에 추가로, 배경에 대한 9가지 class가 있습니다.

-   noise
    -   먼지나 증기, 소음, 안개, 빗방울, 연기 및 반사 등으로 인해 물체와 일치하지 않는 라이다의 값을 의미합니다.

<img src="https://camo.githubusercontent.com/d26be6b92e3cbc602455ff8cadc4887368a745933d3927f4e7c394326378bec6/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f305f7363656e652d303633355f43414d5f46524f4e545f313533373239363038363836323430345f63726f702e6a7067" width="500">

-   flat.driveable\_surface
    -   교통 법규 상관없이 주행 가능한 모든 길을 의미합니다.

<img src="https://camo.githubusercontent.com/d456a3144b9937264cb6c960e2206df8da824da42b7db12585bfa7c4bfc23abd/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f32345f393735305f43414d5f46524f4e542e6a7067" width="500">

-   flat.sidewalk
    -   인도나 자전거 전용 도로 등 사람이 다닐 수 있는 모든 길을 의미합니다.

<img src="https://camo.githubusercontent.com/ea00ce67627dbd2423015261b8ed7626081e69445b615251fa1c3bb8890914fa/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f32355f3238305f43414d5f46524f4e545f4c4546542e6a7067" width="500">

-   flat.terrain
    -   지상으로부터 20cm 높이 까지 올라온 모든 땅을 의미합니다. 단, 풀이나 모래나 자갈같이 자연적인 땅입니다.

<img src="https://camo.githubusercontent.com/6e09d25c2d9c31c8e28f45fee4f082d428a43bbb0ec0c8d225e1b4bc962a6646/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f32365f313236305f43414d5f4241434b5f4c4546542e6a7067" width="500">

-   flat.other
    -   위에 소개된 지상 구조물을 제외한 모든 것들을 의미합니다. 강이나 호수, 기찻길 등을 의미합니다.

<img src="https://camo.githubusercontent.com/7ae0e3068e493828f21f2336c2274d912b1bc604e8008e9384b53832c15b4805/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f32375f323331385f43414d5f46524f4e542e6a7067" width="500">

-   flat.manmade
    -   인공 구조물을 의미합니다. 3층 높이 이상의 계단과 벽이든 빌딩이든 인공 구조물 모두 포함합니다.

<img src="https://camo.githubusercontent.com/68fcd28d9fa6329365b2b648845712361bf119c1c946b1caefa86792542ca17a/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f32385f353030395f43414d5f46524f4e542e6a7067" width="500">

-   static.vegetation
    -   20cm 이상의 모든 덤불, 나무 등을 의미합니다. 단, 바닥에 있는 풀들은 flat.terrain에 포함됩니다.

<img src="https://camo.githubusercontent.com/0d10a4a577be4028661d38a3dfb393d837de0d40567518d743074537b8fd3017/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f32395f353631305f43414d5f4241434b5f52494748542e6a7067" width="500">

-   static.other
    -   구별을 할 수 없거나 위의 어떠한 주석으로도 쓰지 못한 것들을 의미합니다.

<img src="https://camo.githubusercontent.com/4162821e7de2ae53a1b09e42777ab9db254b9c8d00fb4b62e0ad27b93c883c95/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f33305f7363656e652d303136365f43414d5f4241434b5f52494748545f313532363931353338303532373831332e6a7067" width="500">

-   vehicle.ego
    -   센서의 위치와 범위 때문에 어쩔수 없이 보이게 되는 자동차 자체를 의미합니다.

<img src="https://camo.githubusercontent.com/1075e384787ab5ec65c641c655506a16c49280d5d17a11d25ac85c27450b3c82/68747470733a2f2f7777772e6e757363656e65732e6f72672f7075626c69632f696d616765732f7461786f6e6f6d795f696d67732f6c696461727365672f33315f32343233305f4245562e6a7067" width="500">

 추가로 특정 클래스에는 특별한 속성이 있습니다.

-   vehicle.moving
    -   vehicle 클래스의 물체가 움직임을 의미합니다.
-   vehicle.stopped
    -   vehicle 클래스의 물체가 멈춰있음을 의미합니다.
-   vehicle.parked
    -   vehicle 클래스의 물체가 주차되어 있음을 의미합니다.
-   cycle.with\_rider
    -   cycle 클래스의 물체에 운전자가 포함되어 있음을 의미합니다.
-   cycle.without\_rider
    -   cycle 클래스의 물체에 운전자가 포함되어 있지 않음을 의미합니다.
-   pedestrain.sitting\_lying\_down
    -   pedestrian 클래스의 보행자가 앉거나 누워있음을 의미합니다.
-   pedestrian.standing
    -   pedestrian 클래스의 보행자가 일어서 있음을 의미합니다.
-   pedestrain.moving
    -   pedestrain 클래스의 보행자가 움직이고 있음을 의미합니다.


## 데이터 포맷

 모든 주석과 메타 데이터(보정치와 지도, 차량 좌표등을 포함)는 아래의 관계형 데이터베이스에서 다룹니다.

<img src="https://www.nuscenes.org/public/images/nuscenes-schema.svg" width="650">

```
attribute {
   "token":                   <str> -- Unique record identifier.
   "name":                    <str> -- Attribute name.
   "description":             <str> -- Attribute description.
}
```

 attribute는 카테고리가 그대로 유지되는 동안 scene의 여러 부분에서 변경될 수 있는 요소의 속성을 나타냅니다. 예) 자전거에 사람이 타있는가 아닌가 or 자동차가 주차되어 있는가 움직이는가

```
calibrated_sensor {
   "token":                   <str> -- Unique record identifier.
   "sensor_token":            <str> -- Foreign key pointing to the sensor type.
   "translation":             <float> [3] -- Coordinate system origin in meters: x, y, z.
   "rotation":                <float> [4] -- Coordinate system orientation as quaternion: w, x, y, z.
   "camera_intrinsic":        <float> [3, 3] -- Intrinsic camera calibration. Empty for sensors that are not cameras.
}
```

 calibrated\_sensor는 특정 센서의 calibation을 진행한 paramter를 나타냅니다.

```
category {
   "token":                   <str> -- Unique record identifier.
   "name":                    <str> -- Category name. Subcategories indicated by period.
   "description":             <str> -- Category description.
   "index":                   <int> -- The index of the label used for efficiency reasons in the .bin label files of nuScenes-lidarseg. This field did not exist previously.
}
```

 category는 카테고리의 분류를 나타냅니다. 하위 카테고리는 마침표로 구분됩니다. 예) human.pedestrian.adult

```
ego_pose {
   "token":                   <str> -- Unique record identifier.
   "translation":             <float> [3] -- Coordinate system origin in meters: x, y, z. Note that z is always 0.
   "rotation":                <float> [4] -- Coordinate system orientation as quaternion: w, x, y, z.
   "timestamp":               <int> -- Unix time stamp.
}
```

 ego\_pose는 특정 시간의 자율주행 자동차의 pose를 나타냅니다. ego\_pose는 라이다의 맵 기반 localization한 출력값을 가지며 x-y 평면의 2차원 값을 가집니다.

```
instance {
   "token":                   <str> -- Unique record identifier.
   "category_token":          <str> -- Foreign key pointing to the object category.
   "nbr_annotations":         <int> -- Number of annotations of this instance.
   "first_annotation_token":  <str> -- Foreign key. Points to the first annotation of this instance.
   "last_annotation_token":   <str> -- Foreign key. Points to the last annotation of this instance.
}
```

 instance는 차량이나 사람같은 물체를 나타냅니다. 이 table은 nuScenes팀에서 관찰한 모든 물체들을 열거합니다.

```
lidarseg {
   "token":                   <str> -- Unique record identifier.
   "filename":                <str> -- The name of the .bin files containing the nuScenes-lidarseg labels. These are numpy arrays of uint8 stored in binary format using numpy.
   "sample_data_token":       <str> -- Foreign key. Sample_data corresponding to the annotated lidar pointcloud with is_key_frame=True. 
}
```

 lidarseg는 nuScenes-lidarseg의 주석과 lidar pointclud에 해당하는 샘플 데이터를 mapping 해줍니다.

```
log {
   "token":                   <str> -- Unique record identifier.
   "logfile":                 <str> -- Log file name.
   "vehicle":                 <str> -- Vehicle name.
   "date_captured":           <str> -- Date (YYYY-MM-DD).
   "location":                <str> -- Area where log was captured, e.g. singapore-onenorth.
}
```

 log는 데이터에서 나온 log정보를 나타냅니다.

```
map {
   "token":                   <str> -- Unique record identifier.
   "log_tokens":              <str> [n] -- Foreign keys.
   "category":                <str> -- Map category, currently only semantic_prior for drivable surface and sidewalk.
   "filename":                <str> -- Relative path to the file with the map mask.
}
```

 map은 binary semantic masks로 저장된 map data입니다.

```
sample {
   "token":                   <str> -- Unique record identifier.
   "timestamp":               <int> -- Unix time stamp.
   "scene_token":             <str> -- Foreign key pointing to the scene.
   "next":                    <str> -- Foreign key. Sample that follows this in time. Empty if end of scene.
   "prev":                    <str> -- Foreign key. Sample that precedes this in time. Empty if start of scene.
}
```

 sample은 2Hz 주석처리된 데이터들을 나타냅니다. 데이터는 lidar와 거의 같은 시간으로 수집되어 있습니다.

```
sample_annotation {
   "token":                   <str> -- Unique record identifier.
   "sample_token":            <str> -- Foreign key. NOTE: this points to a sample NOT a sample_data since annotations are done on the sample level taking all relevant sample_data into account.
   "instance_token":          <str> -- Foreign key. Which object instance is this annotating. An instance can have multiple annotations over time.
   "attribute_tokens":        <str> [n] -- Foreign keys. List of attributes for this annotation. Attributes can change over time, so they belong here, not in the instance table.
   "visibility_token":        <str> -- Foreign key. Visibility may also change over time. If no visibility is annotated, the token is an empty string.
   "translation":             <float> [3] -- Bounding box location in meters as center_x, center_y, center_z.
   "size":                    <float> [3] -- Bounding box size in meters as width, length, height.
   "rotation":                <float> [4] -- Bounding box orientation as quaternion: w, x, y, z.
   "num_lidar_pts":           <int> -- Number of lidar points in this box. Points are counted during the lidar sweep identified with this sample.
   "num_radar_pts":           <int> -- Number of radar points in this box. Points are counted during the radar sweep identified with this sample. This number is summed across all radar sensors without any invalid point filtering.
   "next":                    <str> -- Foreign key. Sample annotation from the same object instance that follows this in time. Empty if this is the last annotation for this object.
   "prev":                    <str> -- Foreign key. Sample annotation from the same object instance that precedes this in time. Empty if this is the first annotation for this object.
}
```

 sample\_annotation은 bounding box 처리된 물체의 여러 데이터를 나타냅니다. 참고로 모든 위치는 global 좌표계로 주어집니다.

```
sample_data {
   "token":                   <str> -- Unique record identifier.
   "sample_token":            <str> -- Foreign key. Sample to which this sample_data is associated.
   "ego_pose_token":          <str> -- Foreign key.
   "calibrated_sensor_token": <str> -- Foreign key.
   "filename":                <str> -- Relative path to data-blob on disk.
   "fileformat":              <str> -- Data file format.
   "width":                   <int> -- If the sample data is an image, this is the image width in pixels.
   "height":                  <int> -- If the sample data is an image, this is the image height in pixels.
   "timestamp":               <int> -- Unix time stamp.
   "is_key_frame":            <bool> -- True if sample_data is part of key_frame, else False.
   "next":                    <str> -- Foreign key. Sample data from the same sensor that follows this in time. Empty if end of scene.
   "prev":                    <str> -- Foreign key. Sample data from the same sensor that precedes this in time. Empty if start of scene.
}
```

 sample\_data는 image나 point cloud 같은 센서 데이터를 나타냅니다. is\_key\_frame이 True라면 time-stamp에 맞게 데이터를 가리키지만, False라면 순차적으로 데이터를 가리킵니다.

```
scene {
   "token":                   <str> -- Unique record identifier.
   "name":                    <str> -- Short string identifier.
   "description":             <str> -- Longer description of the scene.
   "log_token":               <str> -- Foreign key. Points to log from where the data was extracted.
   "nbr_samples":             <int> -- Number of samples in this scene.
   "first_sample_token":      <str> -- Foreign key. Points to the first sample in scene.
   "last_sample_token":       <str> -- Foreign key. Points to the last sample in scene.
}
```

 scene은 log와 관련된 20초짜리 연속된 프레임을 나타냅니다. 같은 log에 여려개의 scene이 존재할 수 있습니다. 다만, scene에 객체의 id를 보여주지는 않습니다.

```
sensor {
   "token":                   <str> -- Unique record identifier.
   "channel":                 <str> -- Sensor channel name.
   "modality":                <str> {camera, lidar, radar} -- Sensor modality. Supports category(ies) in brackets.
}
```

 sensor는 센서의 type을 나타냅니다.

```
visibility {
   "token":                   <str> -- Unique record identifier.
   "level":                   <str> -- Visibility level.
   "description":             <str> -- Description of visibility level.
}
```

 visibility는 6개 카메라 이미지 내 instance의 주석의 비율를 나타냅니다. 4개의 항목으로 0-40%, 40-60%, 60-80%, 80-100%가 있습니다.


## 데이터 예시

```
%matplotlib inline
from nuscenes.nuscenes import NuScenes

nusc = NuScenes(version='v1.0-mini', dataroot='/data/sets/nuscenes', verbose=True)

my_scene = nusc.scene[0]
first_sample_token = my_scene['first_sample_token']

my_sample = nusc.get('sample', first_sample_token)
```

 이 부분 까지는 NuScenes 패키지를 받아 scene들을 받은 후 하나의 scene을 가져온 작업입니다. 결과값이 너무 길어 생략합니다.

 scene 중에 하나를 선택하여 sample 데이터를 가져옵니다.

```
my_sample['data']
```

```
{'RADAR_FRONT': '37091c75b9704e0daa829ba56dfa0906',
 'RADAR_FRONT_LEFT': '11946c1461d14016a322916157da3c7d',
 'RADAR_FRONT_RIGHT': '491209956ee3435a9ec173dad3aaf58b',
 'RADAR_BACK_LEFT': '312aa38d0e3e4f01b3124c523e6f9776',
 'RADAR_BACK_RIGHT': '07b30d5eb6104e79be58eadf94382bc1',
 'LIDAR_TOP': '9d9bf11fb0e144c8b446d54a8a00184f',
 'CAM_FRONT': 'e3d495d4ac534d54b321f50006683844',
 'CAM_FRONT_RIGHT': 'aac7867ebf4f446395d29fbd60b63b3b',
 'CAM_BACK_RIGHT': '79dbb4460a6b40f49f9c150cb118247e',
 'CAM_BACK': '03bea5763f0f4722933508d5999c5fd8',
 'CAM_BACK_LEFT': '43893a033f9c46d4a51b5e08a67a1eb7',
 'CAM_FRONT_LEFT': 'fe5422747a7d4268a4b07fc396707b23'}
```

 sample 데이터에서 data 값을 추출해 가져오면 해당 센서가 가진 이미지 파일의 token을 나타냅니다.

```
sensor = 'CAM_FRONT'
cam_front_data = nusc.get('sample_data', my_sample['data'][sensor])
cam_front_data
```

```
{'token': 'e3d495d4ac534d54b321f50006683844',
 'sample_token': 'ca9a282c9e77460f8360f564131a8af5',
 'ego_pose_token': 'e3d495d4ac534d54b321f50006683844',
 'calibrated_sensor_token': '1d31c729b073425e8e0202c5c6e66ee1',
 'timestamp': 1532402927612460,
 'fileformat': 'jpg',
 'is_key_frame': True,
 'height': 900,
 'width': 1600,
 'filename': 'samples/CAM_FRONT/n015-2018-07-24-11-22-45+0800__CAM_FRONT__1532402927612460.jpg',
 'prev': '',
 'next': '68e8e98cf7b0487baa139df808641db7',
 'sensor_modality': 'camera',
 'channel': 'CAM_FRONT'}
```

 CAM\_FRONT에서 가져온 sample\_data의 메타 데이터를 볼 수 있습니다.

```
nusc.render_sample_data(cam_front_data['token'])
```

<img src="https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/NUSCENES/sample_data.png" width="500">

 sample\_data를 렌더링하면 상단과 같은 이미지를 얻을 수 있습니다.

```
my_annotation_token = my_sample['anns'][18]
my_annotation_metadata =  nusc.get('sample_annotation', my_annotation_token)
my_annotation_metadata
```

```
{'token': '83d881a6b3d94ef3a3bc3b585cc514f8',
 'sample_token': 'ca9a282c9e77460f8360f564131a8af5',
 'instance_token': 'e91afa15647c4c4994f19aeb302c7179',
 'visibility_token': '4',
 'attribute_tokens': ['58aa28b1c2a54dc88e169808c07331e3'],
 'translation': [409.989, 1164.099, 1.623],
 'size': [2.877, 10.201, 3.595],
 'rotation': [-0.5828819500503033, 0.0, 0.0, 0.812556848660791],
 'prev': '',
 'next': 'f3721bdfd7ee4fd2a4f94874286df471',
 'num_lidar_pts': 495,
 'num_radar_pts': 13,
 'category_name': 'vehicle.truck'}
```

 추가적으로 주석처리된 부분 하나를 추출하여 메타 데이터를 확인하면 상단과 같이 볼 수 있습니다.

```
nusc.render_annotation(my_annotation_token)
```


<img src="https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/NUSCENES/sample_annotiation.png" width="500">

 그리고 추출된 데이터를 렌더링하면 아까 전의 사진과 다르게, 직육면체가 하나만 보이게 됩니다.

 이들 외에 데이터를 조작할 수 있는 방법이 많지만, 상단의 방법이 데이터를 확인하는 기초적인 방법입니다.

## lidarseg

 처음 공개된 nuScenes는 직육면체와 bounding box를 통해 물체를 나타냈습니다. 하지만 이것으로 물체의 디테일함을 나타내기는 부족했습니다. 그래서 nuScenes-lidarseg는 라이다 포인트들을 이용하여 물체를 더 세밀하게 나타내고, 9개의 배경 클래스를 추가하여 더 큰 가능성을 포함합니다.

 그리고 nuScenes-lidarseg는 nuScenes이나 nuImages와 호환되기 때문에 센서 보정이나 매핑, point cloud 분할 같은 여러 문제를 해결할 수 있는 기회를 줍니다.


## 출처

 nuScenes 소개 및 데이터 예시 풀버전: [www.nuscenes.org/nuscenes](https://www.nuscenes.org/nuscenes)

 nuScenes 주석: [github.com/nutonomy/nuscenes-devkit/blob/master/docs/instructions\_nuscenes.md#attributes](https://github.com/nutonomy/nuscenes-devkit/blob/master/docs/instructions_nuscenes.md#attributes)

 nuScenes-lidarseg 주석: [github.com/nutonomy/nuscenes-devkit/blob/master/docs/instructions\_lidarseg.md](http://github.com/nutonomy/nuscenes-devkit/blob/master/docs/instructions\_lidarseg.md)
