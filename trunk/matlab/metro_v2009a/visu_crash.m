close all;
plot(temps,ecarts);
hold on;
plot(temps,angles_consignes(:,1)-angles(:,1));
title('ecarts');
figure
plot(temps,vitesses);
title('vitesses');
figure;
plot(temps,angles_consignes,'r')
title('positions');
hold on
plot(temps,angles)

figure;
plot(temps,commandes_courant)
hold on
plot([temps(1) temps(length(temps))],[4096 4096])
title('commande');