## 소개

<img src="https://pjreddie.com/media/image/yologo_2.png" width="200">

 YOLO(You Only Look Once)는 Object Detection을 구현한 오픈소스입니다.

 최근 YOLO, SSD, Mask RCNN, RetinaNet 등의 다양한 Object Detection 알고리즘들을 개발됐는데, YOLO는 기존 모델들 보다 더 높은 정확도를 추구하는 것이 아닌, 근접한 정확도를 가지면서도 더 많은 양의 이미지를 처리할 수 있는 실시간 Object Detection를 하고자 개발되었습니다.

 YOLO의 특징을 세 가지로 요약하면, You Only Look Once(이미지 전체를 단 한 번만 본다), Unified(통합된 모델을 사용한다), Real-time Object Detection(실시간 객체 탐지)가 됩니다.

 첫번째, YOLO 이전 Object Detection 딥러닝 모델은 일정한 규칙으로 이미지를 여러장 쪼개서 CNN모델을 통과시키기 때문에 한장의 이미지에서 객체 탐지를 수행해도 실제로는 수 천장의 이미지를 모델을 통과시킵니다. 하지만 YOLO는 이미지 전체를 말 그대로 단 한번만 통과시킵니다.

 두번째, 다른 Object Detection 딥러닝 모델들은 다양한 전처리 모델과 인공 신경망을 결합해서 사용하지만, YOLO는 단 하나의 인공 신경망에서 전부 처리합니다. 따라서 다른 모델들보다 간단합니다.

 세번째, 실시간으로 여러장의 이미지를 탐지 할 수 있습니다. 예를 들어, Fast RCNN은 0.5FPS인 반면에 YOLO는 45FPS 성능을 보여줍니다. 이는 영상을 스트리밍 하면서 동시에 화면 상의 물체를 부드럽게 구분할 수 있는 정도가 됩니다.

 2016년 처음 발표된 이후 현재 YOLO v5까지 공개되었습니다. YOLO 개발자 Joeeph Redmon은 YOLO v3까지 개발하였으나 군사적 위험, 개인정보 침해 등의 위혐성을 이유로 중단하였습니다. 그 후 Alexey Bochkovskiy가 v4를, Glenn Jocher v5를 발표하였습니다.

 YOLO v2는 v1의 20개 이미지 분류 모델을 9000개 이미지 분류 모델로 발전시켰습니다. 또한 Normalization, Direct Location Prediction, Multi-Scale Training 기법을 도입하여 FPS와 mAP(정확도)를 높였습니다. 그리고 YOLO v3는 v2에 ResNet의 Residual Block을 차용해 darknet-19에서 darknet-53으로 변경해 106개의 신경망 층을 구성하였습니다. YOLO v4는 YOLO v3에서 백본망은 CSP-ResNet, CSP-Darknet, EfficientNet을 적용해 보고 다른 요소들에도 최신 기술들을 적용해본 뒤 가장 결과값이 좋은 것들을 모아둔 소스입니다. 마지막으로 v5는 v4와 비슷한 전략을 취하였습니다. 유의할 점은 v4는 2020년 4월에 발표되었고 v5는 2020년 6월에 발표되었기 때문에 둘 다 v3에서 파생되었다는 점입니다.


## YOLO 네트워크 디자인

 YOLO의 전체적인 설명은 v1을 통해 하겠습니다. (v2, v3는 개량형이기 때문입니다)

