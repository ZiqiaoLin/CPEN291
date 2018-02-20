% Date: Febrary 3rd, 2017
% Purpose: Get familiar with Matlab and finish CPEN 291 lab4

s1 = serial('COM7', 'BaudRate', 9600, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1, 'FlowControl', 'none');
fopen(s1);
x = 1:10;
val=fscanf(s1);
result = 0*x;

%Create a graphic object
H = gobjects(1, 4);

%Create 4 subplots to plot 4 types of data received 
subplot(2,2,1);
%Initialize one element in the H graphic object
H(1) = plot(x, result, 'r');
ylim([0 50]);
%Turn on the grid
grid on
%Add the title, and x, y label
title('Temperature');
xlabel('time (s)'); % x-axis label
ylabel('Degree(*C)'); % y-axis label

subplot(2,2,2);
H(2) = plot(x, result, 'm');
ylim([0 50]);
grid on
title('Humidity');
xlabel('time (s)'); % x-axis label
ylabel('Percent(%)'); % y-axis label

subplot(2,2,3);
grid on
H(3) = plot(x, result, 'Color',[0.2,0.1,0.9]);
ylim([0 100]);
title('Temperature from LM35');
xlabel('time (s)'); % x-axis label
ylabel('Degree(*C)'); % y-axis label
grid on;

subplot(2,2,4);
grid on;
H(4) = plot(x, result, 'Color',[0.2,0.5,0.4]);
ylim([0 1050]);
title('Ambient Light Level');
xlabel('time (s)'); % x-axis label
ylabel('LightLevel(foot candle)'); % y-axis label

StripChart('Initialize',gca);

%Use a nested loop to plot 4 liines of data simoultaneously
    for i = 1:100
      %loop through four elements in the reslut array, plot each of them
      %one by one 
      for x = 1:4
         %store the received the data in the val string
         val=fscanf(s1);
         %use an array to store the float data in the array
         result = sscanf(val, '%f');
         StripChart('Update', H(x), result(x));
         %Display the result on the comment window
         display(result);
    end 
    end
  
%close the serial port in the end
fclose(s1);
