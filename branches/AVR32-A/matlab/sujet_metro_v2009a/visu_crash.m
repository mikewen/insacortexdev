close all;
plot(temps,positions_consignes(:,1)-positions(:,1));
title('ecarts');
figure
plot(temps,vitesses_consignes,'k--')
hold on
plot(temps,vitesses);
title('vitesses');
figure;
plot(temps,positions_consignes,'k--')
title('positions');
hold on
plot(temps,positions)

figure;
plot(temps,commandes)
hold on
plot([temps(1) temps(length(temps))],[4096 4096])
title('commande');