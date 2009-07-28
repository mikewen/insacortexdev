close all;
title('ecarts');
plot(temps,ecarts);
figure
title('vitesses');
plot(temps,vitesses);
figure;
plot(temps,angles_consignes)
hold on
plot(temps,angles)

figure;
plot(temps,commandes_courant)
hold on
plot([temps(1) temps(length(temps))],[8 8])