<img src="https://miro.medium.com/max/2396/1*Xn6RgHiav56W-vJm_5uhsw.png" width="650">

 24개의 Convolutional Layer와 2개의 Fully Connected Layer로 연결된 구조를 사용하고 있습니다. 그리고 논문에서는 설명을 쉽게하기 위해 Pre-trained Network, Training Network, Reduction Layer 영역을 그려서 구분하였습니다.

 Pre-trained Network는 GoogLeNet을 이용하여 ImageNet 1000-class dataset을 사전에 학습한 결과를 Fine-Tuning한 네트워크를 말합니다. 이 네트워크에서 88%의 정확도를 사전에 학습했다고 합니다.

 Reduction Layer는 깊을 수록 정확도가 높아지는 네트워크의 단점인 연산 시간을 줄이기 위해, GoogLeNet의 기법을 응용하여 연산량은 감소하지만 층은 깊게 쌓는 Layer입니다.

 Training Network는 Pre-Trained Network에서 학습한 feature를 이용하여 Class Probability와 Bounding box를 학습하고 예측하는 네트워크입니다.


## 네트워크 Parameter 설명

 위에서 설명한 네트워크를 나타내는 parameter들은 darknet폴더의 하위 cfg폴더에 정의되어 있습니다. 그래서 널리 쓰고 있는 yolov3.cfg 파일을 간단하게 설명하겠습니다.

```
[net]
# Testing
# batch=1
# subdivisions=1
# Training
batch=64
subdivisions=16
width=608
height=608
channels=3
momentum=0.9
decay=0.0005
angle=0
saturation = 1.5
exposure = 1.5
hue=.1

learning_rate=0.001
burn_in=1000
max_batches = 500200
policy=steps
steps=400000,450000
scales=.1,.1
```

 \[net\]에는 네트워크의 전반적인 설정이 있습니다.

-   batch: 한 번에 처리할 이미지 수입니다. 클수록 정확도가 높아지지만 메모리를 많이 잡아 먹습니다.
-   subdivisions: mini\_batch 계산을 할 때 batch를 분할하는 개수입니다. mini\_batch는 gpu에서 한 번에 처리하는 개수입니다. 따라서 작을수록 정확도가 높아지지만 메모리를 많이 잡아먹습니다.
-   width: 이미지의 가로 픽셀 수입니다.
-   height: 이미지의 세로 픽셀 수입니다.
-   channels: 이미지의 채널입니다. 보통 RGB 이미지 파일을 쓰기 때문에 3입니다.
-   momentum: 경사하강법의 최적화 알고리즘 중 gradient를 보정하는 보정치입니다.
-   decay: 데이터의 불균형을 제거하기 위해 weight가 너무 크지 않도록 합니다.
-   angle: training 시에 랜덤으로 이미지를 회전시킵니다.
-   saturation: training 시에 랜덤으로 이미지의 채도를 변환시킵니다.
-   exposure: training 시에 랜덤으로 이미지의 노출(밝기)를 변환시킵니다.
-   hue: training시에 랜덤으로 이미지의 색상을 변환시킵니다.
-   learning\_rate: training시에 초기 학습률입니다.
-   burn\_in: 해당 training 횟수 동안 learning\_rate를 일정한 수치만큼 높입니다.
-   max\_batches: training이 진행됨에 따라 처리되는 최대 batch 수입니다.
-   policy: training시에 learning\_rate를 어떤 방식으로 조정할지 정합니다. (random, steps 선택)
-   steps: learning\_rate가 조정되는 training 횟수입니다.
-   scales: steps에 따라 learning\_rate에 해당 수치만큼 곱합니다.

```
[convolutional]
batch_normalize=1
filters=32
size=3
stride=1
pad=1
activation=leaky
```

 \[convolutional\]은 convolutional layer층의 설정이며 cfg파일 안에는 여러개가 있습니다.

-   batch\_normalize: batch\_normalize의 유무를 뜻합니다.
-   filters: 해당 convolutionl layer에서 이미지 특징을 찾아내기 위한 필터 개수입니다.
-   size: 필터의 사이즈입니다. 3이면 3X3을 의미합니다.
-   stride: 이미지에서 필터가 순회하는 간격입니다.
-   pad: padding의 유무를 뜻합니다.
-   activation: RELU를 의미합니다.

