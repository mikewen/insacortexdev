close all;
plot(temps,angles-angles_consignes);
title('ecarts sur consigne');

figure
plot(temps,ecarts+D);
hold on;
plot([temps(1) temps(length(temps))],[D D],'k')
plot([temps(1) temps(length(temps))],[-D -D],'k')
title('ecarts entre voiture');

figure
plot(temps,vitesses);
title('vitesses');


figure;
plot(temps,angles_consignes,'--')
hold on
plot(temps,angles)
title('positions');

figure;
plot(temps,commandes_courant)
hold on
plot([temps(1) temps(length(temps))],[1 1],'k')
plot([temps(1) temps(length(temps))],[-1 -1],'k')
title('courants');
