clc;clear;
%��·�����ж�

IMG_HIGH = 240; 
IMG_WIDTH = 376; 

Image = imread('�����ұ���.BMP');

 subplot(2,2,1);
 imshow(Image);
 title('ԭʼͼ��');
 
 %/* ����������Ϣ */
midline(IMG_HIGH)   = int16(0);
leftline(IMG_HIGH)    = int16(0);
rightline(IMG_HIGH) = int16(0);


%����У��ǰ��Ԥ����������򵥵� ����ҳ��Զ�
mid = int16(IMG_WIDTH/2);
for i = IMG_HIGH:-1:1
	leftline(i) = 1;
    for j = mid:-1:1       %�������
            if Image(i,j) == 0
                leftline(i) = j;
                break
            end
    end

    rightline(i) = IMG_WIDTH;
    for j = mid:IMG_WIDTH   %���ұ���
         if Image(i,j) == 0
            rightline(i) = j;
            break
         end  
    end

    mid = uint16((leftline(i)+rightline(i))/2);

    if leftline(i) == rightline(i)
        midline(i) = -1;    %%��Ч�������Ѿ�Ѱ�ҽ���  ���ߺͱ��߽���
        
    else
        midline(i) = mid;       
    end
    
end

%�ڶ������� ��ֱ�ӱȽ�����ͷ�е��Ԥ������õ��е㡣
    disp('·���󣬳�����');
    %ѡ�Ҳ��߶��ж�·������
    k1 = midline(80) - midline(120);
    k2 = midline(120) - midline(160); 
    deltaK = k1 - k2;
    
   %�����Զ����б��k1,������б��k2,����б�ʵı仯��deltaK
   if deltaK >= -6 && deltaK <= 6
       roadfalg = 1;
       disp('ֱ��');
   else
       disp('���');
       if k1 < 0
           roadfalg = 2;
           disp('����');
       else
           roadfalg = 2;
           disp('����');
       end
   end 