```
[shortcut]
from=-3
activation=linear
```

 \[shortcut\]은 ResNet에서 사용되는 것과 비슷한 skip connection 설정입니다. shortcut은 convolutional layer 뒤에 위치합니다. YOLO v3는 ResNet의 아이디어를 차용했기 때문에 해당 parameter가 있습니다.

-   from: 해당 개수의 convolutional layer 뒤에 shorcut이 위치합니다.
-   activation: shorcut으로 진행되는 함수입니다.

```
[yolo]
mask = 6,7,8
anchors = 10,13,  16,30,  33,23,  30,61,  62,45,  59,119,  116,90,  156,198,  373,326
classes=80
num=9
jitter=.3
ignore_thresh = .7
truth_thresh = 1
random=1
```

 \[yolo\]에는 물체를 감지할 때 사용하는 설정이 있습니다.

-   mask: 아래 anchors에서 해당하는 번호의 값만 사용합니다.
-   anchors: anchor는 검출 객체의 '너비 초기값, 높이 초기값'으로 주어진 값입니다.
-   classes: class의 수, 즉 감지할 종류의 개수입니다.
-   num: anchors의 전체 개수입니다.
-   jitter: 이미지를 랜덤한 수치로 crop합니다. 단, 랜덤한 수의 범위가 jitter에 따릅니다.
-   ignore\_thresh: IOU가 loss 계산에서 사용될 지 여부에 대한 기준값입니다.
-   truth\_thresh: 계산에 포함된 IOU threshold의 크기 값입니다.
-   random: 1인 경우, network-input에 다른 size를 사용합니다.

```
[route]
layers = -4
```

 \[route\]는 현재 output layer와 이전 output layer를 연결하여 미세조정을 합니다.

-   layers: 해당 layer의 몇 번째 이전 layer의 값을 output layer로 내보냅니다.

```
[upsample]
stride=2
```

 \[upsample\]는 convolution을 통해 데이터를 키우는 방법입니다.

-   stride: 값의 배수에 따라 feature map을 upsampling 합니다.


## 기본적인 사용법

YOLO는 Object detection과 training으로 나눌 수 있습니다.

-   #### Object Detection
    

```
./darknet detect <cfg file> <weight file> <image file>
```

 사진에 사용하고 싶을 때 cfg 파일과 weight 파일이 있다면, 위의 명령어를 적용할 수 있습니다.

```
./darknet detector demo <data file> <cfg file> <weight file> <video file>
```

 동영상에 사용하고 싶을 때 위의 명령어를 적용하면 실시간으로 처리합니다. 튜토리얼에서는 data file에 coco.data를 씁니다.

-   #### Training
    

```
./darknet detector train <data file> <cfg file> darknet53.conv.74
```

위의 명령어를 사용하여 model을 생성할 수 있습니다. data file에 dataset을 씁니다. cfg파일에 yolov3를 쓰면 YOLO v3를 사용한 것입니다.


## 출처

 yolo 정식 홈페이지:[pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

 yolo v1~v3:[medium.com/curg/you-only-look-once-%EB%8B%A4-%EB%8B%A8%EC%A7%80-%ED%95%9C-%EB%B2%88%EB%A7%8C-%EB%B3%B4%EC%95%98%EC%9D%84-%EB%BF%90%EC%9D%B4%EB%9D%BC%EA%B5%AC-bddc8e6238e2](https://medium.com/curg/you-only-look-once-%EB%8B%A4-%EB%8B%A8%EC%A7%80-%ED%95%9C-%EB%B2%88%EB%A7%8C-%EB%B3%B4%EC%95%98%EC%9D%84-%EB%BF%90%EC%9D%B4%EB%9D%BC%EA%B5%AC-bddc8e6238e2)

 yolo v4:[jetsonaicar.tistory.com/68](https://jetsonaicar.tistory.com/68)

 yolo v5:[lv99.tistory.com/73](https://lv99.tistory.com/73)

 cfg paramter:[blog.naver.com/hyunmonn/222015942781](https://blog.naver.com/hyunmonn/222015942781)
