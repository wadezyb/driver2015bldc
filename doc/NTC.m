%http://blog.sina.com.cn/s/blog_54c059840100qgwm.html
R = 10;
B = 3380;
T2 = 25+273.15;
V = 1:4000;
T1 = 1./( log( 40960/R./V-10/R )./B + 1/T2 )-273.15;
plot(V,T1)
fileID = fopen('data.txt','w')
fprintf(fileID,'%3.2f,',T1);
fclose(fileID);