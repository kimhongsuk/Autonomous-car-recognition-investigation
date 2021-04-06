## 코드 소개

 pre-trained model을 가지고 object detection을 하기 전에, 코드 설명을 하겠습니다.

```
./darknet detect cfg/yolov3.cfg yolov3.weights data/dog.jpg
```

 공식홈페이지에는 위와 같은 명령어가 소개되어 있습니다. 이를 통해 darknet.c의 파일의 detect란 명령을 내렸다는 것을 알 수 있습니다.

```
#include "darknet.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h

extern void test_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh, float hier_thresh, char *outfile, int fullscreen);

int main(int argc, char **argv)
{
    //test_resize("data/bad.jpg");
    //test_box();
    //test_convolutional_layer();
    if(argc < 2){
        fprintf(stderr, "usage: %s <function>\n", argv[0]);
        return 0;
    }
    gpu_index = find_int_arg(argc, argv, "-i", 0);
    if(find_arg(argc, argv, "-nogpu")) {
        gpu_index = -1;
    }

#ifndef GPU
    gpu_index = -1;
#else
    if(gpu_index >= 0){
        cuda_set_device(gpu_index);
    }
#endif

    if (0 == strcmp(argv[1], "average")){
        average(argc, argv);
    } else if (0 == strcmp(argv[1], "yolo")){
        run_yolo(argc, argv);
    } else if (0 == strcmp(argv[1], "super")){
        run_super(argc, argv);
    } else if (0 == strcmp(argv[1], "lsd")){
        run_lsd(argc, argv);
    } else if (0 == strcmp(argv[1], "detector")){
        run_detector(argc, argv);
    } else if (0 == strcmp(argv[1], "detect")){
        float thresh = find_float_arg(argc, argv, "-thresh", .5);
        char *filename = (argc > 4) ? argv[4]: 0;
        char *outfile = find_char_arg(argc, argv, "-out", 0);
        int fullscreen = find_arg(argc, argv, "-fullscreen");
        test_detector("cfg/coco.data", argv[2], argv[3], filename, thresh, .5, outfile, fullscreen);
    }
    return 0;
}

```

 위의 코드는 detect.c를 보면 너무 방대해서 명령어와 직접적으로 연관되어 있는 부분을 모은 코드입니다.

 명령어를 보게 되면 argv\[1\]부터 차례대로 detect, cfg파일, weight파일, image파일이 들어갑니다. 그 후 main 함수에서 어떻게 진행되는지 순차적으로 설명합니다.

-   입력 파라미터가 2개 미만이면 fprintf()를 통해 에러처리합니다.
-   gpu 옵션이 있으면 gpu\_index를 할당하고 -nogpu면 gpu\_index에 -1을 대입해 cuda\_set\_device를 실행합니다.
-   입력 파라미터에 따라 실행시키는 if문에 돌입합니다. 그래서 strcmp(argv\[1\], "detect")란 부분이 있는데, test\_detector를 실행시키게 됩니다.
-   test\_detector는 extern void test\_detector()로 선언되어 있습니다. extern은 파일 밖의 함수를 쓸 수 있게 하는 방법이기 때문에, test\_detector를 찾습니다.

