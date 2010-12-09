clear all ;
close all;
clc;


Te=2E-3; % période d'échantillonnage de la commande


Delta=10; %% écart initial entre les loco au démarrage

%% Initialisation du générateur de trajectoire
%   Génère des consignes de position et de vitesse cohérentes selon le profil 
%   de vitesse suivant
% 
% Vitesse
%   /\         Vmax ----> _______________________________
%    |                  / I                             I\
%    |                 /  I                             I \
%    |                /	  I                             I  \
%    |			     /	  I                             I   \
%    |		  	    /	  I 							I	 \
%    |   __________/	  I                             I     \____________
%    |_____________I______I_____________________________I___________________\ temps
%                  |       |_                           |<---->|_          /
%                  |		 \_                         |		  \_
%                  |           \_                       |		    \_
%                  |<----------->|                      |<----------> |
%                  |  tpsm   					   	    |      tpsm 
%                 tda                                   tdd
%
% L'intégrale de ce profil de vitesse donne une position qui doit se
% terminer à la valeur Dist désirée
%   Surface = Position finale = Dist = (tdd-tda)*Vmax

Dist=32000; % distance à parcourir
tda=0.5;    %temps début d'accélération (s)
tpsm=2;     %temps de montée (s)
Vmax=1000;  % Vitesse Maximale (pas/s)
tdd=tda+Dist/Vmax; % temps début de descente
Amax = Vmax/tpsm;  % accélération maximale (pas/s/s)
L=tdd+tpsm+2;   % horizon de simulation

Vitmax = 1200 %% pas/s


%% Valeurs des N modèles de locomotive
% Vit(p)          K
% _______  =  ____________
%  Com(p)      1 + Tau . p

N=3; % nombre de loco simulées

%% initialise N gains K autour de la valeur nominale
Knom = 0.4979;          % Gain nominal
sigma_K = Knom/100;     % 1% devariance de K sur l'univers probabiliste des différentes locos
K=(randn(N,1)*sigma_K)+Knom; % Vecteur des N gains K de chaque loco simulée
K(end)=K(end)*1;   


%% initialise N constante Tu autour de la valeur nominale
Taunom=3E-2; % constante de temps d'une loco 
sigma_Tau=Taunom/10; % forte variance de 10% car il peut y avoir
                     % beaucoup de monde dans une voiture de métro
Tau=(randn(N,1)*sigma_Tau)+Taunom; % Vecteur des N constante de temps Tau des locos
Tau(end) = 1*Tau(end);


[Kr,S,E,Kry,Sy,Ey]=calcul_retour_etat(Knom,Taunom,N);
dim=3;
Ki=Kr(:,1:dim:dim*N)*(dim-2)
Ka=Kr(:,dim-1:dim:dim*N)
Kv=Kr(:,dim:dim:dim*N)



sim n_metro_discret;
Puissance_LQ = sum(commandes.*commandes)/length(commandes)
Puissance_Tot_LQ = sum(Puissance_LQ)/N
visu_crash;

pause;
close all;
hold off;
Ki=Kry(:,1:dim:dim*N)*(dim-2)
Ka=Kry(:,dim-1:dim:dim*N)
Kv=Kry(:,dim:dim:dim*N)
sim n_metro_discret;
Puissance_LQY = sum(commandes.*commandes)/length(commandes)
Puissance_Tot_LQY = sum(Puissance_LQY)/N

 visu_crash