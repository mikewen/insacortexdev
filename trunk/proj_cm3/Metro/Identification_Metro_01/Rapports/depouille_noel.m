clear all;
close all;

ImportMesures('Mesures_Noel_Avant.txt','');

Te=2e-3;
id=50;
L = 26000;

while (Acc_Pos(id)~=0)
    id=id+1;
end
iend = id-10;

Acc_Pos= Acc_Pos(1:iend);
Acc_Vit= Acc_Vit(1:iend);
Acc_I= Acc_I(1:iend);
t=0:Te:((iend-1)*Te);


vit = (Acc_Pos(2:end)-Acc_Pos(1:(end-1)))/Te; % pas/s
Vmax = mean(vit(end-100:end)) ; % pas/s
Vitmax = mean (1./Acc_Vit(end-200:end));
Kvit = Vmax/Vitmax 
K = Vmax/4096

id=1;
while (1./Acc_Vit(id)<(Vitmax*0.62))
    id= id+1;
end
Tau = t(id)

%Kvit = 7.0835e+006

%K =    0.5282
%Tau =   0.3100
sysv=tf(K,[Tau 1]);
sysp=tf(K,[Tau 1 0]);
vmod=lsim(sysv,(t>-1)*4095,t);
pmod=lsim(sysp,(t>-1)*4095,t);

plot(t(1:end-1),vit,'r'); hold on;
plot(t,1./Acc_Vit * Kvit,'g')
plot(t,vmod,'k');

figure
plot(t,Acc_Pos,'k');hold on
plot(t,pmod,'r');