```
void test_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh, float hier_thresh, char *outfile, int fullscreen)
{
    list *options = read_data_cfg(datacfg);
    char *name_list = option_find_str(options, "names", "data/names.list");
    char **names = get_labels(name_list);

    image **alphabet = load_alphabet();
    network *net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    srand(2222222);
    double time;
    char buff[256];
    char *input = buff;
    float nms=.45;
    while(1){
        if(filename){
            strncpy(input, filename, 256);
        } else {
            printf("Enter Image Path: ");
            fflush(stdout);
            input = fgets(input, 256, stdin);
            if(!input) return;
            strtok(input, "\n");
        }
        image im = load_image_color(input,0,0);
        image sized = letterbox_image(im, net->w, net->h);
        //image sized = resize_image(im, net->w, net->h);
        //image sized2 = resize_max(im, net->w);
        //image sized = crop_image(sized2, -((net->w - sized2.w)/2), -((net->h - sized2.h)/2), net->w, net->h);
        //resize_network(net, sized.w, sized.h);
        layer l = net->layers[net->n-1];


        float *X = sized.data;
        time=what_time_is_it_now();
        network_predict(net, X);
        printf("%s: Predicted in %f seconds.\n", input, what_time_is_it_now()-time);
        int nboxes = 0;
        detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
        //printf("%d\n", nboxes);
        //if (nms) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);
        if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
        draw_detections(im, dets, nboxes, thresh, names, alphabet, l.classes);
        free_detections(dets, nboxes);
        if(outfile){
            save_image(im, outfile);
        }
        else{
            save_image(im, "predictions");
#ifdef OPENCV
            make_window("predictions", 512, 512, 0);
            show_image(im, "predictions", 0);
#endif
        }

        free_image(im);
        free_image(sized);
        if (filename) break;
    }
}
```

 폴더 내 다른 파일들을 찾아보면, detector.c에 test\_detector를 찾을 수 있습니다.

 darknet.c로 부터 test\_detector 함수에 cfg/coco.data", argv\[2\], argv\[3\], filename, thresh, .5, outfile, fullscreen이 넘어옵니다. 그 후 함수에서 어떻게 진행되는지 순차적으로 설명합니다.

-   cfg/coco.data의 내용들을 리스트로 받아 "names"의 key값을 이용해 label을 가져옵니다.
-   load\_network(cfgfile, weightfile, 0)에서 cfg파일과 weightfile을 인자로 받아 network 구조체 net을 반환합니다.
-   set\_batch\_network는 네트워크 구조체와 int값을 인자로 받아 네트워크 구조체의 멤버변수 batch의 값을 int값으로 변경합니다
-   각종 필요한 변수 선언을 끝으로 모든 설정이 끝났으면 반복문을 시작합니다.
-   먼저 file이 제대로 들어왔는지 체크를 합니다. 제대로 들어와 있으면 input에 filename을 넣어주고, 없으면 image path를 입력합니다.
-   파일 이름으로 image를 load하고 image를 정방으로 resize하고 net에 맞게 다시 resize해줍니다. 그리고 net에서 layer중 일부를 저장합니다.
-   resize한 image의 data를 X포인터에 저장하면 예측을 시작하고, 걸린 시간을 계산해 알려줍니다.
-   bounding box들의 정보를 받아와 중복처리 작업을 한 후 image위에 bounding box를 그리고 저장합니다.
-   마지막으로 저장된 이미지는 초기화합니다.

 실행 순서대로 코드를 설명했으나, detector.c에서 알 수 없는 함수들이 나옵니다. 그래서 다른 파일을 찾아서 추가적으로 함수를 설명합니다. (직관적으로 정체를 알기 쉬운 함수는 생략합니다)

```
network *load_network(char *cfg, char *weights, int clear)
{
    network *net = parse_network_cfg(cfg);
    if(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    if(clear) (*net->seen) = 0;
    return net;
}
```

 \*.cfg 파일을 파싱하고 \*.weights 파일을 로드합니다. 만약 clear 옵션이 들어오면 network 구조체의 멤버 seen을 0으로 초기화하고 network 구조체 net을 반환합니다.

```
void set_batch_network(network *net, int b)
{
    net->batch = b;
    int i;
    for(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        if(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        if(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}
```

 network 구조체의 멤버변수 batch를 b로 변경합니다. 그리고 network 구조체의 모든 레이어멤버의 batch 값을 b로 변경합니다. 위의 과정에 맞춰서 CUDNN일 경우에 CUDNN 코드를 변경합니다.

```
float *network_predict(network *net, float *input)
{
    network orig = *net;
    net->input = input;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    float *out = net->output;
    *net = orig;
    return out;
}
```

 net를 복사하여 저장해두고난 후 forward\_network를 호출하고 결과값을 반환합니다.

```
void forward_network(network *netp)
{
#ifdef GPU
    if(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        return;
    }
#endif
    network net = *netp;
    int i;
    for(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        if(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.input = l.output;
        if(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}
```

 GPU가 있다면 GPU 전용 forward 함수를 실행하고, 없다면 바로 layer들을 통과시킵니다.

