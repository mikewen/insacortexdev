% foncition d'interruption lors d'un reception de caractère sur COM2

function callread(obj,event)
disp('Fonction d IT');
chaine = fgets(obj);   % reception de la chaine 
fprintf('%s',chaine(1:length(chaine)-1)) % envoie sur la fenetre command de matlab
disp('juste apres');
fprintf('\n==>');  % regeneration du "prompt"
%return
if (strcmp(chaine(1),'$'))  % detection d'un demande de fin de communication
    obj.userdata = 1;    % affecte le drapeau global pour procedure appelante
    disp('Taper return pour sortir');
end
if (strcmp(chaine(1),'#') |  strcmp(chaine(2),'#'))  % detection d'un demande de téléchargement d'entiers
    disp('Telechargement d''entiers en ASCII');
    obj.BytesAvailableFcn = '';  %  passe en mode sans IT : le flux de donnée est récupéré en scrutatif direct
    obj.userdata = 2;     % affecte le drapeau global pour procedure appelante
    disp('Appuyer sur une touche pour continuer');   
end
if (strcmp(chaine(1),'%') |  strcmp(chaine(2),'%'))  % detection d'une demande de téléchargement d'entiers en binaire
    disp('Telechargement d''entiers en binaire');
    obj.BytesAvailableFcn = '';  %  passe en mode sans IT : le flux de donnée est récupéré en scrutatif direct
    obj.userdata = 3;     % affecte le drapeau global pour procedure appelante
    disp('Appuyer sur une touche pour continuer');   
end


