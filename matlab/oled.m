%从串口返回的赛道图像处理
clc;
clear;

%读取图像
cameraReceiver = imread('188x120.bmp');
[IMG_BUF_HIGH,IMG_BUF_WIDTH] = size(cameraReceiver);%摄像头尺寸



%读HEX数据
bin_image = textread('出环1.txt','%c');

%循环边界值选定
image_data_length = size(bin_image);
IMG_HIGH    = 56;
IMG_WIDTH  = 94;
frame_size = IMG_HIGH*IMG_WIDTH/8;

%创建一个RGB图片
R=1;G=2;B=3;
Image(IMG_HIGH,IMG_WIDTH,3) = uint8(0);

%计数变量    dec2hex(image(1))    dec2hex(abs(a(1)))
ImageByteCount = 1;

%帧头判断 0x55 0x55
if hex2dec('55') == bin_image(ImageByteCount) && hex2dec('55') == bin_image(ImageByteCount+1)
    ImageByteCount = ImageByteCount + 2; %跳过帧头
    for high = 0:6
        for j = 1:94        %列数增加
            for i = 1:8         %转换一个字节
                PixleTemp = bitget(abs(bin_image(ImageByteCount)),i) * 255;
                Image(i+high*8,j,:) = [PixleTemp, PixleTemp, PixleTemp];
            end
            ImageByteCount = ImageByteCount + 1;
        end
    end
end

%Image = imread('ojbk.bmp');
subplot(1,2,1);
imshow(Image);
title('原始图像');

 


%/* 储存赛道信息 */
midline(IMG_HIGH)   = int16(0);
leftline(IMG_HIGH)    = int16(0);
rightline(IMG_HIGH) = int16(0);

mid = 46;
for i = IMG_HIGH:-1:1
	leftline(i) = 1;
    for j = mid:-1:1       %往左边找
            if Image(i,j) == 0
                leftline(i) = j;
                break
            end
    end

    rightline(i) = IMG_WIDTH;
	for j = mid:IMG_WIDTH       %往右边找
         if Image(i,j)==0
            rightline(i) = j;
            break
         end
	end  

    mid = uint8((leftline(i)+rightline(i))/2);

    if leftline(i) == rightline(i)
        midline(i) = -1;    %%有效的中线已经寻找结束  中线和边线交点
    else
        midline(i) = mid;       
    end

    if midline(i) >0
        Image(i,midline(i),R) = 255; %画出原中线
        Image(i,midline(i),G) = 0;
        Image(i,midline(i),B) = 0; 
    end
end

%中线校正算法

%从下向上找中线和变线交点
CrossoverPointY = 1;
for i = 35:-1:1
    if midline(i) == -1
        CrossoverPointY = i;
        break;
    end
end
%从交点向下找赛道与左边界交点

if CrossoverPointY ~=1 %边线和中线有交点，转弯不大
    if midline(CrossoverPointY+1) < 46
        p1 = leftline;
        p2 = rightline;
        BoundValue = 1; 
    else
        p1 = rightline;
        p2 = leftline;  
        BoundValue = IMG_WIDTH;
    end

    for i = CrossoverPointY+1:35
        if p1(i) ~= BoundValue
            RoadLowBound = i;
            break;
        end
    end

    shift = p2(RoadLowBound) - midline(RoadLowBound);

    %用边线代替中线
    for i = CrossoverPointY:RoadLowBound
        midline(i) = p2(i) - shift;
    end
    midline(CrossoverPointY+1) = BoundValue;

end


%对新中线的一阶滤波

aaaaa = 0.7;
for i = CrossoverPointY+1:IMG_HIGH-1
   midline(i) = aaaaa*midline(i) + (1-aaaaa)*midline(i+1);
   
    if midline(i)>0
        Image(i,midline(i),R) = 0; %画出中线
        Image(i,midline(i),G) = 0;
        Image(i,midline(i),B) = 255;
    end
end


%OELD画赛道特征线
for i = 1:IMG_HIGH
    Image(i,leftline(i),R) = 0; %画出左边线
    Image(i,leftline(i),G) = 255;
    Image(i,leftline(i),B) = 0;
    Image(i,rightline(i),R) = 0; %画出you边线
    Image(i,rightline(i),G) = 255;
    Image(i,rightline(i),B) = 0;
end
subplot(1,2,2);
imshow(Image);
title('中线提取');