close all;

%%
%title('ecarts');
%plot(temps,ecarts);
%figure
dist_max=max(max(abs(ecarts)))

Istop = ceil(interp1(temps,1:length(temps),Tstop));
%%
title('vitesses');
plot(temps,vitesses);
hold on;
plot(temps,vit_c,'--k')
plot(temps(Istop),vitesses(Istop,:),'o');
vit_max=max(vitesses(Istop,:))
%%
figure;
plot(temps,pos_c,'--k')
hold on
plot(temps,angles)
plot(temps(Istop),angles(Istop,:),'o');
eca_max=max(angles(Istop,:)-(Pfin+(0:N-1)*Delta))
%%
figure;
plot(temps,commandes_courant)
hold on
plot([temps(1) temps(length(temps))],[1 1])