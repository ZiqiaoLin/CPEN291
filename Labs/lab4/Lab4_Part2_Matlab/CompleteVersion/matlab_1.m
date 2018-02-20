%5s1 = serial('COM7', 'BaudRate', 9600, 'Parity', 'none', 'DataBits', 8,
%'StopBits', 1, 'FlowControl', 'none');
%fopen(s1);
%val=fscanf(s1);
%result = sscanf(val, '%f');
%fclose(s1);

%    x = 1:1000;
%    y = sin(2*pi*x/1000);
%   hLine = plot(x,y);
%   StripChart('Initialize',gca)
%     for i=1:1000
%       StripChart('Update',hLine,y(i))
%     end
%     

s1 = serial('COM7', 'BaudRate', 9600, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1, 'FlowControl', 'none');
fopen(s1);
x = 1:50;
val=fscanf(s1);
result = 0*x;
H = gobjects(1, 4);

subplot(2,2,1);
H(1) = plot(x, result);
ylim([0 50]);
grid on
title('Temperature');
xlabel('time (s)') % x-axis label
ylabel('Degree(*C)') % y-axis label

subplot(2,2,2);
H(2) = plot(x, result);
ylim([0 50]);
grid on
title('Humidity');
xlabel('time (s)') % x-axis label
ylabel('Percent(%)') % y-axis label

subplot(2,2,3);
H(3) = plot(x, result);
grid on
ylim([0 100]);
title('Temperature from LM35');
xlabel('time (s)') % x-axis label
ylabel('Degree(*C)') % y-axis label
grid on

subplot(2,2,4);
H(4) = plot(x, result);
grid on
ylim([0 1050]);
title('Ambient Light Level');
xlabel('time (s)') % x-axis label
ylabel('LightLevel(foot candle)') % y-axis label

StripChart('Initialize',gca);

    for i = 1:100
      for x = 1:4
         val=fscanf(s1);
         result = sscanf(val, '%f');
         StripChart('Update', H(x), result(x));
         display(result);
    end 
 end
fclose(s1);