```
detection *get_network_boxes(network *net, int w, int h, float thresh, float hier, int *map, int relative, int *num)
{
    detection *dets = make_network_boxes(net, thresh, num);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    return dets;
}
```

 bounding box를 만들고 맞는지 아닌지 찾습니다.

```
image load_image_color(char *filename, int w, int h)
{
    return load_image(filename, w, h, 3);
}
```

 받았던 파라미터 filename, w, h를 이용해서 load\_image를 호출하고, 결과를 반환합니다.

```
image load_image(char *filename,int w,int h,int c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#else
    image out = load_image_stb(filename, c);
#endif
 
    if((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    return out;
}

```

 OPENCV 옵션이 설정되어있는 상태로 make되었다면, filename, c를 이용하여 load\_image\_cv를 호출하고 그 결과값을 out에 대입합니다.

 OPENCV 옵션이 설정되어있지 않은 상태로 make되었다면, filename, c를 이용하여 load\_image\_stb를 호출하고 그 결과값을 out에 대입합니다.

 w, h값이 존재하고, 읽어들인 image의 h와 w가 서로 일치하지 않다면 w, h와 out을 파라미터로 하는 resize\_image를 호출하여 대입합니다.

```
image **load_alphabet()
{
    int i, j;
    const int nsize = 8;
    image **alphabets = calloc(nsize, sizeof(image));
    for(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, sizeof(image));
        for(i = 32; i < 127; ++i){
            char buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    return alphabets;
}
```

 image 구조체 이중 포인터 alphabets을 선언하고 구조체 image 8개 크기로 메모리 동적할당합니다. 그리고 각 alphabets 요소에 128개의 image 구조체만큼 메모리 동적할당을 8번 반복합니다.

 위의 반복 마다 char 배열 buff를 256크기의 사이즈로 선언하고 반복할 때 마다 buff에 각 png의 경로를 넣어줍니다. 그리고 alphabets의 2차원 배열에 데이터 경로를 넣어준 buff를 인자로 갖는 load\_image\_color를 호출해 반환값을 대입합니다.

```
image letterbox_image(image im, int w, int h)
{
    int new_w = im.w;
    int new_h = im.h;
    if (((float)w/im.w) < ((float)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } else {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    image boxed = make_image(w, h, im.c);
    fill_image(boxed, .5);
    //int i;
    //for(i = 0; i < boxed.w*boxed.h*boxed.c; ++i) boxed.data[i] = 0;
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
    return boxed;
}
```

  letterbox\_image는 Image 비율에 맞춰서 이미지를 정방으로 resize 해주고, 이를 다시 네트워크 입력에 맞춰 재 resize하는 역할입니다. 참고로 letterbox의 의미는 이미지에 여분의 픽셀을 추가하는 것입니다. 


## 실행

 OS는 Ubuntu 18.04이며 YOLO 외에 추가설치할 것이 없습니다. 

```
git clone https://github.com/pjreddie/darknet
cd darknet
make
```

 YOLO는 설치파일이 아니라, github의 코드들을 받아와야 합니다.

```
wget https://pjreddie.com/media/files/yolov3.weights
```

 그리고 예제를 하기 위해서는 weight 파일을 받아야 합니다.

```
./darknet detect cfg/yolo.cfg yolov3.weights data/dog.jpg
```

 준비 사항들을 끝마쳤다면, Object Detection을 실행합니다. 메시지가 올라가고 난 후, darknet 폴더 안에 predictions.jpg란 이름으로 결과물이 나옵니다.

<img src="https://github.com/kimhongsuk/Autonomous-car-recognition-investigation/blob/main/img/YOLO/Result.png", width="650">


## 출처

 YOLO 공식 홈페이지:[pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

 코드: [github.com/pjreddie/darknet](https://github.com/pjreddie/darknet)

 코드 설명 참고: [dhhwang89.tistory.com/126?category=733930](https://dhhwang89.tistory.com/126?category=733930)
