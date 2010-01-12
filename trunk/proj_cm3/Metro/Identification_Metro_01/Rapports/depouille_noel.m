clear all;
close all;
clc;

%% Import  des mesures faites avec le projet d'identification

ImportMesures('mesures_trampoline.txt','');
%Les mesures contiennet les réponses en accélération à un échelon
% positif de 0 à 4096 => Acc_
% et l'échelon négatif de 4096 à 0 => Dec_
% Il y a directement les info des capteurs
%   Pos (pas) issue de Lire_Position();
%   Vit (pas/s) issue de Lire_Vitesse();
%   I (résolution DAC 12 bits) issue de Lire_Courant

Te=2e-3;
id=50;
L = 26000;

%% Affiche les données brutes
plot([Acc_Pos ; Dec_Pos])
figure
plot([Acc_Vit ; Dec_Vit])
figure
plot([Acc_I ; Dec_I])
figure

%% On identifie sur la phase accel
Est_Accel=1; %%TODO décel ne marche pas 
if (Est_Accel)
    Mes_Pos = Acc_Pos;
    Mes_Vit = Acc_Vit;
    Mes_I = Acc_I;
    COM = 4096;
else
    Mes_Pos = Dec_Pos;
    Mes_Vit = Dec_Vit;
    Mes_I = Dec_I;
    COM=-4096
end 

%% enlève des 0 fictifs en fin de mesures (buffer non rempli)
while ((Mes_Pos(id)~=0) & id < length(Mes_Pos))
    id=id+1;
end
iend = id-10;


Mes_Pos= Mes_Pos(1:iend)-Mes_Pos(1);
Mes_Vit= Mes_Vit(1:iend);
Mes_I= Mes_I(1:iend);
t=0:Te:((iend-1)*Te);

%% Génère la commande en voyée
Mes_M=(t'>-1)*COM;
Mes_M(1)=0;
Mes_M(2)=0;

% mesure de la vitesse à partir de Pos et de Vmax en fin de réponse
vit = [0 ; (Mes_Pos(2:end)-Mes_Pos(1:(end-1)))/Te]; % pas/s
Vmax = mean(vit(end-50:end)) ; % pas/s issu du calcul Matlab
Vitmax = mean (Mes_Vit(end-50:end)); % pas/s issu de la mesure STM32

%% Mesure du gain du modèle
K = Vmax/4096

%% mesure du temps de réponse
id=1;
while (Mes_Vit(id)<(Vitmax*0.62))
    id= id+1;
end
Tau = t(id)

%% Quelques mesures faites à noel
%      Cote     Avant   Pente    Cote   Arriere  Pente 
%K =   0.4596   0.5282  0.5960   0.4596  0.5008  0.5529
%Tau = 0.338    0.3100  0.3060    0.3        0.28   0.3040

%% Confrontation modèle théorique mesure
sysv=tf(K,[Tau 1]); % Vitesse
sysp=tf(K,[Tau 1 0]); % Position
vmod=lsim(sysv,Mes_M,t);


pmod=lsim(sysp,Mes_M,t);
pmes=lsim(tf(1,[1 0]),Mes_Vit,t);

plot(t,vit,'r'); hold on;
plot(t,Mes_Vit,'g')
plot(t,vmod,'k');
title('Vitesse');

figure
plot(t,Mes_Pos,'k');hold on
plot(t,pmod,'r');
plot(t,pmes,'g');
title ('Positions');


%% Vérification du modèle discret pour le simulateur de Keil

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
	  vitesse(k) = -bb1 * vitesse(k-1) + K0 * (Mes_M(k)+Mes_M(k-1)) ;
 
    for k=3:length(t)
			angle(k)  =  angle(k-1) +  vitesse(k-1) * Te;
			vitesse(k) = -bb1 * vitesse(k-1) + K0 * (Mes_M(k)+Mes_M(k-1)) ;
            cour(k) = a0*Mes_M(k) + a1*Mes_M(k-1) + a2*Mes_M(k-2) -b1*cour(k-1) -b2*cour(k-2);
    end	

%% Confrontation Modèle discret Keil avec modèle théorique
    figure;
    plot(t,vitesse,'r'); hold on;   
    plot(t,vmod,'k');

    figure
    plot(t,pmod,'k');hold on;
    plot(t,angle,'r');

    figure
    plot(t,Mes_I,'k');hold on;
    plot(t,cour,'r');

