clear all;
close all;

ImportMesures('mesures_simule.txt','');

Te=2e-3;
id=50;
L = 26000;

while (Acc_Pos(id)~=0)
    id=id+1;
end
iend = id-10;

plot([Acc_Pos ; Dec_Pos])
figure
plot([Acc_Vit ; Dec_Vit])
figure
plot([Acc_I ; Dec_I])
figure

Acc_Pos= Acc_Pos(1:iend)-Acc_Pos(1);
Acc_Vit= Acc_Vit(1:iend);
Acc_I= Acc_I(1:iend);
t=0:Te:((iend-1)*Te);
Acc_M=(t'>-1)*4095;
Acc_M(1)=0;
Acc_M(2)=0;


vit = [0 ; (Acc_Pos(2:end)-Acc_Pos(1:(end-1)))/Te]; % pas/s
Vmax = mean(vit(end-50:end)) ; % pas/s
Vitmax = 1./mean (Acc_Vit(end-50:end));
Kvit = Vmax/Vitmax 
TicT4 = 1/(40e6/200)
Kvitth=(4/TicT4)
Kvit=Kvitth;
K = Vmax/4096

id=1;
while (1./Acc_Vit(id)<(Vitmax*0.62))
    id= id+1;
end
Tau = t(id)
%Kvit = 7.0835e+006
%      Cote     Avant   Pente    Cote   Arriere  Pente 
%K =   0.4596   0.5282  0.5960   0.4596  0.5008  0.5529
%Tau = 0.338    0.3100  0.3060    0.3        0.28   0.3040
sysv=tf(K,[Tau 1]);
sysp=tf(K,[Tau 1 0]);
vmod=lsim(sysv,Acc_M,t);


pmod=lsim(sysp,Acc_M,t);
pmes=lsim(tf(1,[1 0]),1./Acc_Vit*Kvit,t);

plot(t,vit,'r'); hold on;
plot(t,1./Acc_Vit * Kvit,'g')
plot(t,vmod,'k');

figure
plot(t,Acc_Pos,'k');hold on
plot(t,pmod,'r');
plot(t,pmes,'g');



%% Vérification pour le simulateur

		K = 0.4979;
		Tau= 0.3040;
		
		Te = t(2)-t(1);
		
		K0 = K/(1.0+2.0*Tau/Te);
		bb1 = (1.0-2.0*Tau/Te)/(1.0+2.0*Tau/Te);
 
        Kp = 0.35436
       Tw = 0.022858
       z = 6.4233
       Tz = 0.78006
		
      a0=1+2*Tz/Te;
      a1=2;
      a2=1-2*Tz/Te;
      b0=1+4*z*Tw/Te+4*Tw^2/Te^2;
      b1=2-8*Tw^2/Te^2;
      b2=1-4*z*Tw/Te+4*Tw^2/Te^2;
      a0=Kp*a0/b0;
      a1=Kp*a1/b0;
      a2=Kp*a2/b0;
      b1=b1/b0;
      b2=b2/b0;
      
      
       vitesse = 0*t;
	   angle=0*t;
       cour=0*t;
   
       k=2;
       angle(k)  =  angle(k) +  vitesse(k) * Te;
	  vitesse(k) = -bb1 * vitesse(k-1) + K0 * (Acc_M(k)+Acc_M(k-1)) ;
 
    for k=3:length(t)
			angle(k)  =  angle(k-1) +  vitesse(k-1) * Te;
			vitesse(k) = -bb1 * vitesse(k-1) + K0 * (Acc_M(k)+Acc_M(k-1)) ;
            cour(k) = a0*Acc_M(k) + a1*Acc_M(k-1) + a2*Acc_M(k-2) -b1*cour(k-1) -b2*cour(k-2);
    end	

    figure;
    plot(t,vitesse,'r'); hold on;   
    plot(t,vmod,'k');

    figure
    plot(t,pmod,'k');hold on;
    plot(t,angle,'r');

    figure
    plot(t,Acc_I,'k');hold on;
    plot(t,cour,'r');

