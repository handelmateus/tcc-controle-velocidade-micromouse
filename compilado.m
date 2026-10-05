%% inicializa.m
function z = inicializa(porta)
global SerialESP
disp('Inicializando Serial...');
SerialESP = serial(porta); %<--change this appropriately
set(SerialESP,'BaudRate', 115200, 'DataBits', 8, 'Parity', 'none','StopBits', 1, 'FlowControl', 'none');
fopen(SerialESP); %--open the serial port to the ESP
disp('Comunicação Estabelecida, iniciando Tunel...');
fprintf(SerialESP,'%c','i');
pause(2);
clc;

%% recebe_altura.m
function altura = recebe_altura
global SerialESP
flushinput(SerialESP);
fprintf(SerialESP,'%c','s');
altura = fscanf(SerialESP,'%d');  
if isnan(altura) %Checagem de erro
     fprintf(SerialESP,'%c','s');
     altura = fscanf(SerialESP,'%d');
end
flushoutput(SerialESP);
end

%% set_pwm.m
function x = set_pwm(duty)
global SerialESP
fprintf(SerialESP,'%c','p') ;
if duty > 100
    duty = 100;
elseif duty < 0
    duty = 0;
end
dutyy = num2str(duty);
fprintf(SerialESP, '%s\n', dutyy);
end

%% finaliza.m
global SerialESP
fprintf(SerialESP,'%c','i');
flushoutput(SerialESP);
fclose(SerialESP); %--close the serial port when done
delete(SerialESP);
delete(instrfind);
disp('Comunicação Finalizada');
pause(2);
clear;
clc;